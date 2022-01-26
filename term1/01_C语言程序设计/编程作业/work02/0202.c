//***********************************************
//File name:0202
//Author:¿Ó∫∆»ª
//Date:2019.10.20
//Student ID:2019218211
//***********************************************
#include<stdio.h>

int main(void)
{
    float a, b, c, fVolume;
    printf("Please input long,weight,high:\n");
    scanf("%f%f%f", &a, &b, &c);

    fVolume = a * b * c;

    printf("The volume is:%.2f", fVolume);

    return 0;
}
