/* example1-9 */
#include<stdio.h>
#include<math.h>

int main(void)
{
    float x, y, dDistance, dAngle;

    printf("请输入x的值:");
    scanf("%f", &x);
    printf("请输入y的值:");
    scanf("%f", &y);

    dDistance = sqrt((x * x)+(y * y));
    dAngle = atan(y / x);

    printf("The Distance is:%f\n", dDistance);
    printf("The Angle is:%f rad\n", dAngle);

    return 0;
}




