#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> 
#include <stdbool.h>
#include <string.h>
#include <assert.h>

typedef struct node234* node234;

struct frac{ /* Stores a fraction */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"
  unsigned long long int a; /* Numerator */
  unsigned long long int b; /* Denominator */
#pragma GCC diagnostic pop
};

struct node234
{
  struct frac V[3]; /* Key values */
  node234 p[4]; /* Pointers */
};

int ptr2loc(node234 v, node234 A) {  
  int r; r = -1; 
  if(NULL != v) r = ((size_t) v - (size_t) A) / sizeof(struct node234); 
  return (int)r; 
} 

void showNode(node234 v) { 
  int f;
  int k; 
  assert(NULL != v && "Trying to show NULL node."); 
  printf("node: %d ", ptr2loc(v)); 
  k = 0; 
  while(true){ printf("%d ", ptr2loc(v->p[k])); 
    if(3 == k) break; 
    printf("%llu/%llu ", v->V[k].a, v->V[k].b);
    k++; } printf("\n"); 
}

int main(int argc, char const *argv[])
{
  int n ;
  node234 A;
  scanf("%d", &n); 
  A = calloc(n, sizeof(struct node234));

  A[1].V[1].a = 1;
  A[1].V[1].b = 2;

  A[1].V[2].a = 3;
  A[1].V[2].b = 4;

  printf("node: %d ", ptr2loc(A,A)); 
  return 0;
}
