#include <stdio.h>
#include <stdint.h>



// 最小二乘法
/********************************
 * x=adc   y=mv
 * k = ∑(x-xi)*(y-yi)/∑(x-xi)²    b =yi - k*xi
 * *****************************/
float* Least_Squares_Method(float x[], float y[], uint8_t cnt)
{
    static float Beta[2] = {0};
    float xi = 0;
    float yi = 0;
    int i = 0;
    for (; i < cnt; i++)
    {
        yi += y[i];
    }
    yi = yi / cnt;
    for (i = 0; i < cnt; i++)
    {
        xi += x[i];
    }
    xi = xi / cnt;
    float denominator = 0;
    float molecular = 0;
    for (i = 0; i < cnt; i++)
    {
        molecular += (x[i] - xi) * (y[i] - yi);
        denominator += (x[i] - xi) * (x[i] - xi);
    }
    if (denominator != 0)
    {
        Beta[0] = molecular / denominator;
    }
    else
    {
        Beta[0] = denominator;
    }

    Beta[1] = yi - Beta[0] * xi;
    return Beta;
}