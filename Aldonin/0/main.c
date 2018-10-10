#include "stdio.h"
#include "translate.h"

int main()
{
/*-----------------------------------------------*/
    int b1, b2;
    char enter[255];

    scanf("%d %d",&b1,&b2);
    scanf("%s",enter);
/*-----------------------------------------------*/
    int test = failSafe(b1, b2, enter);
    if(test != 0)
    {
        printf("bad input",test);
        return 0;
    }
/*-----------------------------------------------*/
    double d = numToDouble(b1, enter);
    char c[255];

    doubleToNum(c, b2, d);
    printf("%s",c);
/*-----------------------------------------------*/
    return 0;
}