#include <stdio.h>
#include <string.h>
#include "dijkstra_string.h"

int main()
{
    FILE *input = fopen("in.txt", "r");
    FILE *output = fopen("out.txt","w");

    int times;
    char buffer[10+1];
    fscanf(input,"%s %d",buffer,&times);

    if(!test(buffer,strlen(buffer)))
    {
        fprintf(output,"bad input");
        return 0;
    }

    for(int i = 0;i < times;i++)
    {
        if(shift(buffer,strlen(buffer)))
            fprintf(output,"%s\n",buffer);
    }

    fclose(input);
    fclose(output);

    return 0;
}
