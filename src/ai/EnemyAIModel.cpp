

#include "EnemyAIModel.h"


EnemyAIModel::EnemyAIModel(double r)
{
	aux = 0;
	rate = r;
}


//-------------------------------------------------
EnemyAIModel::~EnemyAIModel()
{

}

//-------------------------------------------------
void EnemyAIModel::preProcess(double time)
{
	aux += time;
}

//-------------------------------------------------
void EnemyAIModel::postProcess()
{
	if (aux >= rate) {
		aux = 0;
	}
}

//-------------------------------------------------------------

EnemyAIModelJustTurn::EnemyAIModelJustTurn() : EnemyAIModel(1)
{
	applied = false;
}

void EnemyAIModelJustTurn::makeDecision(MOC::CollisionTools *mCollisionTools, const Ogre::FrameEvent& evt, Enemy *enemy, ZombiePack **zombies)
{
	if (!applied)
	{
		enemy->setAttack(true);
		enemy->setSeek();
		applied = true;
	}

	enemy->update(mCollisionTools, evt, zombies);
}

//-------------------------------------------------------------


EnemyAIModelRandom::EnemyAIModelRandom() : EnemyAIModel(1.5)
{
	movModel = new EnemyMovModelRandom();
	applied = false;
}

void EnemyAIModelRandom::makeDecision(MOC::CollisionTools *mCollisionTools, const Ogre::FrameEvent& evt, Enemy *enemy, ZombiePack **zombies)
{
	if (!applied)
	{
		enemy->setAttack(true);
		enemy->setPatrol();
		applied = true;
	}
	if (aux >= rate)
	{
		double x, z;
		movModel->calculateMove(enemy, zombies, &x, &z);
		enemy->move(x, z);
	}

	enemy->update(mCollisionTools, evt, zombies);
}

