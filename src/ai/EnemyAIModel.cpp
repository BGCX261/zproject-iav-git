

#include "EnemyAIModel.h"


EnemyAIModel::EnemyAIModel(double r)
{
	aux = 0;
	rate = r;
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

