#include "nurbs.h"

Nurbs::Nurbs()
    : BSpline()
{
    setOrder(4);
    m_weights.clear();
}

void Nurbs::append(const QVector3D &point)
{
    BSpline::append(point);
    m_weights.append(1.0f);
}

void Nurbs::insert(int index, const QVector3D &point)
{
    BSpline::insert(index, point);
    m_weights.insert(index, 1.0f);
}

void Nurbs::remove(int index)
{
    BSpline::remove(index);
    m_weights.remove(index);
}

void Nurbs::modifyWeight(int index, float weight)
{
    assert(index >= 0);
    assert(index < m_weights.size());

    m_weights[index] = weight;
}

const QVector<float> &Nurbs::weights() const
{
    return m_weights;
}

void Nurbs::build()
{
    float max  = float(m_basePoints.size() - m_order + 2);
    int   size = int(max / m_step);
    if (max - m_step * size >= 0.5f * m_step) {
        ++size;
    }
    m_step = max / size;

    m_data.clear();
    m_data.resize(size + 1);

    calculateKnots();

    for (int i = 0; i < size + 1; ++i) {
        QVector3D tmp(0.0f, 0.0f, 0.0f);
        float     sum = 0.0f;
        float     u   = m_step * i;
        if (u > max) {
            u = max;
        }
        for (int j = 0; j < m_basePoints.size(); ++j) {
            float wn = m_weights[j] * calculateN(j, m_order, u);
            tmp += m_basePoints[j] * wn;
            sum += wn;
        }
        m_data[i] = tmp / sum; // WARNING if (sum == 0.0f) ???
    }
}
