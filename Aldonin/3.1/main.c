#include <stdio.h>
#include <stdlib.h>

void change(int* a, int* b)
{
    //printf("\n$ %d %d",*a,*b);
    if(*a != *b)
    {
        *a ^= *b;
        *b ^= *a;
        *a ^= *b;
    }
}



void quickSort(int* stack,int begin,int end)
{
    if(begin >= end)
        return;

    int iterSmall = begin-1;
    int iterLarge = end+1;
    int anchorStack = stack[end/2+begin];

    while(1)
    {
        do
        {
            iterSmall++;
        }while(stack[iterSmall] < anchorStack);

        do
        {
            iterLarge--;
        }while(stack[iterLarge] > anchorStack);

        if(iterSmall>iterLarge)
            break;

        change(&stack[iterSmall],&stack[iterLarge]);
    }

    quickSort(stack,begin,iterLarge-1);
    quickSort(stack,iterLarge+1,end);
}

int main()
{
    FILE *input,*output;
    input = fopen("in.txt","r");
    output = fopen("out.txt","w");

    int size;
    fscanf(input,"%d\n",&size);

    int* stack = (int*) malloc(size*sizeof(int));
    if(stack == NULL)
    {
        fprintf(output,"segmentation fault");
        return 0;
    }
    for(int i = 0;i < size;i++)
    {
        fscanf(input,"%d ",&stack[i]);
    }

    quickSort(stack,0,size-1);
    for(int i = 0;i < size;i++)
        fprintf(output,"%d ",stack[i]);
    return 0;
}
