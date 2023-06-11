#include <iostream>
#include "SquareFunc.h"
#include "LinearFunc.h"
#include "Solver.h"
#include "SolverTest.h"

QVector<Solver*> getGraphics()
{
    Solver* sf = new SquareFunc(2.0, -1.0, 1.1);
    Solver* lf = new LinearFunc(2.0, 5.0);
    Solver* pf = new SquareFunc(1.0, 0, 0);
    QVector<Solver*> graphics;
    graphics.push_back(sf);
    graphics.push_back(lf);
    return graphics;
}
