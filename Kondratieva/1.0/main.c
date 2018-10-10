#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN_STR 16
#define MAX_LEN_PAT 17
#define MAX_LEN_TABLE 256

//ñîçäàíèå òàáëèöû ñìåùåíèé
void buildShiftTable (const unsigned char* pat, int* shiftTable){
    int len = strlen(pat);
    for (int i = 0; i < MAX_LEN_TABLE; i++)
        shiftTable[i] = len;
    for (int i = 0; i < len - 1; i++)
        shiftTable[pat[i]] = len - 1 - i;
}

//ïåðåäâèíóòü shift ñèìâîëîâ ñòðîêè ñ åå êîíöà â åå íà÷àëî
void makeShift (int shift, unsigned char* str) {
    for (int i = 0; i < shift; i++)
        str[i] = str[strlen(str) - shift + i];
}

//çàïîëíèòü áóôôåð íà÷èíàÿ ñ ñèìâîëà ñ èíäåêñîì shift
int getString (int shift, unsigned char* str) {
    int count = fread(&str[shift], 1, (size_t) (MAX_LEN_STR - shift), stdin);
    str[shift + count] = '\0';
    if (count == MAX_LEN_STR - shift)
        return 1;
    return 0;
}

//îñóùåñòâëåíèå àëãîðèòìà áîéåðà ìóðà íà ñòðîêå
int Search (const int* shiftTable, const unsigned char* pat, const unsigned char* str, int* n) {
    int skip = 0, sl = strlen(str), pl = strlen(pat);//skip - äëèíà ñìåùåíèé
    while (sl - skip >= pl) {
        int i = pl;
        do {
            printf("%d ", *n + skip + i);
            i--;
        } while(str[skip + i] == pat[i] && i > 0);
        skip += shiftTable[str[skip + pl - 1]];
    }
    *n += skip;//äëèíà ñìåùåíèÿ ïðîøëûõ ðàáîò àëãîðèòìà
    return sl - skip; //êîëè÷åñòâî ñèìâîëîâ âûøåäøåå çà ïðåäåëû ñòðîêè
}

//îñóùåñòâëåíèå àëãîðèòìà áîéåðà ìóðà íà òåêñòå ëþáîé äëèíû
void boyerMoore (const int* shiftTable, unsigned char* pat, unsigned char* str) {
    int textIsOn = 1, shift = 0, n = 0;
    while (textIsOn) {
        textIsOn = getString(shift, str);
        shift = Search(shiftTable, pat, str, &n);
        makeShift(shift, str);
    }
}

int main(){
    unsigned char s[MAX_LEN_STR] = { 0 }, p[MAX_LEN_PAT];
    int shiftTable[MAX_LEN_TABLE];
    scanf("%[^\n]s", p);
    getchar();
    buildShiftTable(p, shiftTable);
    boyerMoore(shiftTable, p, s);
    return 0;
}
