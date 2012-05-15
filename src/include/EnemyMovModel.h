#ifndef __EnemyMovModel_h_
#define __EnemyMovModel_h_

#include <OGRE/Ogre.h>
#include "ZombiePack.h"
#include "Enemy.h"
#include "CollisionTools.h"

class EnemyMovModel
{
   protected:
	double aux;
	double rate;

   public:
	EnemyMovModel(double r);
	virtual bool calculateMove(Enemy *enemy, ZombiePack** zomb, double* x, double* z)  = 0;
	void preProcess(double time);
	void postProcess();
};
//-------------------------------------------------------

class EnemyMovModelRandom : public EnemyMovModel
{

	public:
		EnemyMovModelRandom();
		bool calculateMove(Enemy *enemy, ZombiePack** zomb, double* x, double* z);
};

#endif // #ifndef __EnemyMovModel_h_
