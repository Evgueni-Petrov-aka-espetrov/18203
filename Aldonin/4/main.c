#include <stdio.h>
#include <stack.h>
#include <calc.h>

int main()
{
    Stack numbers,symbols;
    init(&numbers,2);
    init(&symbols,1);

    FILE *input,*output;
    input = fopen("in.txt","r");
    output = fopen("out.txt","w");

    int c = calc(input,&numbers,&symbols);
    if(c == 0)
        fprintf(output,"%d",numbers.stack[0]);
    else if(c == -1)
        fprintf(output,"syntax error");
    else if(c == -2)
        fprintf(output,"division by zero");

    return 0;
}
