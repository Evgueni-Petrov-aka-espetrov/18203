#include "dijkstra_string.h"
#include <stdbool.h>

void swap(char *a,char *b)
{
    if(*a!=*b)
    {
        *a^=*b;
        *b^=*a;
        *a^=*b;
    }
}

int shift(char* buffer,int size)
{
    for(int i = size-2;i >= 0;i--)
    {
        if(buffer[i] < buffer[i+1])
        {
            if(buffer[size-1]<buffer[i+1] && buffer[size-1]>buffer[i])
                swap(&buffer[size-1],&buffer[i]);
            else
                swap(&buffer[i+1],&buffer[i]);

            for(int a = i+1;i < size;i++)
            {
                for(int b = a;b < size;b++)
                {
                    if(buffer[a]>buffer[b])
                        swap(&buffer[a],&buffer[b]);
                }
            }
            return true;
        }
    }
    return false;
}

int test(char* buffer,int size)
{
    bool test[10] = {0,0,0,0,0,0,0,0,0,0};
    char *iter = buffer;
    for(int i = 0;i < size;i++,iter++)
    {
        if(test[*iter-'0'] || *iter < '0' || *iter > '9')
            return false;
        else
            test[*iter-'0'] = true;
    }
    return true;
}
