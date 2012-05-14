#ifndef __ZOMBIE_H_
#define __ZOMBIE_H_

#include <OGRE/Ogre.h>
#include "CollisionTools.h"


//#include "Enemy.h"


class Zombie
{
public:
	Zombie(Ogre::String model, int gr, int ind, Ogre::Real initX, Ogre::Real initZ, Ogre::Real sp, Ogre::Real sptr);
	virtual ~Zombie(void);

	void move(Ogre::Real axisX, Ogre::Real axisZ);
	void attack(const Ogre::FrameEvent& evt, MOC::CollisionTools *mCollisionTools, Ogre::String* &nombre);
	void update(const Ogre::FrameEvent& evt, MOC::CollisionTools *mCollisionTools);

	void damage(int dps, double deltaT);
	bool isAlive();
	void deselect();
	void select();

	Ogre::SceneNode* node;		// lo necesito publico para acceder a su posicion y dispararle


	double hunger;
	Ogre::Vector3 headingTo;

	// cambiar velocidad inGame
	Ogre::Real speed;
	int dps;

protected:

	// Graphics:
	Ogre::Entity* entity;
	Ogre::AnimationState *anim_walk;
	Ogre::AnimationState *anim_death;
	Ogre::AnimationState *anim_attack;

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
	bool alive;

	// State:
	bool attacking;
};

#endif // #ifndef __Zombie_h_
