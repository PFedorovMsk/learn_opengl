#include "curve3d.h"


Curve3D::Curve3D()
    : m_step(0.01f)
    , m_order(3)
{
    m_basePoints.clear();
    m_data.clear();
}

void Curve3D::append(const QVector3D &point)
{
    m_basePoints.append(point);
}

void Curve3D::insert(int index, const QVector3D &point)
{
    assert(index >= 0);
    assert(index < m_basePoints.size());

    m_basePoints.insert(index, point);
}

void Curve3D::modify(int index, const QVector3D &newPoint)
{
    assert(index >= 0);
    assert(index < m_basePoints.size());

    m_basePoints[index] = newPoint;
}

void Curve3D::remove(int index)
{
    assert(index >= 0);
    assert(index < m_basePoints.size());

    m_basePoints.remove(index);
}

const QVector<QVector3D> Curve3D::basePoints() const
{
    return m_basePoints;
}

const QVector<QVector3D> Curve3D::data() const
{
    return m_data;
}

float Curve3D::approximationStep() const
{
    return m_step;
}

int Curve3D::order() const
{
    return m_order;
}

void Curve3D::setApproximationStep(float step)
{
    assert(step > 0.0f);

    m_step = step;
}

void Curve3D::setOrder(int order)
{
    assert(order > 0);

    m_order = order;
}
