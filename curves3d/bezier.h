#ifndef BEZIER_H
#define BEZIER_H

#include "curve3d.h"


class Bezier : public Curve3D
{
public:
    Bezier();

    void build() override;


private:
    float bernsteinPolynomial(int n, int i, float u);
    int factorial(int x);
};

#endif // BEZIER_H
