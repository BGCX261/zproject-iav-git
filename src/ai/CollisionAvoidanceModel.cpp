

#include "UnitMovModel.h"

UnitMovModel::UnitMovModel(double r)
{
	aux = 0;
	rate = r;
}


//-------------------------------------------------
void UnitMovModel::preProcess(double time)
{
	aux += time;
}

//-------------------------------------------------
void UnitMovModel::postProcess()
{
	if (aux >= rate) {
		aux = 0;
	}
}

//-------------------------------------------------
//-------------------------------------------------
//-------------------------------------------------

UnitMovModelRandom::UnitMovModelRandom() : UnitMovModel(1)
{
	
}


bool UnitMovModelRandom::calculateMove(Zombie **zom,  int nZom, int i, Ogre::Vector3 flag, double* x, double* z)
{
	bool change = false;
	
	if (aux >= rate)
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


//-------------------------------------------------
//-------------------------------------------------
//-------------------------------------------------

UnitMovModelRBSFlock::UnitMovModelRBSFlock(double max, double min) : UnitMovModel(0.5)
{
	maxDist = max;
	minDist = min;
}


bool UnitMovModelRBSFlock::calculateMove(Zombie **zom,  int nZom, int i, Ogre::Vector3 flag, double* x, double* z)
{
	bool change = false;
	
	if (aux >= rate)
	{
		double starvation = (float)rand()/(float)RAND_MAX;
		
		// If the z cant overcome the hunger will apply the rules:
		if (starvation < zom[i]->hunger)
		{
			double myX = zom[i]->node->getPosition().x;
			double myZ = zom[i]->node->getPosition().z;
			
			// Calculate the nearest zombie:
			int nearest = -1;
			double nearestDist = 99999999;
			for (int j = 0; j < nZom; j++)
			{
				if (i == j || !zom[j]->isAlive()) {
					continue;
				}

				double hisX =  zom[j]->node->getPosition().x;
				double hisZ =  zom[j]->node->getPosition().z;
				double d = Ogre::Math::Sqrt( (myX-hisX)*(myX-hisX) + (myZ-hisZ)*(myZ-hisZ));

				if (d < nearestDist) {
					nearest = j;
					nearestDist = d;
				}
			}

			// Apply the rule set:
			// If you are the only one, move randomly:		
			if (nearest == -1) {
				Ogre::Vector3 pos = zom[i]->node->getPosition();
		
				double angle = rand() % 360;
				angle = Ogre::Math::DegreesToRadians(angle);

				*x = pos.x + 5 * Ogre::Math::Cos(angle);
				*z = pos.z + 5 * Ogre::Math::Sin(angle);

			// If you are far away, move closer to the nearest:
			} else if (nearestDist > maxDist) {
				*x = zom[nearest]->node->getPosition().x;								
				*z = zom[nearest]->node->getPosition().z;

			// If you are about to crash move away:
			} else if (nearestDist < minDist) {
				*x = 2 * myX - zom[nearest]->node->getPosition().x;								
				*z = 2 * myZ - zom[nearest]->node->getPosition().z;								

			// Otherwise fly in the same direction:
			} else {
				*x = zom[nearest]->headingTo.x;
				*z = zom[nearest]->headingTo.z;
			}
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



