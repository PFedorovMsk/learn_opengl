#ifndef BSPLINE_H
#define BSPLINE_H

#include "curve3d.h"


class BSpline : public Curve3D
{
public:
    BSpline();

    void build() override;


protected:
    void  calculateKnots();
    float calculateN(int i, int n, float u);


protected:
    QVector<float> m_knots;
};

#endif // BSPLINE_H
