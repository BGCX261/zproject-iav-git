#ifndef __EnemyMovModel_h_
#define __EnemyMovModel_h_

#include <OGRE/Ogre.h>
#include "ZombiePack.h"
#include "Enemy.h"
#include "CollisionTools.h"

class EnemyMovModel
{
   public:
	EnemyMovModel();
	virtual void calculateMove(Enemy *enemy, ZombiePack** zomb, int nZombies, double* x, double* z)  = 0;
};

//-------------------------------------------------------

class EnemyMovModelRandom : public EnemyMovModel
{

	public:
		EnemyMovModelRandom();
		void calculateMove(Enemy *enemy, ZombiePack** zomb, int nZombies, double* x, double* z);
};

//-------------------------------------------------------

class EnemyMovModelChase : public EnemyMovModel
{

	public:
		EnemyMovModelChase();
		void calculateMove(Enemy *enemy, ZombiePack** zomb, int nZombies, double* x, double* z);
};

#endif // #ifndef __EnemyMovModel_h_
