#include <math.h>

float E_Basic(int x)
{
    return pow(1.0 + 1.0 / x, x);
}

float E_Series(int x)
{
    float e = 0.0;
    float fact = 1.0;
    for (int i = 0; i <= x; i++)
    {
        if (i > 0)
            fact *= i;
        e += 1.0 / fact;
    }
    return e;
}
