

#include "UnitMovModel.h"

UnitMovModelRandom::UnitMovModelRandom()
{
	aux = 0;
}


//-------------------------------------------------
void UnitMovModelRandom::preProcess(double time)
{
	aux += time;
}

//-------------------------------------------------
void UnitMovModelRandom::postProcess()
{
	if (aux >= 1) {
		aux = 0;
	}
}

//-------------------------------------------------
bool UnitMovModelRandom::calculateMove(Zombie **zom, int i, double* x, double* z)
{
	bool change = false;
	
	if (aux >= 1)
	{
		// Get the center a calculate a point of a circumference:
		Ogre::Vector3 pos = zom[i]->node->getPosition();
		
		double angle = rand() % 360;
		angle = Ogre::Math::DegreesToRadians(angle);

		*x = pos.x + 1 * Ogre::Math::Cos(angle);
		*z = pos.z + 1 * Ogre::Math::Sin(angle);

		change = true;
	}

	return change;
}






