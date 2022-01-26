//****************************************
//File Name:0310
//Author:Li Haoran
//Date:2019.10.29
//Student ID:2019218211
//****************************************
#include<stdio.h>

int main(void)
{
   double dItem, dPi, dSign;

    dItem = 1.0;
    dPi = 0.0;
    dSign = 1.0;

    while(1 / dItem > 1e-8 || 1 / dItem < -1e-8)
    {
        dPi += (1.0 * dSign) / dItem ;
        dItem += 2.0;
        dSign = -dSign;
    }
    dPi *= 4.0;

    printf("Ô²ÖÜÂÊ = %.8f\n", dPi);

    return 0;
}
