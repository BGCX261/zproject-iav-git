

#include "EnemyAIModel.h"


EnemyAIModel::EnemyAIModel(double r)
{
	rate = r;
	aux = 0;
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

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------

EnemyAIModelJustTurn::EnemyAIModelJustTurn() : EnemyAIModel(1)
{

}

void EnemyAIModelJustTurn::start(Enemy* enemy)
{
	enemy->setAttack(true);
	enemy->setSeek();
}

void EnemyAIModelJustTurn::makeDecision(MOC::CollisionTools *mCollisionTools, const Ogre::FrameEvent& evt, Enemy *enemy, ZombiePack **zombies, int nZombies)
{
	enemy->update(mCollisionTools, evt, zombies);
}

//-------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------

EnemyAIModelRandom::EnemyAIModelRandom() : EnemyAIModel(1.5)
{
	movModel = new EnemyMovModelRandom();
}

void EnemyAIModelRandom::start(Enemy* enemy)
{
	enemy->setAttack(true);
	enemy->setPatrol();
}

void EnemyAIModelRandom::makeDecision(MOC::CollisionTools *mCollisionTools, const Ogre::FrameEvent& evt, Enemy *enemy, ZombiePack **zombies, int nZombies)
{
	if (aux >= rate)
	{
		double x, z;
		movModel->calculateMove(enemy, zombies, nZombies, &x, &z);
		enemy->move(x, z);
	}

	enemy->update(mCollisionTools, evt, zombies);
}

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------

EnemyAIModelHunt::EnemyAIModelHunt() : EnemyAIModel(1.5)
{
	movModel = new EnemyMovModelChase();
}

void EnemyAIModelHunt::start(Enemy* enemy)
{
	enemy->setAttack(true);
	enemy->setPatrol();
}

void EnemyAIModelHunt::makeDecision(MOC::CollisionTools *mCollisionTools, const Ogre::FrameEvent& evt, Enemy *enemy, ZombiePack **zombies, int nZombies)
{
	if (aux >= rate)
	{
		double x, z;
		movModel->calculateMove(enemy, zombies, nZombies, &x, &z);
		enemy->move(x, z);
	}

	enemy->update(mCollisionTools, evt, zombies);
}


