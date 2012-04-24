#ifndef __UnitMovModel_h_
#define __UnitMovModel_h_

#include <OGRE/Ogre.h>
#include "Zombie.h"

class UnitMovModel
{
   public:
	virtual bool calculateMove(Zombie **zom, int i, Ogre::Vector3 flag, double* x, double* z)  = 0;
};
//-------------------------------------------------------

class UnitMovModelRandom : public UnitMovModel
{
	private:
		double aux;
	public:
		UnitMovModelRandom();
		bool calculateMove(Zombie **zom, int i, Ogre::Vector3 flag, double* x, double* z);
		void preProcess(double time);
		void postProcess();
};

#endif // #ifndef __UnitMovModel_h_
