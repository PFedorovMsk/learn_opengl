#ifndef CURVE3D_H
#define CURVE3D_H

#include <QVector3D>
#include <QVector>
#include <cassert>
#include <cmath>


class Curve3D
{
public:
    Curve3D();

    virtual void append(const QVector3D &point);
    virtual void insert(int index, const QVector3D &point);
    virtual void modify(int index, const QVector3D &newPoint);
    virtual void remove(int index);

    const QVector<QVector3D> basePoints() const;
    const QVector<QVector3D> data() const;
    float                    approximationStep() const;
    int                      order() const;

    void setApproximationStep(float step);
    void setOrder(int order);

    virtual void build() = 0;

protected:
    QVector<QVector3D> m_basePoints;
    QVector<QVector3D> m_data;

    float m_step;
    int   m_order;
};

#endif // CURVE3D_H
