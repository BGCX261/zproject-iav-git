#ifndef __Zombie_h_
#define __Zombie_h_

#include <OGRE/Ogre.h>

#include "BaseApplication.h"

class Zombie
{
public:
	Zombie(Ogre::String model, 	int gr, int ind, Ogre::Real initX, Ogre::Real initZ, Ogre::Real sp, Ogre::Real sptr);
	virtual ~Zombie(void);

	void move(Ogre::Real axisX, Ogre::Real axisZ);
	void update(const Ogre::FrameEvent& evt);

	void damage(double deltaT);
	bool isLive(void);

	Ogre::SceneNode* node;		// lo necesito publico para acceder a su posicion y dispararle


	double hunger;
	Ogre::Vector3 headingTo;

	// cambiar velocidad inGame
	Ogre::Real speed;

protected:

	// Graphics:
	Ogre::Entity* entity;
	Ogre::AnimationState *anim_walk;

	Ogre::Entity* lifebar;
	Ogre::SceneNode *lifebarNode;

	// Movement:
	Ogre::Vector3 translateVector;
	Ogre::Radian angleTurn;
	bool turning;
	Ogre::Real speedTurn;

	// Atributes:
	int group;
	int individual;

	double life;
	bool live;		// live/dead zombie
};

#endif // #ifndef __Zombie_h_
