#include <stdlib.h>
#include <string.h>
#include "boyer_moore.h"

int table(unsigned char *table, int size, unsigned char test)
{
    for(int i = size-2,a = 1;i >= 0;i--,a++)//Проверяет все символы от предпоследнего до первого
    {
        if(table[i] == test)
            return a;
    }
    return size;//Если нет в таблице
}

void search(FILE *input, FILE *output)
{
    unsigned char template[16];//Шаблон
    fscanf(input,"%[^\n]",template);
    fgetc(input);//Переход на 2 строку

    int length = strlen(template);
    unsigned char buffer[length];//Буффер для проверки

    int pos = 1;
    int test;//Проверка на EOF
    while(true)
    {
        test = fread(buffer,sizeof(unsigned char),length,input);
        if(test != length)
            break;
        for(int i = length-1;i >= 0;i--)
        {
            fprintf(output,"%d ",i+pos);
            if(buffer[i] != template[i])//Проверка найденной строки
                break;
        }
        int shift = table(template, length, buffer[length-1]);//Использует шаблон как таблицу стоп-символов

        if(shift != length)
            fseek(input, shift-length, SEEK_CUR);
        pos += shift;
    }
}
