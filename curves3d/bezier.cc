#include "bezier.h"


Bezier::Bezier()
    : Curve3D()
{
    setOrder(6);
}

void Bezier::build()
{
    int size = int(1.0f / m_step);
    if (1.0f - m_step * size >= 0.5f * m_step) {
        ++size;
    }
    m_step = 1.0f / size;

    m_data.clear();
    m_data.resize(size + 1);

    for (int i = 0; i < size + 1; ++i) {
        float u = i * m_step;
        if (u > 1.0f) {
            u = 1.0f;
        }
        QVector3D tmp(0.0f, 0.0f, 0.0f);
        for (int j = 0; j < m_basePoints.size(); ++j) {
            tmp += (m_basePoints[j] * bernsteinPolynomial(m_order, j, u));
        }
        m_data[i] = tmp;
    }
}

float Bezier::bernsteinPolynomial(int n, int i, float u)
{
    float f = float(factorial(n)) / (float(factorial(i)) * factorial(n - i));
    float g = std::pow(u, float(i)) * std::pow(1.0f - u, float(n - i));
    return f * g;
}

int Bezier::factorial(int x)
{
    int fact = 1;
    for (; x > 0; --x) {
        fact *= x;
    }
    return fact;
}
