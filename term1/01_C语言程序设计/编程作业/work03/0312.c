//****************************************
//File Name:
//Author:
//Date:
//Student ID:
//****************************************
#include<stdio.h>

int main(void)
{
    double dCosx, x, dItem;
    int i = 2;
    dCosx = 0.0;
    dItem = 1.0;
    printf("请输入x的值:");
    scanf("%lf", &x);

    while(x > 3.14159 / 2)
        x -= 3.14159;
    while(x < -3.14159 / 2)
        x += 3.14159;

    while(1)
    {
        dCosx += dItem;
        dItem *= x * x /(i * (i - 1) * (-1.0));
        i += 2;
        if(dItem < 1.0e-6&&dItem > -1.0e-6)
            break;
    }
    printf("Cosx的值为:%f", dCosx);

    return 0;
}
