//***********************************************
//File name:0201
//Author:¿Ó∫∆»ª
//Date:2019.10.20
//Student ID:2019218211
//***********************************************
#include<stdio.h>
#include<math.h>

int main(void)
{
    float a, b, c, q, fArea;
    printf("please input triangles side:\n");
    scanf("%f%f%f", &a, &b, &c);

    q = (a + b + c) *0.5;
    fArea = sqrt(q * (q - a) * (q - b) * (q - c));

    printf("The area is:%.2f", fArea);

}
