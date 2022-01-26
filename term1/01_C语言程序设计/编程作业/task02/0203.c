//***********************************************
//File name:0203
//Author:¿Ó∫∆»ª
//Date:2019.10.20
//Student ID:2019218211
//***********************************************
#include<stdio.h>
#define PI 3.14159
int main(void)
{
    float r, h, fVolume;
    printf("please input r,h:\n");
    scanf("%f%f", &r, &h);

    fVolume = (PI * r * r * h)/3;

    printf("The volume is:%f", fVolume);

    return 0;
}
