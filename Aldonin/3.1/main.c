#include <stdio.h>

void change(int* a, int* b)
{
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

void quickSort(int *stack,int size)
{
    if(size <= 1)
        return;
    int iterMid = 0;
    int iter = 1;

    for(int i = 0;i < size;i++)
    {
        if(stack[iter] < stack[iterMid])
        {
            if(stack[iterMid+1] != stack[iter])
                change(&stack[iterMid+1],&stack[iter]);
            change(&stack[iterMid],&stack[iterMid+1]);
            iterMid++;
        }
        iter++;
    }

    quickSort(stack,iterMid);
    quickSort(&stack[iterMid+1],size-iterMid-1);
}

int main()
{
    FILE *input,*output;
    input = fopen("in.txt","r");
    output = fopen("out.txt","w");

    int size;
    fscanf(input,"%d\n",&size);

    int *stack;
    if(stack = (int*) malloc(size*sizeof(int)) == NULL)
    {
        fprintf(output,"segmentation fault");
        return 0;
    }
    for(int i = 0;i < size;i++)
    {
        fscanf(input,"%d ",&stack[i]);
    }

    quickSort(stack,size);
    for(int i = 0;i < size;i++)
        fprintf(output,"%d ",stack[i]);
    return 0;
}
