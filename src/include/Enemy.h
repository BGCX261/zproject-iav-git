#ifndef __Enemy_h_
#define __Enemy_h_

#include <OGRE/Ogre.h>
#include "CollisionTools.h"
#include "ZombiePack.h"
#include "Zombie.h"

#define ENEMY_TYPE_TURRET  0
#define ENEMY_TYPE_FLYER   1
#define ENEMY_TYPE_ROBOT   2

struct EnemyDataStructure {
	Ogre::String model;
	int dps;
	int range;
	int life;
	int speed;
	int speedTurn;
	
	double scaleFactor;
};

static struct EnemyDataStructure cosa = {"Robot.mesh"};

class Enemy
{

static const struct EnemyDataStructure EnemyData[3];

public:
	Enemy(int ty, int ind, Ogre::Real initX, Ogre::Real initZ);
	virtual ~Enemy(void);

	void move(Ogre::Real axisX, Ogre::Real axisZ);
	void trace(MOC::CollisionTools *mCollisionTools, const Ogre::FrameEvent& evt, ZombiePack** zombies);
	void update(MOC::CollisionTools *mCollisionTools, const Ogre::FrameEvent& evt, ZombiePack** zombies);

	void damage(int dps,  double deltaT);
	bool isAlive();
	void setPatrol();
	void setSeek();
	void setAttack(bool);

	void modifySpeed(double factor);

	Ogre::SceneNode* node;		// lo necesito publico para acceder a su posicion y dispararle
	Ogre::AxisAlignedBox box;
	
	bool patrol, seek, shoot;
	bool attack;

	int type;
	int index;

protected:

	void fire();

	Ogre::Entity* entity;
	Ogre::AnimationState *anim_walk, *anim_shoot, *anim_idle;

	Ogre::Vector3 translateVector;
	bool turning;
	Ogre::Radian angleTurn;

	Ogre::Real speed;
	Ogre::Real speedTurn;

	double shootingDelay;

	int range;
	int dps;

	bool alive;		// live/dead
	double life;

        Ogre::SceneNode* smokeNode;
        Ogre::ParticleSystem* smokeParticles;
};

#endif // #ifndef __Enemy_h_
