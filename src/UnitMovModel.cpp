

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
bool UnitMovModelRandom::calculateMove(Zombie **zom, int i, Ogre::Vector3 flag, double* x, double* z)
{
	bool change = false;
	
	if (aux >= 1)
	{
		double starvation = (float)rand()/(float)RAND_MAX;
		
		// If the z cant overcome the hunger will wander randomly:
		if (starvation < zom[i]->hunger)
		{
			// Get the center and calculate a point of a circumference:
			Ogre::Vector3 pos = zom[i]->node->getPosition();
		
			double angle = rand() % 360;
			angle = Ogre::Math::DegreesToRadians(angle);

			*x = pos.x + 5 * Ogre::Math::Cos(angle);
			*z = pos.z + 5 * Ogre::Math::Sin(angle);
		}
		// Other case, it will chase the flag.
		else
		{
			*x = flag.x;
			*z = flag.z;
		}

		change = true;
	}

	return change;
}






