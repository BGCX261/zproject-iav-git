#ifndef __ZombiePack_h_
#define __ZombiePack_h_

#include <OGRE/Ogre.h>
#include <vector>

#include "CollisionTools.h"
#include "Zombie.h"
#include "UnitMovModel.h"

class ZombiePack
{
public:
	ZombiePack(int id, int nz, Ogre::Real initX, Ogre::Real initZ, Ogre::Real sp, Ogre::Real sptr);
	virtual ~ZombiePack(void);

	Zombie* getZombie(int i);

	void move(UnitMovModel *model);
	void attack(const Ogre::FrameEvent& evt, MOC::CollisionTools *mCollisionTools, Ogre::String** &nombre);
	void update(const Ogre::FrameEvent& evt, MOC::CollisionTools *mCollisionTools);
	void modifySpeed(float factor);
	void setBandera(double x, double z);
	void select();
	void deselect();

	Zombie **zombies;

	int identifier;
	

protected:

	int nZombies;

	Ogre::SceneNode* banderaNode;
};

#endif // #ifndef __ZombiePack_h_
