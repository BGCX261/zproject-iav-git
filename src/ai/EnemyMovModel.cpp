

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


void EnemyMovModelRandom::calculateMove(Enemy *enemy, ZombiePack** zomb, double* x, double* z)
{
	// Get the center and calculate a point of a circumference:
	Ogre::Vector3 pos = enemy->node->getPosition();

	double angle = rand() % 360;
	angle = Ogre::Math::DegreesToRadians(angle);

	*x = pos.x + 5 * Ogre::Math::Cos(angle);
	*z = pos.z + 5 * Ogre::Math::Sin(angle);
}

