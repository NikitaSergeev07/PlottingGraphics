// ConsoleApplication4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "SquareFunc.h"
#include "LinearFunc.h"
#include "Solver.h"


int main()
{
  Solver* sf = new SquareFunc(2.0, -1.0, 1.1);
  Solver* lf = new LinearFunc(2.0, 5.0);

  for (int i = -10; i <= 10; i++)
  {
    std::cout << "x = " << i << ", y = " << lf->GetValue(i) << std::endl;
  }
  for (int i = -10; i <= 10; i++)
  {
    std::cout << "x = " << i << ", y = " << sf->GetValue(i) << std::endl;
  }

  delete sf;
  delete lf;
}
