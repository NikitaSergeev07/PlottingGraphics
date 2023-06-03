#include "SquareFunc.h"

SquareFunc::SquareFunc(double a, double b, double c)
{
  this->a = a;
  this->b = b;
  this->c = c;
}

double SquareFunc::GetValue(double x)
{
  return a * x * x + b * x + c;
}