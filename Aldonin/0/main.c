#include "stdio.h"
#include "translate.h"

int main()
{
/*-----------------------------------------------*/
    int b1, b2;
    char enter[255];
    FILE *input = fopen("in.txt","r");
    FILE *output = fopen("out.txt","w");

    fscanf(input,"%d %d",&b1,&b2);
    fscanf(input,"%s",enter);
/*-----------------------------------------------*/
    int test = failSafe(b1, b2, enter);
    if(test != 0)
    {
        fprintf(output,"bad input",test);
        return 0;
    }
/*-----------------------------------------------*/
    double d = numToDouble(b1, enter);
    char c[255];

    doubleToNum(c, b2, d);

    fprintf(output,"%s",c);
/*-----------------------------------------------*/
    return 0;
}
