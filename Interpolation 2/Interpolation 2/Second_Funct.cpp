#include "Functions.h"

//вычисление значение функции
//!!!НУЖНО СВОЮ ПОДСТАВИТЬ!!!!!!!!!!!!!!!!!


long double Second_Funct(long double& x)
{
	return exp(sin(x)*sin(x))*sin(3*x);
}

long double Second_Funct2(long double& x)
{

	long double ch;
	long double zn;

	ch = sin(2 * x) + pow(sin(3 * x), 2);
	zn = 3 + sin(x) + cos(2 * x);
	return ch / zn;

}