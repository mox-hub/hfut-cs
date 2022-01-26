/* example1-5 */
#include<stdio.h>

int main(void)
{
    char a, b, c, d;

    printf("please enter a character: ");
    scanf("%c",&c);
    a = 'A';
    b = a + 32;
    d = 112;

    printf("a = %c, %d\n", a, a);
    printf("b = %c, %d\n", b, b);
    printf("c = %c, %d\n", c, c);
    printf("d = %c, %d\n", d, d);

    return 0;


}
