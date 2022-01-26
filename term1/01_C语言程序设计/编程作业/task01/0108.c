/* example1-8 */
#include<stdio.h>

int main(void)
{
    int a, b, c, d, e, f;

    a = sizeof(int);
    b = sizeof(char);
    c = sizeof(double);
    d = sizeof(short);
    e = sizeof(long);
    f = sizeof(float);

    printf("%d\n%d\n%d\n%d\n%d\n%d", a, b, c, d, e, f);

    return 0;
}
