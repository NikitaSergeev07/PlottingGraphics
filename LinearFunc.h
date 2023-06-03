#pragma once
#include "Solver.h"

class LinearFunc : public Solver
{
private:
  double k, b;
public:
  LinearFunc(double k, double b);
  double GetValue(double x);
};

