

#include "CameraMan.h"

//-------------------------------------------------------------------------------------
CameraMan::CameraMan(Ogre::SceneNode* node, Ogre::Vector3 tv, Ogre::Real s)
{
	cameraNode = node;
	
	speed = s;
	translateVector = tv;
}

//-------------------------------------------------------------------------------------
CameraMan::~CameraMan(void)
{
}

//-------------------------------------------------------------------------------------
void CameraMan::update(const Ogre::FrameEvent& evt)
{

	cameraNode->translate(translateVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
}

//-------------------------------------------------------------------------------------
void CameraMan::keyPressed( const OIS::KeyEvent &arg )
{
	switch (arg.key)
	{
	case OIS::KC_UP:
	case OIS::KC_W:
	    translateVector.z = -speed;
	    break;

	case OIS::KC_DOWN:
	case OIS::KC_S:
	    translateVector.z = speed;
	    break;

	case OIS::KC_LEFT:
	case OIS::KC_A:
	    translateVector.x = -speed;
	    break;

	case OIS::KC_RIGHT:
	case OIS::KC_D:
	    translateVector.x = speed;
	    break;

	case OIS::KC_PGDOWN:
	case OIS::KC_E:
	    translateVector.y = -speed;
	    break;

	case OIS::KC_PGUP:
	case OIS::KC_Q:
	    translateVector.y = speed;
	    break;
	}
}

//-------------------------------------------------------------------------------------
void  CameraMan::keyReleased( const OIS::KeyEvent &arg )
{
	switch (arg.key)
	{
	case OIS::KC_UP:
	case OIS::KC_W:
	    translateVector.z = 0;
	    break;
	 
	case OIS::KC_DOWN:
	case OIS::KC_S:
	    translateVector.z = 0;
	    break;
	 
	case OIS::KC_LEFT:
	case OIS::KC_A:
	    translateVector.x = 0;
	    break;
	 
	case OIS::KC_RIGHT:
	case OIS::KC_D:
	    translateVector.x = 0;
	    break;
	 
	case OIS::KC_PGDOWN:
	case OIS::KC_E:
	    translateVector.y = 0;
	    break;
	 
	case OIS::KC_PGUP:
	case OIS::KC_Q:
	    translateVector.y = 0;
	    break;
	 
	default:
	    break;
	}
}


