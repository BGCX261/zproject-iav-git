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


	// Create the camera for map navigation:
	cameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("cameraNode", Ogre::Vector3(0, 20, 30));
	cameraNode->attachObject(mCamera);
	cameraMan = new CameraMan(cameraNode, Ogre::Vector3::ZERO ,10);

	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);

	/*
	Ogre::Entity* terrain = mSceneMgr->createEntity("Terrain", "Plane.mesh");
	Ogre::SceneNode* terrainNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	terrainNode->attachObject(terrain);
	terrainNode->scale(Ogre::Vector3(10,10,10));*/
	
	// Create a plane for terrain (with texture)
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        	plane, 100, 100, 1, 1, true, 1, 4, 4, Ogre::Vector3::UNIT_Z);

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

	zombies = new Zombie*[10];
	for (int i = 0; i < 10; i++) {
		zombies[i] = new Zombie(Ogre::String("Cube.001.mesh"), i, 0, 4);	
	}
	zombiesMovementModel = new UnitMovModelRandom(5124);

}

bool Zproyect::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	bool ret = BaseApplication::frameRenderingQueued(evt);

	zombiesMovementModel->preProcess(evt.timeSinceLastFrame);

	cameraMan->update(evt);

	// We now move all the units, according to the movement model:
	for (int i = 0; i < 10; i++) {

		double x, z;
		if (zombiesMovementModel->calculateMove(&x, &z))
			zombies[i]->move(x, z);

		zombies[i]->update(evt);
	}


	zombiesMovementModel->postProcess();

	return ret;
}

bool Zproyect::keyPressed( const OIS::KeyEvent &arg )
{
	bool ret = BaseApplication::keyPressed(arg);

	cameraMan->keyPressed(arg);

	return ret;
}

bool Zproyect::keyReleased( const OIS::KeyEvent &arg )
{
	bool ret = BaseApplication::keyReleased(arg);

	cameraMan->keyReleased(arg);

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
