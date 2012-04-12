

#include "UnitMovModel.h"

UnitMovModelRandom::UnitMovModelRandom(int seed)
{
	srand(seed);
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
bool UnitMovModelRandom::calculateMove(double* x, double* z)
{
	bool change = false;

	if (aux >= 1)
	{
		*x = rand() % 3 - 1;
		*z = rand() % 3 - 1;

		change = true;
	}

	return change;
}






