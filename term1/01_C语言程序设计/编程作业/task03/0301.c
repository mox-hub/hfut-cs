//****************************************
//File Name:
//Author:
//Date:
//Student ID:
//****************************************
#include<stdio.h>

int main(void)
{
    float fTax, fPrincipal;
    printf("请输入收入:");
    scanf("%f", &fPrincipal);

    if(fPrincipal <=5000)
        fTax = 0;
    if(fPrincipal > 5000 &&fPrincipal <=8000)
        fTax = fPrincipal * 0.03f;
    if(fPrincipal > 8000 &&fPrincipal <=17000)
        fTax = fPrincipal * 0.10f - 350.00f;
    if(fPrincipal > 17000 &&fPrincipal <=30000)
        fTax = fPrincipal * 0.20f - 2410.00f;
    if(fPrincipal > 30000 &&fPrincipal <=40000)
        fTax = fPrincipal * 0.25f - 3910.00f;
    if(fPrincipal > 40000 &&fPrincipal <=60000)
        fTax = fPrincipal * 0.30f - 5910.00f;
    if(fPrincipal > 60000 &&fPrincipal <=85000)
        fTax = fPrincipal * 0.35f - 8910.00f;
    if(fPrincipal >85000)
        fTax = fPrincipal * 0.45f - 17510.00f;

    printf("您的税费为：%.2f", fTax);

    return 0;
}
