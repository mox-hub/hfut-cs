/* example1-6 */
#include<stdio.h>

int main(void)
{
    int a = 9, b = 5;

    printf("%d, %d\n", a, b);
    printf("%d, %d\n", a++, b--);
    printf("%d, %d\n", ++a, --b);

    return 0;
}
