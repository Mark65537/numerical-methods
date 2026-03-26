#pragma once

#include "Library.h"

#define SIGNUM(Value) ((Value) < 0 ? (-1) : !!(Value))

const long double EPS = 1E-15;
const long double PI = 3.141592653589793238462643;

long double First_Funct(long double& x);
long double Second_Funct(long double& x);
void Lagrange(int q);
void Main_Spline(int q);
