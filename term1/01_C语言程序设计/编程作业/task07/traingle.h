#ifndef __TRAINGLE_H__
#define __TRAINGLE_H__

#include <stdio.h>
#define PI 3.141592
double sinX(int Angle)
{
    int i;
    double n , result = 0,x = (PI / 180) * Angle;
    n = x;
    for(i = 0;n > 1e-6 || n < -1e-6;i += 2)
    {
        result += n;
        n *= (x * x)/(i * (i + 1));
        n *= -1;
    }
    return(result);
}
double cosX(int Angle)
{
    int i;
    double n = 1, result = 0,x = (PI / 180) * Angle;
    for(i =1;n > 1e-6 || n < -1e-6;i += 2)
    {
        result += n;
        n *= (x * x)/(i * (i + 1));
        n *= -1;
    }
    return(result);
}
#endif
