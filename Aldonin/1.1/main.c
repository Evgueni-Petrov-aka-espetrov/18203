#include <stdio.h>
#include "rabin_karp.h"

int main()
{
    FILE *input = fopen("in.txt", "r");
    FILE *output = fopen("out.txt","w");

    search(input, output);

    fclose(input);
    fclose(output);

    return 0;
}
