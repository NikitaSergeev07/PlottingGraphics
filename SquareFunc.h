#pragma once

#include "Solver.h"

class SquareFunc : public Solver
{
private:
  double a, b, c;
public:
  SquareFunc(double a, double b, double c);
  double GetValue(double x);
};

