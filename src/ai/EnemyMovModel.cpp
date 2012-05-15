

#include "EnemyMovModel.h"

EnemyMovModel::EnemyMovModel(double r)
{
	aux = 0;
	rate = r;
}


//-------------------------------------------------
void EnemyMovModel::preProcess(double time)
{
	aux += time;
}

//-------------------------------------------------
void EnemyMovModel::postProcess()
{
	if (aux >= rate) {
		aux = 0;
	}
}

//-------------------------------------------------
//-------------------------------------------------
//-------------------------------------------------

EnemyMovModelRandom::EnemyMovModelRandom() : EnemyMovModel(1)
{
	
}


bool EnemyMovModelRandom::calculateMove(Enemy *enemy, ZombiePack** zomb, double* x, double* z)
{
	bool change = false;
	
	if (aux >= rate)
	{
		// Get the center and calculate a point of a circumference:
		Ogre::Vector3 pos = enemy->node->getPosition();
	
		double angle = rand() % 360;
		angle = Ogre::Math::DegreesToRadians(angle);

		*x = pos.x + 5 * Ogre::Math::Cos(angle);
		*z = pos.z + 5 * Ogre::Math::Sin(angle);
		change = true;
	}

	return change;
}

