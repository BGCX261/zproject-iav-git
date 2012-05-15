

#include "EnemyMovModel.h"

EnemyMovModel::EnemyMovModel()
{
	
}

//-------------------------------------------------
//-------------------------------------------------
//-------------------------------------------------

EnemyMovModelRandom::EnemyMovModelRandom() : EnemyMovModel()
{
	
}


void EnemyMovModelRandom::calculateMove(Enemy *enemy, ZombiePack** zomb, int nZombies, double* x, double* z)
{
	// Get the center and calculate a point of a circumference:
	Ogre::Vector3 pos = enemy->node->getPosition();

	double angle = rand() % 360;
	angle = Ogre::Math::DegreesToRadians(angle);

	*x = pos.x + 5 * Ogre::Math::Cos(angle);
	*z = pos.z + 5 * Ogre::Math::Sin(angle);
}


//-------------------------------------------------
//-------------------------------------------------
//-------------------------------------------------

EnemyMovModelChase::EnemyMovModelChase() : EnemyMovModel()
{
	
}


void EnemyMovModelChase::calculateMove(Enemy *enemy, ZombiePack** zomb, int nZombies, double* x, double* z)
{

	Ogre::Vector3 myPos = enemy->node->getPosition();

	double maxDist = 999999999;
	int bestGroup  = -1;
	int bestZombie = -1;

	for (int i = 0; i < nZombies; i++)
	{
		for (int j = 0; j < zomb[i]->nZombies; j++)
		{
			if (zomb[i]->getZombie(j)->isAlive()) {
				
				double dist = myPos.distance(zomb[i]->getZombie(j)->node->getPosition());
				if (dist < maxDist)
				{
					bestGroup = i;
					bestZombie = j;
					maxDist = dist;
				}
			}
		}		
	}

	if (bestGroup == -1)
	{
		*x = myPos.x;
		*z = myPos.z;
		return;
	}

	*x = zomb[bestGroup]->getZombie(bestZombie)->node->getPosition().x;
	*z = zomb[bestGroup]->getZombie(bestZombie)->node->getPosition().z;
}


