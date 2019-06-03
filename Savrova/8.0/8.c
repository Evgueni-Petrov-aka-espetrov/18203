#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <limits.h>
#include <malloc.h>
#include<stdlib.h>
typedef struct Edge {
    int begin;
    int end;
    int length;
} Edge;

void swap(Edge* a, Edge* b) {
    Edge tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}
int min(int a, int b) {
    return (a < b) ? a:b;
}
int max(int a, int b) {
    return (a > b) ? a:b;
}
void sort(Edge* array, int number) {
    if ((number == 0) || (number == 1))
        return;
    int i = 0, j = number - 1;
    int point = array[rand() % number].length;
    do {
    while (point > array[i].length)
        i++;
    while (point < array[j].length)
        j--;
    if (i <= j) {
        swap(&array[i], &array[j]);
        i++;
        j--;
    }
    } while (i <= j);
    sort(array, j + 1);
    sort(array + i, number - i);
}


Edge* kruskal (Edge* edges, int vertix,int edge) {
    int* vertixcolor = calloc(vertix + 1, sizeof(int));
    int colornumber = 0;
    Edge* result = malloc(sizeof(Edge)*(vertix-1));
    int rescount = 0;

    for (int i = 0; i < edge; i++) {
        int color1 = vertixcolor[edges[i].begin];
        int color2 = vertixcolor[edges[i].end];
        if (color1 && color2 && color1 == color2)
            continue;


        if (!color1 && !color2) {
            colornumber++;
            vertixcolor[edges[i].begin] = colornumber;
            vertixcolor[edges[i].end] = colornumber;
        }
        if (color1 != color2){
            if (!color1)
                vertixcolor[edges[i].begin] = color2;
            else if (!color2)
                vertixcolor[edges[i].end] = color1;
            else {
                int colormin = min(color1, color2);
                int colormax = max(color1, color2);
                for (int j = 0; j <= vertix; j++){
                    if (vertixcolor[j] == colormax)
                        vertixcolor[j] = colormin;
                }
            }
        }
        result[rescount] = edges[i];
        rescount++;
    }

    int ch_color = 0;
    for (int i = 0; i < vertix; i++) {
        if (vertixcolor[i] == 1)
        ch_color++;
    }
    if (ch_color != vertix - 1 || (rescount < vertix - 1) || (vertix == 0))  {
        printf("no spanning tree");
        exit(0);
    }
    free(vertixcolor);
    return result;
}

int main() {
    int vertix;
    int edge;
    scanf("%d ", &vertix);
    if (vertix > 5000) {
        printf("bad number of vertices");
        exit(0);
    }
    scanf("%d ", &edge);
    if (edge > vertix * (vertix + 1) / 2) {
        printf("bad number of edges");
        exit(0);
    }
    Edge* edges = malloc(edge * sizeof(Edge));
    for (int i = 0; i < edge; i++) {
        if (scanf("%d%d%d", &edges[i].begin, &edges[i].end, &edges[i].length) < 3) {
            printf("bad number of lines");
            exit(0);
        };

        if ((edges[i].begin > vertix) || (edges[i].end > vertix) || (edges[i].begin < 0) || (edges[i].end < 0)) {
            printf("bad vertex");
            exit(0);
        }
        if ((0 > edges[i].length) || (edges[i].length > INT_MAX))  {
            printf("bad length");
            exit(0);
        }
        if (edges[i].begin == edges[i].end) {
            edge--;
            i--;
        }
    }
    sort(edges, edge);
    Edge* result = kruskal(edges, vertix, edge);
    for (int i = 0; i < vertix - 1; i++)
        printf("%d %d \n", min(result[i].begin, result[i].end), max(result[i].begin, result[i].end) );
    free(result);
    free(edges);
    return 0;
}
