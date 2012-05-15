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

	virtual void makeDecision(MOC::CollisionTools *mCollisionTools, const Ogre::FrameEvent& evt, Enemy *enemy, ZombiePack **zombies)  = 0;
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
		void makeDecision(MOC::CollisionTools *mCollisionTools, const Ogre::FrameEvent& evt, Enemy *enemy, ZombiePack **zombies);

	protected:
		bool applied;
};
#endif // #ifndef __EnemyAIModel_h_
