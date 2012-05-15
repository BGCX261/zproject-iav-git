#ifndef __EnemyAIModel_h_
#define __EnemyAIModel_h_

#include <OGRE/Ogre.h>
#include "ZombiePack.h"
#include "Enemy.h"
#include "EnemyMovModel.h"
#include "CollisionTools.h"

class EnemyAIModel
{
   public:
	EnemyAIModel(double r);
	~EnemyAIModel();

	virtual void makeDecision(MOC::CollisionTools *mCollisionTools, const Ogre::FrameEvent& evt, Enemy *enemy, ZombiePack **zombies, int nZombies)  = 0;
	virtual void start(Enemy* enemy) = 0;
	void preProcess(double time);
	void postProcess();

   protected:
	double rate;
	double aux;
};


class EnemyAIModelJustTurn : public EnemyAIModel
{
	public:
		EnemyAIModelJustTurn();
		~EnemyAIModelJustTurn();
		void start(Enemy* enemy);
		void makeDecision(MOC::CollisionTools *mCollisionTools, const Ogre::FrameEvent& evt, Enemy *enemy, ZombiePack **zombies, int nZombies);
};

class EnemyAIModelRandom : public EnemyAIModel
{
	public:
		EnemyAIModelRandom();
		~EnemyAIModelRandom();
		void start(Enemy* enemy);
		void makeDecision(MOC::CollisionTools *mCollisionTools, const Ogre::FrameEvent& evt, Enemy *enemy, ZombiePack **zombies, int nZombies);

	protected:
		EnemyMovModelRandom *movModel;
};

class EnemyAIModelHunt : public EnemyAIModel
{
	public:
		EnemyAIModelHunt();
		~EnemyAIModelHunt();
		void start(Enemy* enemy);
		void makeDecision(MOC::CollisionTools *mCollisionTools, const Ogre::FrameEvent& evt, Enemy *enemy, ZombiePack **zombies, int nZombies);

	protected:
		EnemyMovModelChase *movModel;
};
#endif // #ifndef __EnemyAIModel_h_
