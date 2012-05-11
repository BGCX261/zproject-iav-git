/*
-----------------------------------------------------------------------------
Filename:    Zproyect.cpp
-----------------------------------------------------------------------------


-----------------------------------------------------------------------------
*/

#include "Zproyect.h"

//-------------------------------------------------------------------------------------
Zproyect::Zproyect(void)
{
}
//-------------------------------------------------------------------------------------
Zproyect::~Zproyect(void)
{
}

//-------------------------------------------------------------------------------------
void Zproyect::createScene(void)
{
	// -----------------------------------------------
	// Auxiliar declarations and actions:
	srand((unsigned int)time(0));


	// -----------------------------------------------
	// Camera init and management:
	cameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("cameraNode", Ogre::Vector3(0, 35, 50));
	cameraNode->attachObject(mCamera);
	cameraMan = new CameraMan(cameraNode, Ogre::Vector3::ZERO ,15);

	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);
		

	// -----------------------------------------------
	// Terrain and enviroment:
	plane.redefine(Ogre::Vector3::UNIT_Y, Ogre::Vector3(0,0,0) );
		Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        	plane, 300, 300, 1, 1, true, 1, 12, 12, Ogre::Vector3::UNIT_Z);

    	Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
    	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
	entGround->setQueryFlags(OTHER_MASK);

    	entGround->setMaterialName("GroundMat");
    	entGround->setCastShadows(false);

	// SkyBox with skydom
	mSceneMgr->setSkyDome(true, "CloudySky", 5, 8);
	// fog
	Ogre::ColourValue fadeColour(0.8, 0.8, 0.7);
	mSceneMgr->setFog(Ogre::FOG_LINEAR, fadeColour, 0.0, 70, 200);

	// Bunker
	Ogre::Entity* bunkerEntity = mSceneMgr->createEntity("Bunker", "bunker.mesh");
	bunkerEntity->setQueryFlags(STATIC_MASK);
	Ogre::AxisAlignedBox bunkerBox = bunkerEntity->getBoundingBox();	// bounding box
	Ogre::SceneNode* bunkerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	bunkerNode->scale(5,5,6);
	bunkerNode->yaw(Ogre::Degree(90) );

	// Node Position (relative to bunker bounding box Left-Bottom)
	bunkerNode->setPosition(10, -bunkerBox.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM).y*5, -40);
	bunkerNode->attachObject(bunkerEntity);


	// -----------------------------------------------
	// Ilumination:
	// Set ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	// Create a light
	Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setPosition(20,80,50);


	// -----------------------------------------------
	// Units:
	// Create the zombies
	nGroups = 2;
	zombieGroups = new ZombiePack*[nGroups];
	for (int i = 0; i < nGroups; i++)
		zombieGroups[i] = new ZombiePack(i, 20, i*20, i*20, 2, 1);

	selectedGroup = 0;
	zombieGroups[selectedGroup]->select();

	//zombiesMovementModel = new UnitMovModelRandom();
	zombiesMovementModel = new UnitMovModelRBSFlock(30, 5);


	// Enemies:
	nEnemies = 1;
	enemies = new Enemy*[nEnemies];
	enemies[0] = new Enemy(Ogre::String("Turret.mesh"), -20, 10, 2, 1);



	// ------------ MOC ------------------------------------
	mCollisionTools = new MOC::CollisionTools(mSceneMgr);

	
	// 	PRUEBAS:::::


}

//-----------------------------------------------------------------------------------------------
bool Zproyect::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	bool ret = BaseApplication::frameRenderingQueued(evt);

	// ------------------------
	// Camera:
	cameraMan->update(evt);


	// ------------------------
	// Zombies:
	Ogre::String **nombres = new std::string*[20];	

	zombiesMovementModel->preProcess(evt.timeSinceLastFrame);
	for (int i = 0; i < nGroups; i++)
	{
		zombieGroups[i]->move(zombiesMovementModel);
		zombieGroups[i]->attack(evt, mCollisionTools, nombres);
		for (int j = 0; j < 20; j++)
		{	
			// Dañamos a los enemigos si procede:
			std::vector<Ogre::String, Ogre::STLAllocator<Ogre::String, Ogre::GeneralAllocPolicy> > nameGroups = Ogre::StringUtil::split(*nombres[j], Ogre::String("."));
			int individual = Ogre::StringConverter::parseInt(nameGroups[1]);
			if (individual != -1)
				enemies[individual]->damage(zombieGroups[i]->getZombie(j)->dps, evt.timeSinceLastFrame);
		}
		zombieGroups[i]->update(evt, mCollisionTools);
	}
	zombiesMovementModel->postProcess();

	// ------------------------
	// Enemies:
	enemies[0]->trace(mCollisionTools, evt, zombieGroups);
	enemies[0]->update(evt);

	return ret;
}




void Zproyect::pickEntity(const OIS::MouseEvent &arg, const Ogre::uint32 queryMask)
{
	Ogre::Entity *tmpE = NULL;
	Ogre::Vector3 result = Ogre::Vector3::ZERO;
	float distToColl;
	Ogre::Vector2 mousePos = Ogre::Vector2(arg.state.X.abs,arg.state.Y.abs);

    if (mCollisionTools->raycastFromCamera(mWindow, mCamera, mousePos, result, tmpE, distToColl, queryMask))
	{
		Ogre::String name = tmpE->getName();
		std::vector<Ogre::String, Ogre::STLAllocator<Ogre::String, Ogre::GeneralAllocPolicy> > nameGroups = Ogre::StringUtil::split(name, Ogre::String("-"));
		int group = Ogre::StringConverter::parseInt(nameGroups[1]);
		zombieGroups[selectedGroup]->deselect();
		selectedGroup = group;
		zombieGroups[selectedGroup]->select();
	}
}

bool Zproyect::keyPressed( const OIS::KeyEvent &arg )
{
	bool ret = BaseApplication::keyPressed(arg);

	cameraMan->keyPressed(arg);

	if (arg.key == OIS::KC_ADD)
		for(int i=0;i<nGroups;i++) zombieGroups[i]->modifySpeed(1.1);
	
	if (arg.key == OIS::KC_SUBTRACT)
		for(int i=0;i<nGroups;i++) zombieGroups[i]->modifySpeed(0.9);
	
	return ret;
}

bool Zproyect::keyReleased( const OIS::KeyEvent &arg )
{
	bool ret = BaseApplication::keyReleased(arg);

	cameraMan->keyReleased(arg);
	/**
	if (arg.key == OIS::KC_ADD)
		for(int i=0;i<nZombies;i++) zombies[i]->speed =zombies[i]->speed;
	
	if (arg.key == OIS::KC_SUBTRACT)
		for(int i=0;i<nZombies;i++) zombies[i]->speed =zombies[i]->speed;
	**/
	return ret;
}

bool Zproyect::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	bool ret = BaseApplication::mousePressed(arg,id);

	// if right mouse button put flag
	if (arg.state.buttonDown(OIS::MB_Right))
	{
		// Ray throught viewport
		Ogre::Ray mouseRay = mCamera->getCameraToViewportRay((float)arg.state.X.abs/arg.state.width, (float)arg.state.Y.abs/arg.state.height );
	
		// Calculate intersection between plane and a 3D point
		std::pair<bool, Ogre::Real> resultRay =  mouseRay.intersects( plane );	
		Ogre::Vector3 pointRay = mouseRay.getPoint(resultRay.second);
		// Put the flag 
		zombieGroups[selectedGroup]->setBandera(pointRay.x, pointRay.z);
	}

	// if left button select a entity
	if(arg.state.buttonDown(OIS::MB_Left)){
		// Pick one
		pickEntity(arg, ZOMBIE_MASK);
	}
	

	return ret;
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
	int main(int argc, char *argv[])
#endif
	{
		// Create application object
		Zproyect app;

		try {
			app.go();
		} catch( Ogre::Exception& e ) {
					#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
						MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
					#else
						std::cerr << "An exception has occured: " <<
							e.getFullDescription().c_str() << std::endl;
					#endif
		}

		return 0;
    	}

#ifdef __cplusplus
}
#endif
