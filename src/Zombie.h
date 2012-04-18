#ifndef __Zombie_h_
#define __Zombie_h_

#include <OGRE/Ogre.h>


class Zombie
{
public:
	Zombie(Ogre::String model, Ogre::Real initX, Ogre::Real initZ, Ogre::Real sp);
	virtual ~Zombie(void);

	void move(Ogre::Real axisX, Ogre::Real axisZ);
	void update(const Ogre::FrameEvent& evt);

	void kill(void);			// kill it
	bool isLive(void);

	Ogre::SceneNode* node;		// lo necesito publico para acceder a su posicion y dispararle

protected:

	Ogre::Entity* entity;
	Ogre::Vector3 translateVector;
	Ogre::Real speed;
	bool live;		// live/dead zombie
};

#endif // #ifndef __Zombie_h_
