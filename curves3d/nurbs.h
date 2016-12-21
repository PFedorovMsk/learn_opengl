#ifndef NURBS_H
#define NURBS_H

#include "bspline.h"


class Nurbs : public BSpline
{
public:
    Nurbs();

    void append(const QVector3D &point) override;
    void insert(int index, const QVector3D &point) override;
    void remove(int index) override;

    void modifyWeight(int index, float weight);

    const QVector<float> &weights() const;

    void build() override;


private:
    QVector<float> m_weights;
};

#endif // NURBS_H
