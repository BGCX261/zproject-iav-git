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
	// Setup - Sound
	setupSound();


	// -----------------------------------------------
	// Camera init and management:
	cameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("cameraNode", Ogre::Vector3(0, 35, 50));
	cameraNode->attachObject(mCamera);
	cameraMan = new CameraMan(cameraNode, Ogre::Vector3::ZERO ,25);

	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);
		

	// -----------------------------------------------
	// Terrain and enviroment:
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	
	Ogre::Entity* entGround2 = mSceneMgr->createEntity("terrenoEntity", "terreno.mesh");
	Ogre::SceneNode* nodeGround = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0,0,0));
	nodeGround->attachObject(entGround2);
	nodeGround->scale(80,80/2,80);
	entGround2->setQueryFlags(OTHER_MASK);
       entGround2->setCastShadows(false);

	plane.redefine(Ogre::Vector3::UNIT_Y, Ogre::Vector3(0,0,0) );
	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane, 300, 300, 1, 1, true, 1, 12, 12, Ogre::Vector3::UNIT_Z);

	// hacerlo invisible, PORVISIONAL <-----

	// SkyBox with skydom
	mSceneMgr->setSkyDome(true, "CloudySky", 5, 8);
	// fog
	Ogre::ColourValue fadeColour(0.8, 0.8, 0.7);
	mSceneMgr->setFog(Ogre::FOG_LINEAR, fadeColour, 0.0, 100, 500);

	// Ruins
	Ogre::Entity* ruinsEntity = mSceneMgr->createEntity("Ruins", "ruina.mesh");
	ruinsEntity->setQueryFlags(STATIC_MASK);
	Ogre::AxisAlignedBox ruinsBox = ruinsEntity->getBoundingBox();	// bounding box
	Ogre::SceneNode* ruinsNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	ruinsNode->scale(4,4,4);
	ruinsNode->yaw(Ogre::Degree(-90) );

	// Node Position (relative to ruins bounding box Left-Bottom)
	ruinsNode->setPosition(10, -ruinsBox.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM).y-1, -40);
	ruinsNode->attachObject(ruinsEntity);


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
	nGroups = 3;
	zombieGroups = new ZombiePack*[nGroups];
	zombieGroups[0] = new ZombiePack(0, 20, -60, 80, 3, 1);
	zombieGroups[1] = new ZombiePack(1, 20, 0, 80, 3, 1);
	zombieGroups[2] = new ZombiePack(2, 20, 60, 80, 3, 1);

	selectedGroup = 0;
	zombieGroups[selectedGroup]->select();

	//zombiesMovementModel = new UnitMovModelRandom();
	zombiesMovementModel = new UnitMovModelRBSFlock(30, 5);


	// Enemies:
	nEnemies = 4;
	enemies = new Enemy*[nEnemies];
	enemies[0] = new Enemy(ENEMY_TYPE_TURRET, 0, 10, 10);
	enemies[1] = new Enemy(ENEMY_TYPE_FLYER, 1, -60, 0);
	enemies[2] = new Enemy(ENEMY_TYPE_FLYER, 2, 60, -20);
	enemies[3] = new Enemy(ENEMY_TYPE_ROBOT, 3, 10, -50);

	
	enemyAIJustTurn = new EnemyAIModelJustTurn();
	enemyAIRandom = new EnemyAIModelRandom();
	enemyAIHunt = new EnemyAIModelHunt();

	// Init them:
	for (int e = 0; e < nEnemies; e++)
	{
		switch (enemies[e]->type)
		{
	
		case ENEMY_TYPE_TURRET:
			enemyAIJustTurn->start(enemies[e]);
	    		break;

		case ENEMY_TYPE_FLYER:
			enemyAIRandom->start(enemies[e]);
			break;

		case ENEMY_TYPE_ROBOT:
			enemyAIHunt->start(enemies[e]);
			break;
		}
	}

	// ------------ MOC ------------------------------------
	mCollisionTools = new MOC::CollisionTools(mSceneMgr);

	
	// PRUEBAS:::::


}

// -----------------------------------------------------------------------------
// Set sound
void Zproyect::setupSound(void){
	
	soundMgr = SoundManager::createManager();
	soundMgr->init();

	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		soundMgr->setAudioPath( (char*) "..\\..\\media\\sounds\\" );
	#else
		soundMgr->setAudioPath( (char*) "./media/sounds/" );
	#endif

	// load audio
	soundMgr->loadAudio( "file1.wav", &audioClip1, false);
}

// process sound
bool Zproyect::processSound(const Ogre::FrameEvent& evt){

	static Ogre::Real mToggle = 0.0; // The time left until next toggle
	mToggle -= evt.timeSinceLastFrame;

	if ((mToggle < 0.0f ) && mKeyboard->isKeyDown(OIS::KC_L)){
		mToggle = 0.5;
		soundMgr->playAudio( audioClip1, false );
	}

	return true;
}

//-----------------------------------------------------------------------------------------------
bool Zproyect::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	bool ret = BaseApplication::frameRenderingQueued(evt);

	// ------------------------
	// Camera:
	cameraMan->update(evt);

	// Update Models:
	zombiesMovementModel->preProcess(evt.timeSinceLastFrame);
	enemyAIJustTurn->preProcess(evt.timeSinceLastFrame);
	enemyAIRandom->preProcess(evt.timeSinceLastFrame);
	enemyAIHunt->preProcess(evt.timeSinceLastFrame);

	// ------------------------
	// Zombies:
	for (int i = 0; i < nGroups; i++)
	{	
		zombieGroups[i]->move(zombiesMovementModel);
		
		nameZombies = new std::string*[zombieGroups[i]->nZombies];
		zombieGroups[i]->attack(evt, mCollisionTools, nameZombies);
		for (int j = 0; j < zombieGroups[i]->nZombies; j++)
		{	
			// Dañamos a los enemigos si procede:
			nameGroups = Ogre::StringUtil::split(*nameZombies[j], Ogre::String("."));
			int individual = Ogre::StringConverter::parseInt(nameGroups[1]);
			if (individual != -1)
				enemies[individual]->damage(zombieGroups[i]->getZombie(j)->dps, evt.timeSinceLastFrame);
		}
		zombieGroups[i]->update(evt, mCollisionTools);
	}


	// ------------------------
	// Enemies:
	for (int e = 0; e < nEnemies; e++)
	{
		switch (enemies[e]->type)
		{
	
		case ENEMY_TYPE_TURRET:
			enemyAIJustTurn->makeDecision(mCollisionTools, evt, enemies[e], zombieGroups, nGroups);
	    		break;

		case ENEMY_TYPE_FLYER:
			enemyAIRandom->makeDecision(mCollisionTools, evt, enemies[e], zombieGroups, nGroups);
			break;

		case ENEMY_TYPE_ROBOT:
			enemyAIHunt->makeDecision(mCollisionTools, evt, enemies[e], zombieGroups, nGroups);
			break;
		}
	}
	// UpdateModels:
	zombiesMovementModel->postProcess();
	enemyAIJustTurn->postProcess();
	enemyAIRandom->postProcess();
	enemyAIHunt->postProcess();


	// ---------------------------------------------	
	// --- Sound System -------------
	if(!processSound(evt)) return false;

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
	{
		for(int i=0;i<nGroups;i++) zombieGroups[i]->modifySpeed(1.1);
		for (int e = 0; e < nEnemies; e++) enemies[e]->modifySpeed(1.1);
	}

	if (arg.key == OIS::KC_SUBTRACT)
	{
		for(int i=0;i<nGroups;i++) zombieGroups[i]->modifySpeed(0.9);
		for (int e = 0; e < nEnemies; e++) enemies[e]->modifySpeed(0.9);
	}
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
