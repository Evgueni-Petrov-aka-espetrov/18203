#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int table(char *table, int size, char test)
{
    for(int i = size-2,a = 1;i >= 0;i--,a++)
    {
        if(table[i] == test)
            return a;
    }
    return size;
}

int main()
{
    FILE *input = fopen("in.txt", "r");
    FILE *output = fopen("out.txt","w");
    
    char example[16];
    fscanf(input,"%s",example);
    fseek(input,sizeof(char),SEEK_CUR);
    int length = strlen(example);
    
    char buffer[length];
    int pos = 1;
    int test = length;
    while(1)
    {
        test = fread(buffer,sizeof(char),length,input);
        if(test != length)
            break;
        for(int i = length-1;i >= 0;i--)
        {
            fprintf(output,"%d ",i+pos);
            if(buffer[i] != example[i])
                break;
        }
        int shift = table(example, length, buffer[length-1]);
        fseek(input, shift-length, SEEK_CUR);
        pos += shift;
    }
}