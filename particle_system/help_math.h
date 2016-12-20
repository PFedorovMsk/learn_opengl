#ifndef HELP_MATH_H
#define HELP_MATH_H

#include <cmath>
#include <cstdlib>
#include <ctime>


inline float mix(float x, float y, float a)
{
    return x * (1.0f - a) + y * a;
}

inline float randFloat()
{
    return (float(rand()) / float(RAND_MAX));
}

inline float pi()
{
    static float x = float(M_PI);
    return x;
}

inline float twoPi()
{
    static float x = 2.0f * float(M_PI);
    return x;
}


#endif // HELP_MATH_H
