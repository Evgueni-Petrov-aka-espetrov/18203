#ifndef BOYER_MOORE_H
#define BOYER_MOORE_H

#define true 1
#include <stdio.h>

int table(unsigned char *table, int size, unsigned char test);
void search(FILE *input, FILE *output);

#endif // BOYER_MOORE_H
