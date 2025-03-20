
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> 
#include <stdbool.h>
#include <string.h>
#include <assert.h>


typedef struct edge* edge;

struct edge{
  int v[2];             // nodes connected by edge
  int cost;
};

edge edgeArray;

int compare(const void* a, const void* b) {
   return (*(int*)a - *(int*)b);
}

void main(){

    size_t len = 256;
    char *line = (char*)malloc(len*sizeof(char));
    char *tok;

    int N;
    int B;
    int E;

    edgeArray = calloc(E, sizeof(edge));

    getline(&line, &len, stdin);
    sscanf(line, "%d\n", &N);  

    getline(&line, &len, stdin);
    sscanf(line, "%d\n", &B);  

    getline(&line, &len, stdin);
    sscanf(line, "%d\n", &E);  

    for (int i = 0; i < E; i++){

        getline(&line, &len, stdin);
        tok = strtok(line, " ");               
        sscanf(tok, "%d %d %d", &edgeArray[i].v[0], &edgeArray[i].v[1], &edgeArray[i].cost);   

    }

    int n = sizeof(edgeArray) / sizeof(edgeArray[0]);

    qsort(edgeArray->cost, n, sizeof(int), compare);

}
