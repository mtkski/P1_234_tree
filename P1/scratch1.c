
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> 
#include <stdbool.h>
#include <string.h>
#include <assert.h>


// structures

typedef struct node234* node234;

struct frac{                                    // Stores a fraction
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"
  unsigned long long int a;                     // Numerator
  unsigned long long int b;                     // Denominator
#pragma GCC diagnostic pop
};

struct node234{
  struct frac V[3];         // Key values
  node234 p[4];             // Pointers
};


// global variables 

struct node234 A[20];          // Array A of nodes
struct node234* root;          // root node 

struct node234 *S;             // Stack of nodes 



// given functions

int ptr2loc(node234 v)     // return index i of A[i] that contains node v 
{
  int r;
  r = -1;

  if(NULL != v)
    r = ((size_t) v - (size_t) A) / sizeof(struct node234);

  return (int)r;
}


void showNode(node234 v)    // prints node
{
  int f;
  int k;
  assert(NULL != v && "Trying to show NULL node.");

  printf("node: %d ", ptr2loc(v));
  k = 0;
  
  while(true){
    printf("%d ", ptr2loc(v->p[k]));
    if(3 == k) break;
    printf("%llu/%llu ", v->V[k].a, v->V[k].b);
    k++;
  }
  
  printf("\n");

}


void structLoad(void)          // load a config directly to array A
{
  int i;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"
  unsigned long long int j;
#pragma GCC diagnostic pop
  int k;
  char c;
  char *tok;
  size_t len = 1<<8;
  char *line = (char*)malloc(len*sizeof(char));

  line[0] = '#';
  printf("line: %s\n", line);

  while('#' == line[0])     
    fgets(line, len, stdin);
  
  tok = strtok(line, " ");         
  tok = strtok(NULL, " ");        
  sscanf(tok, "%d", &i);        
  S = NULL;                         

  if(-1 != i)
    S = &A[i];

  tok = strtok(NULL, " ");
  tok = strtok(NULL, " ");
  sscanf(tok, "%d", &i);
  root = NULL;
  
  if(-1 != i)
    root = &A[i];
  printf("line: %s\n", line);
  while(-1 != getline(&line, &len, stdin) && 'X' != line[0]){
    char *tok = strtok(line, " ");

    if(0 == strcmp("node:", tok)){
      tok = strtok(NULL, " ");
      sscanf(tok, "%d", &i);

      k = 0;

      while(true){
        tok = strtok(NULL, " ");
        sscanf(tok, "%llu", &j);
        A[i].p[k] = NULL;
        
        if(-1 != j)
          A[i].p[k] = &A[j];

        if(3 == k) 
          break;

        tok = strtok(NULL, "/");
        sscanf(tok, "%llu", &j);
        A[i].V[k].a = j;

        tok = strtok(NULL, " ");
        sscanf(tok, "%llu", &j);
        A[i].V[k].b = j;

        k++;
            }
    }

  }
  free(line);
}


void  
vizShow(FILE *f, int n)          // produce description of current state in dot language
{
  int i;
  node234 *Q = calloc(n+1, sizeof(node234));  // Queue of nodes to print 
  int in = 0;
  int out = 0;

  fprintf(f, "digraph {\n");
  fprintf(f, "node [shape=record];");
  fprintf(f, "splines=false;\n");
  Q[in] = root;
  in++;

  while(NULL != Q[out]){                      // Non-empty Queue 
    i = ptr2loc(Q[out]);
    fprintf(f, "A%d [label=\"{<h> A[%d] |{<p0> %llu/%llu |<p1> %llu/%llu |<p2> %llu/%llu}}\"]\n",
	    i, i,
	    A[i].V[0].a, A[i].V[0].b,
	    A[i].V[1].a, A[i].V[1].b,
	    A[i].V[2].a, A[i].V[2].b
	    );

    i = 0;

    while(NULL != Q[out]->p[i] && i < 4){
      Q[in] = Q[out]->p[i];
      in++;
      i++;
    }
    out++;
  }

  in = 0;
  out = 0;
  Q[in] = root;
  in++;

  while(NULL != Q[out]){                      // Non-empty Queue 
    i = 0;

    while(NULL != Q[out]->p[i] && i < 4){
      if(3 == i)
        fprintf(f, "A%d:p2:se -> A%d:h:n\n",
        ptr2loc(Q[out]),
        ptr2loc(Q[out]->p[i]));
      else
        fprintf(f, "A%d:p%d:sw -> A%d:h:n\n",
        ptr2loc(Q[out]), i,
        ptr2loc(Q[out]->p[i]));

      Q[in] = Q[out]->p[i];
      in++;
      i++;
    }
    out++;
  }

  fprintf(f, "}\n");
  free(Q);
}



// implemented functions



void preOrder(node234 v) { 
  printf("node: %d ", ptr2loc(v));

  for (int i = 0; i < 4; i++) {
    if (v->p[i] != NULL) {
      preOrder(v->p[i]);
    }
  }
}

/* Function to compute the greatest common divisor using Euclidean algorithm */
unsigned long long gcd(unsigned long long a, unsigned long long b) {
  while (b != 0) {
      unsigned long long temp = b;
      b = a % b;
      a = temp;
  }
  return a;
}

/* Function to compare two fractions using the Euclidean algorithm */
int compare_frac(struct frac f1, struct frac f2) {
  unsigned long long gcd1 = gcd(f1.a, f1.b);
  unsigned long long gcd2 = gcd(f2.a, f2.b);
  
  f1.a /= gcd1;
  f1.b /= gcd1;
  f2.a /= gcd2;
  f2.b /= gcd2;
  
  unsigned long long lhs = f1.a * f2.b;
  unsigned long long rhs = f2.a * f1.b;
  
  if (lhs < rhs) return -1;
  if (lhs > rhs) return 1;
  return 0;
}

/* Search function for the 2-3-4 tree */
struct node234* search_234(struct node234* root, struct frac target) {
  if (root == NULL) return NULL;
  
  int i;
  for (i = 0; i < 3; i++) {
      if (root->V[i].b == 0) break; // Stop at the first empty fraction slot
      int cmp = compare_frac(target, root->V[i]);
      if (cmp == 0) return root; // Found the fraction
      if (cmp < 0) break; // Target should be in the left subtree
  }
  
  return search_234(root->p[i], target); // Recur to the appropriate subtree
}

void process_file(const char* filename) {
  FILE* file = fopen(filename, "r");
  if (!file) {
      perror("Error opening file");
      return;
  }
  
  char line[256];
  while (fgets(line, sizeof(line), file)) {
      if (line[0] == '#') {
          printf("%s", line); // Print comment lines
          continue;
      }
      
      char command;
      struct frac f;
      int index;
      
      if (sscanf(line, " %c", &command) != 1) continue;
      
      switch (command) {
          case 'S':
              if (sscanf(line, "S %d", &index) == 1) {
                  printf("Show node at index %d\n", index);
                  // Call showNode(index);
              }
              break;
          case 'F':
              if (sscanf(line, "F %llu/%llu", &f.a, &f.b) == 2) {
                  printf("Searching for fraction %llu/%llu\n", f.a, f.b);
                  // Call search function and return index
              }
              break;
          case 'N':
              printf("Printing tree in inorder\n");
              // Call inorder print function
              break;
          case 'P':
              printf("Printing tree in preorder\n");
              // Call preorder print function
              break;
          case 'I':
              if (sscanf(line, "I %llu/%llu", &f.a, &f.b) == 2) {
                  printf("Inserting fraction %llu/%llu\n", f.a, f.b);
                  // Call insert function
              }
              break;
          case 'D':
              if (sscanf(line, "D %llu/%llu", &f.a, &f.b) == 2) {
                  printf("Deleting fraction %llu/%llu\n", f.a, f.b);
                  // Call delete function
              }
              break;
          case 'L':
              printf("Loading tree configuration\n");
              structLoad();
              break;
          case 'X':
              printf("Terminating program\n");
              fclose(file);
              return;
          default:
              printf("Unknown command: %c\n", command);
      }
  }
  
  fclose(file);
}


int main(){
    const char* filename = "P1-tests/T03/input";
    freopen(filename, "r", stdin);

    int i;
    size_t len = 256;
    char *line = (char*)malloc(len*sizeof(char));
    int k;
    char c;
    char *tok;

    while (getline(&line, &len, stdin) != -1) {
        printf("%s", line);
        tok = strtok(line, " ");             
        sscanf(tok, "%d", &i); 

        printf("tok: %c\n", *tok);
        printf("i: %d\n", i);
        printf("S: %d\n", ptr2loc(S));
    }
    free(line);

//   structLoad();
//   printf("%llu\n", A[0].V[0].a);// printing final configuration and location of node S and root 

//   printf("Final configuration:\n");
//   printf("S: %d ", ptr2loc(S));
//   printf("root: %d \n", ptr2loc(root));


  // debugging code, will be removed by preprocessor in mooshak: 
  /* 
  #ifndef NDEBUG 
        sprintf(fname, "tree%.3d.dot", fc);
        fistr = fopen(fname, "w");
        vizShow(fistr, n);
        fclose(fistr);
        fc++;
  #endif 
  */

}


