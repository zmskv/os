#include <math.h>

float Pi_Leibniz(int K)
{
    float pi = 0.0;
    for (int i = 0; i < K; i++)
    {
        pi += pow(-1, i) / (2 * i + 1);
    }
    return pi * 4;
}

float Pi_Wallis(int K)
{
    float pi = 1.0;
    for (int i = 1; i <= K; i++)
    {
        pi *= (2.0 * i / (2.0 * i - 1)) * (2.0 * i / (2.0 * i + 1));
    }
    return pi * 2;
}
