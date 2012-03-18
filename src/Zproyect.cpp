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
	cameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("cameraNode", Ogre::Vector3(0, 100, 100));
	cameraNode->attachObject(mCamera);

	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);
	mDirection = Ogre::Vector3::ZERO;
	mMove = 200;

	Ogre::Entity* terrain = mSceneMgr->createEntity("Terrain", "Plane.mesh");

	Ogre::SceneNode* terrainNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	terrainNode->attachObject(terrain);
	terrainNode->scale(Ogre::Vector3(10,10,10));

	// Set ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	// Create a light
	Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setPosition(20,80,50);
}

bool Zproyect::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	bool ret = BaseApplication::frameRenderingQueued(evt);

	cameraNode->translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);

	return ret;
}

bool Zproyect::keyPressed( const OIS::KeyEvent &arg )
{
	bool ret = BaseApplication::keyPressed(arg);

	switch (arg.key)
    	{
	case OIS::KC_UP:
	case OIS::KC_W:
	    mDirection.z = -mMove;
	    break;
 
	case OIS::KC_DOWN:
	case OIS::KC_S:
	    mDirection.z = mMove;
	    break;
 
	case OIS::KC_LEFT:
	case OIS::KC_A:
	    mDirection.x = -mMove;
	    break;
 
	case OIS::KC_RIGHT:
	case OIS::KC_D:
	    mDirection.x = mMove;
	    break;
 
	case OIS::KC_PGDOWN:
	case OIS::KC_E:
	    mDirection.y = -mMove;
	    break;
 
	case OIS::KC_PGUP:
	case OIS::KC_Q:
	    mDirection.y = mMove;
	    break;
    }
	return ret;
}

bool Zproyect::keyReleased( const OIS::KeyEvent &arg )
{
	bool ret = BaseApplication::keyReleased(arg);

	switch (arg.key)
	{
	case OIS::KC_UP:
	case OIS::KC_W:
	    mDirection.z = 0;
	    break;
	 
	case OIS::KC_DOWN:
	case OIS::KC_S:
	    mDirection.z = 0;
	    break;
	 
	case OIS::KC_LEFT:
	case OIS::KC_A:
	    mDirection.x = 0;
	    break;
	 
	case OIS::KC_RIGHT:
	case OIS::KC_D:
	    mDirection.x = 0;
	    break;
	 
	case OIS::KC_PGDOWN:
	case OIS::KC_E:
	    mDirection.y = 0;
	    break;
	 
	case OIS::KC_PGUP:
	case OIS::KC_Q:
	    mDirection.y = 0;
	    break;
	 
	default:
	    break;
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
