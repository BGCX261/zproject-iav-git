#ifndef __UnitMovModel_h_
#define __UnitMovModel_h_

#include <OGRE/Ogre.h>

class UnitMovModel
{
   public:
	virtual bool calculateMove(double* x, double* z)  = 0;
};
//-------------------------------------------------------

class UnitMovModelRandom : public UnitMovModel
{
	private:
		double aux;
	public:
		UnitMovModelRandom(int seed);
		bool calculateMove(double* x, double* z);
		void preProcess(double time);
		void postProcess();
};

#endif // #ifndef __UnitMovModel_h_
