#ifndef __CameraMan_h_
#define __CameraMan_h_

#include <OGRE/Ogre.h>
#include <OIS.h>

class CameraMan
{
public:
	CameraMan(Ogre::SceneNode* node, Ogre::Vector3 tv, Ogre::Real s);
	virtual ~CameraMan(void);

	void keyPressed( const OIS::KeyEvent &arg );
	void keyReleased( const OIS::KeyEvent &arg );
	void update(const Ogre::FrameEvent& evt);


protected:
	Ogre::SceneNode* cameraNode;

	Ogre::Vector3 translateVector;
	Ogre::Real speed;
};

#endif // #ifndef __CameraMan_h_
