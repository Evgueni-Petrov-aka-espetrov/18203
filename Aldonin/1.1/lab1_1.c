#include "stdio.h"
#include "string.h"
#include "stdbool.h"

#define MAXSTR 16

const int exp3[MAXSTR] = {1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683, 59049, 177147, 531441, 1594323, 4782969, 14348907};

int hashCreate(unsigned char *c,int size)
{
    int sum = 0;
    for(int i = 0;i < size;i++)
    {
        sum += (c[i]%3)*exp3[i];
    }
    return sum;
}

int hashMove(int *hash, unsigned char begin, unsigned char end, int size)
{
    *hash -= begin%3;
    *hash /= 3;
    *hash += (end%3)*exp3[size-1];
    return *hash;
}

void strPopForward(unsigned char* c, int size)
{
    for(int i = 1;i < size;i++)
    {
        c[i-1] = c[i];
    }
}

int main()
{
    FILE *input = fopen("in.txt", "r");
    FILE *output = fopen("out.txt","w");
    
    unsigned char example[MAXSTR];
    fscanf(input,"%[^\n]",example);
    fseek(input,1,SEEK_CUR);
    int length = strlen((char*)example);
    int exampleHash = hashCreate(example,length);
    fprintf(output,"%d ",exampleHash);
    
    unsigned char buffer[length];
    fread(buffer,sizeof(unsigned char),length,input);
    int hash = hashCreate(buffer,length);
    for(int i = 1;!feof(input);i++)
    {
        bool isEqual = false;
        if(hash == exampleHash)
        {
            isEqual = true;
            for(int j = 0;j < length;j++)
            {
                fprintf(output,"%d ",j+i);
                if(buffer[j] != example[j])
                {
                    break;
                }
            }
        }
        if(isEqual)
        {
            i+= length-1;
            fread(buffer,sizeof(unsigned char),length,input);
            hash = hashCreate(buffer,length);
        }
        else
        {
            unsigned char begin = buffer[0];
            strPopForward(buffer,length);
            unsigned char end = fgetc(input);
            buffer[length-1] = end;
            hashMove(&(hash),begin,end,length);
        }
    }
}