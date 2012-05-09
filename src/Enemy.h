#ifndef __Enemy_h_
#define __Enemy_h_

#include <OGRE/Ogre.h>
#include "CollisionTools.h"
#include "ZombiePack.h"

#ifndef __ZOMBIE_H_
#define __ZOMBIE_H_
#include "Zombie.h"
#endif

class Enemy
{
public:
	Enemy(Ogre::String model, Ogre::Real initX, Ogre::Real initZ, Ogre::Real sp, Ogre::Real sptr);
	virtual ~Enemy(void);

	void move(Ogre::Real axisX, Ogre::Real axisZ);
	void trace(MOC::CollisionTools *mCollisionTools, const Ogre::FrameEvent& evt, ZombiePack** zombies);
	void update(const Ogre::FrameEvent& evt);

	void damage(int dps,  double deltaT);
	bool isAlive();
	void fire();

	Ogre::SceneNode* node;		// lo necesito publico para acceder a su posicion y dispararle
	Ogre::AxisAlignedBox box;

	Ogre::Vector3 headingTo;
	
	bool patrol, seek, shoot;

protected:

	Ogre::Entity* entity;
	Ogre::AnimationState *anim_walk;

	Ogre::Vector3 translateVector;

	Ogre::Radian angleTurn;

	Ogre::Real speed;
	Ogre::Real speedTurn;

	
	int range;
	int dps;

	bool alive;		// live/dead
	int life;

	// Animations:
	Ogre::AnimationState *robotAnimState_idle, *robotAnimState_shoot;
	bool playRobotShoot, playRobotIdle;
};

#endif // #ifndef __Enemy_h_
