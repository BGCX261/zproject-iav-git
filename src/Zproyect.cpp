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
	srand((unsigned int)time(0));

	// Create the camera for map navigation:
	cameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("cameraNode", Ogre::Vector3(0, 35, 50));
	cameraNode->attachObject(mCamera);
	cameraMan = new CameraMan(cameraNode, Ogre::Vector3::ZERO ,15);

	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);
		
	// Create a plane for terrain (with texture)
	plane.redefine(Ogre::Vector3::UNIT_Y, Ogre::Vector3(0,0,0) );
		Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        	plane, 300, 300, 1, 1, true, 1, 12, 12, Ogre::Vector3::UNIT_Z);

    	Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
    	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);

    	entGround->setMaterialName("GroundMat");
    	entGround->setCastShadows(false);

	// SkyBox with skydom
	mSceneMgr->setSkyDome(true, "CloudySky", 5, 8);

	// Set ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	// Create a light
	Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setPosition(20,80,50);

	// Create the zombies
	nZombies = 3;
	zombies = new Zombie*[nZombies];
	for (int i = 0; i < nZombies; i++) {
		char aux[20];
		sprintf(aux, "Zombie%d.mesh", rand()%2+1);
		zombies[i] = new Zombie(Ogre::String(aux), 0, i, rand()%(nZombies), rand()%(nZombies), 2, 1);
	}

	//zombiesMovementModel = new UnitMovModelRandom();
	zombiesMovementModel = new UnitMovModelRBSFlock(30, 5);

	enemy = new Enemy(Ogre::String("Turret.mesh"), -20, 10, 2, 1);


	// --------------------- Pruebas --------------------------------

	// Banderita selection with Ray
	Ogre::Entity* banderaEntity = mSceneMgr->createEntity("Banderita", "banderita.mesh");
	banderaEntity->setQueryFlags(OTHER_MASK);
	banderaNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("BanderitaNode",Ogre::Vector3(0,-20,0));
	banderaNode->attachObject(banderaEntity);
	banderaNode->roll(Ogre::Degree(-90));		// Redefine Banderita position (fuck Blender-Ogre exporter)
	banderaNode->pitch(Ogre::Degree(90));

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


	// ------------ MOC ------------------------------------
	mCollisionTools = new MOC::CollisionTools(mSceneMgr);

	myManualObject =  mSceneMgr->createManualObject("manual1"); 
	myManualObjectNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("manual1_node"); 
	 
	myManualObjectMaterial = Ogre::MaterialManager::getSingleton().create("manual1Material", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME); 
	myManualObjectMaterial->setReceiveShadows(false); 
	myManualObjectMaterial->getTechnique(0)->setLightingEnabled(true); 
	myManualObjectMaterial->getTechnique(0)->getPass(0)->setDiffuse(Ogre::ColourValue(255, 0, 0, 0)); 
	myManualObjectMaterial->getTechnique(0)->getPass(0)->setAmbient(0,0,1); 
	myManualObjectMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(0,0,1); 
	//myManualObjectMaterial->dispose();  // dispose pointer, not the material
	 
	 
	myManualObject->begin("manual1Material", Ogre::RenderOperation::OT_LINE_LIST); 
	myManualObject->position(3, 2, 1); 
	myManualObject->position(4, 1, 0); 
	// etc 
	myManualObject->end(); 
	myManualObjectNode->attachObject(myManualObject);

}

//-----------------------------------------------------------------------------------------------
bool Zproyect::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	bool ret = BaseApplication::frameRenderingQueued(evt);

	zombiesMovementModel->preProcess(evt.timeSinceLastFrame);

	cameraMan->update(evt);

	// create a temporal bounding box, for check collisions
	bool isCollission = false;

	// We now move all the units, according to the movement model:
	for (int i = 0; i < nZombies; i++) {

		double x, z;
		if (zombiesMovementModel->calculateMove(zombies, nZombies, i, banderaNode->getPosition(), &x, &z))
			zombies[i]->move(x, z);
		
			// MOC collision
			// Get the old position movement
			Ogre::Vector3 oldPos = zombies[i]->node->getPosition();		

		// Commit the zombie movement
		zombies[i]->update(evt);
		
			// check if we are colliding with anything with a collision radius of 4.0 ogre units and we 
			// set the ray origin 10.0 for the bunker collision
			if (mCollisionTools->collidesWithEntity(oldPos, zombies[i]->node->getPosition(), 4.0f , 10.0f, STATIC_MASK))
			{
				// undo move
				zombies[i]->node->setPosition(oldPos);
			}



		/*if( (zombies[i]->node->getPosition().distance( enemy->node->getPosition() ) <= 25 ) && (zombies[i]->isLive())){
			enemy->fire();
			zombies[i]->damage(evt.timeSinceLastFrame);
		}*/
	}

	zombiesMovementModel->postProcess();

	Ogre::Degree angle	  = enemy->node->getOrientation().getYaw();
	Ogre::Vector3 origin      = enemy->node->getPosition();
	origin.y = 4;
	Ogre::Vector3 destination = Ogre::Vector3(origin.x + 30 * Ogre::Math::Sin(angle), origin.y, origin.z + 30 * Ogre::Math::Cos(angle));

	Ogre::Entity *tmpE = NULL;
	Ogre::Vector3 result = Ogre::Vector3::ZERO;
	float distToColl = 30;

	myManualObject->beginUpdate(0);
	myManualObject->position(origin); 
	myManualObject->position(destination); 
	myManualObject->end(); 

	// enable shoot animation when a zombie is near to robotNode - Kill the zombie		
	if(mCollisionTools->raycastFromPoint(origin, destination, result, tmpE, distToColl, ZOMBIE_MASK)){

		Ogre::String name = tmpE->getName();
		std::vector<Ogre::String, Ogre::STLAllocator<Ogre::String, Ogre::GeneralAllocPolicy> > nameGroups = Ogre::StringUtil::split(name, Ogre::String("-"));
		int group = Ogre::StringConverter::parseInt(nameGroups[1]);
		int individual = Ogre::StringConverter::parseInt(nameGroups[2]);

		if (zombies[individual]->isLive())
		{
			enemy->fire();
			zombies[individual]->damage(evt.timeSinceLastFrame);
		}
	}

	enemy->update(evt);

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
		Ogre::SceneNode* node = tmpE->getParentSceneNode();
		if (node->getShowBoundingBox()) {
			node->showBoundingBox(false);
		} else {
			node->showBoundingBox(true);
		}
	}
}

bool Zproyect::keyPressed( const OIS::KeyEvent &arg )
{
	bool ret = BaseApplication::keyPressed(arg);

	cameraMan->keyPressed(arg);

	if (arg.key == OIS::KC_ADD)
		for(int i=0;i<nZombies;i++) zombies[i]->speed *=1.1;
	
	if (arg.key == OIS::KC_SUBTRACT)
		for(int i=0;i<nZombies;i++) zombies[i]->speed *=0.9;
	
	return ret;
}

bool Zproyect::keyReleased( const OIS::KeyEvent &arg )
{
	bool ret = BaseApplication::keyReleased(arg);

	cameraMan->keyReleased(arg);

	if (arg.key == OIS::KC_ADD)
		for(int i=0;i<nZombies;i++) zombies[i]->speed =zombies[i]->speed;
	
	if (arg.key == OIS::KC_SUBTRACT)
		for(int i=0;i<nZombies;i++) zombies[i]->speed =zombies[i]->speed;

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
		banderaNode->setPosition(pointRay.x,4,pointRay.z);
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
