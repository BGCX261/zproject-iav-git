

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

EnemyAIModelRandom::EnemyAIModelRandom() : EnemyAIModel(3)
{
	movModel = new EnemyMovModelRandom();
	state = 0;
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
		if (enemy->seek)
		{
			enemy->setPatrol();
			double x, z;
			movModel->calculateMove(enemy, zombies, nZombies, &x, &z);
			enemy->move(x, z);
		} else {
			enemy->setSeek();
		}
	}	

	enemy->update(mCollisionTools, evt, zombies);
}

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------

EnemyAIModelHunt::EnemyAIModelHunt() : EnemyAIModel(0.3)
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

		// Calculamos si está demasiado cerca:
		Ogre::Vector3 myPos = enemy->node->getPosition();
		if (myPos.distance(Ogre::Vector3(x, myPos.y, z)) < enemy->range*0.5)
		{
			x = 2 * myPos.x - x;
			z = 2 * myPos.z - z;
		}

		enemy->move(x, z);
	}

	enemy->update(mCollisionTools, evt, zombies);
}


