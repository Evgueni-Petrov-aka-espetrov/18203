#ifndef RABIN_KARP_H
#define RABIN_KARP_H

#define MAXSTR 17 //16+\0
#include <stdio.h>

int hashCreate(unsigned char *c,int size);
int hashMove(int *hash, unsigned char begin, unsigned char end, int size);
void strPopForward(unsigned char* c, size_t size);
void search(FILE *input, FILE *output);

#endif // RABIN_KARP_H
