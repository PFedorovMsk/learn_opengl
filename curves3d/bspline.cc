#include "bspline.h"


BSpline::BSpline()
    : Curve3D()
{
    setOrder(4);
    m_knots.clear();
}

void BSpline::build()
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
            float n = calculateN(j, m_order, u);
            tmp += m_basePoints[j] * n;
            sum += n;
        }
        m_data[i] = tmp / sum; // WARNING if (sum == 0.0f) ???
    }
}

void BSpline::calculateKnots()
{
    int k = m_order;
    int n = m_basePoints.size();
    m_knots.clear();
    m_knots.resize(k + n);

    for (int i = 0; i < k + n; ++i) {
        if (i < k) {
            m_knots[i] = 0.0f;
        } else if (k <= i && i < n) {
            m_knots[i] = float(i - k + 1);
        } else {
            m_knots[i] = float(n - k + 2);
        }
    }
}

float BSpline::calculateN(int i, int n, float u)
{
    assert(i >= 0);
    assert(i < m_knots.size() - 1);
    assert(n >= 0);
    assert(i + n < m_knots.size());

    if (n == 1) {
        if (m_knots[i] <= u && u <= m_knots[i + 1]) {
            return 1;
        }
        return 0;
    }

    float a = m_knots[i + n] - m_knots[i];
    if (std::fabs(a) > std::numeric_limits<float>::epsilon()) {
        float A = calculateN(i, n - 1, u) * (u - m_knots[i]) / a;
        float B = calculateN(i + 1, n - 1, u) * (m_knots[i + n] - u) / a;
        return A + B;
    }
    return 0.0f;
}
