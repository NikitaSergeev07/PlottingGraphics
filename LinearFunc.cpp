#include "LinearFunc.h"

LinearFunc::LinearFunc(double k, double b)
{
  this->k = k;
  this->b = b;
}

double LinearFunc::GetValue(double x)
{
  return k * x + b;
}
