#ifndef __UnitMovModel_h_
#define __UnitMovModel_h_

#include <OGRE/Ogre.h>
#include "Zombie.h"

class UnitMovModel
{
   protected:
	double aux;
	double rate;

   public:
	UnitMovModel(double r);
	virtual bool calculateMove(Zombie **zom, int nZom, int i, Ogre::Vector3 flag, double* x, double* z)  = 0;
	void preProcess(double time);
	void postProcess();
};
//-------------------------------------------------------

class UnitMovModelRandom : public UnitMovModel
{

	public:
		UnitMovModelRandom();
		bool calculateMove(Zombie **zom, int nZom, int i, Ogre::Vector3 flag, double* x, double* z);
};

//-------------------------------------------------------

class UnitMovModelRBSFlock : public UnitMovModel
{

	public:
		UnitMovModelRBSFlock();
		bool calculateMove(Zombie **zom, int nZom, int i, Ogre::Vector3 flag, double* x, double* z);
		void preProcess(double time);
		void postProcess();
};

#endif // #ifndef __UnitMovModel_h_
