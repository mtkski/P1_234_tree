
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

node234 A;                     // Array A of nodes
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
  
  while('#' == line[0])          
    getline(&line, &len, stdin);
  
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

void vizShow(FILE *f, int n)          // produce description of current state in dot language
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


// help functions

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

// ------------------------

int compareFrac(struct frac f1, struct frac f2) {

    if (f1.b == 0 || f2.b == 0){
      return 2;
    }
    
    double val1 = (double)f1.a / f1.b;
    double val2 = (double)f2.a / f2.b;
    
    return (val1 > val2) - (val1 < val2);
}

int compareQ(const void* a, const void* b) {
    const struct frac* f1 = (const struct frac*)a;
    const struct frac* f2 = (const struct frac*)b;
    
    return compareFrac(*f1, *f2);
}

void splitNode(node234 c, int i){ 
  /*i is to check what pointer do we split
    c is the current node
    we have to move up p[i] to the correct position in the current node c */
  if(i == 0){
    c->V[2] = c->V[1];
    c->V[1] = c->V[0];
    c->V[0] = c->p[0]->V[1]; //here we moved all the values of the current to the right

    c->p[3] = c->p[2];
    c->p[2] = c->p[1]; /*move the pointers to the right, p0 is already good*/

    c->p[1] = S; /*new node insertion*/
    S = S->p[3];
    c->p[1]->V[0] = c->p[0]->V[2]; 
    
    c->p[0]->V[1].a = 0 ;
    c->p[0]->V[1].b = 0 ;

    c->p[0]->V[2].a = 0 ;
    c->p[0]->V[2].b = 0 ; /*here we delete everything from p0 (only the value of v0 stays)*/
  }
  
}

node234 joinNode(node234 v1, node234 v2){
  /*
  code
  */
  printf("Joining nodes %d%d\n", ptr2loc(v1), ptr2loc(v2));
  return v1;

}


// implemented functions

int searchFrac(struct frac f){

  bool found = false;
  struct node234 *v = root;
  int next;

  while (!found){

    next = 3;

    for (int i = 0; i < 3; i++){ 

      //printf("Looking for %llu/%llu in node %d, comparing to %llu/%llu\n", f.a, f.b, ptr2loc(v), v->V[i].a, v->V[i].b);

      if (compareFrac(f, v->V[i]) == 0){
        found = true;
        return ptr2loc(v);

      } else if (compareFrac(f, v->V[i]) == -1 || compareFrac(f, v->V[i]) == 2){
        next = i;
        break;
      }
    }

    if (v->p[next] != NULL){
      v = v->p[next];
    } else {
      break;
    }
  }
  
  return -1;
};

void inOrder(int tree_size){
  
};

void preOrder(node234 v) { 
  printf("%d ", ptr2loc(v));

  for (int i = 0; i < 4; i++) {
    if (v->p[i] != NULL) {
      preOrder(v->p[i]);
    }
  }
}

int insert(struct frac f){

  if (root == NULL){ // first insertion (check if relevant?)
    root = S;
    S = S->p[3];
    root->V[0] = f;
    root->p[3] = NULL;

    return ptr2loc(root);

  } else {

    int inserted = 0;
    struct node234 *current_node = root;


    if(current_node->V[2].b != 0){     //root is current_node because before the while loop 
      root = S;
      S = S->p[3];

      root->V[0] = current_node->V[1];
      root->p[0] = current_node;

      root->p[1] = S ;
      S = S->p[3];
      root->p[1]->V[0] = current_node->V[2];

      current_node->V[1].a = 0 ;
      current_node->V[1].b = 0 ;

      current_node->V[2].a = 0 ;
      current_node->V[2].b = 0 ;

      printf("Splitted %i", ptr2loc(current_node));
      return ptr2loc(current_node); // return the node that was splitted
    }


    while(!inserted){
      // check for every pointer only we have to descend to it 
      // Alice -> descend the tree to know where to insert
      // we only check every pointer, insert inside only leaf nodes

      if(compare_frac(f, current_node->V[0]) == -1){  // navigate down p0 
        if(current_node->p[0]->V[2].b != 0){ // p0 is a 4node
          splitNode(current_node, 0);
          continue;                         // going to restart the while loop
        }

        current_node = current_node->p[0];               
        // p0 case done 

      } else if(current_node->V[1].b != 0){                     // add frac middle space because it is NULL
        current_node->V[1] = f;
        inserted = 1;
        return ptr2loc(current_node);

      } else if (compare_frac(f, current_node->V[1]) == -1){    // navigate down second pointer
        if(current_node->p[1]->V[2].b != 0){ // p1 is a 4node
          splitNode(current_node, 1);
          continue;                         // going to restart the while loop
        }

        current_node = current_node->p[1];

      } else if(current_node->V[2].b != 0){                     // add frac right space because it is NULL
        current_node->V[2] = f;
        inserted = 1;
        return ptr2loc(current_node);
      }
    }
  }
};

int delete(struct frac f){
  
  int index = searchFrac(f);

  return index;

};


int main(){
  
  //const char* filename = "P1-tests/T03/input";
  //freopen(filename, "r", stdin);

  size_t len = 256;
  char *line = (char*)malloc(len*sizeof(char));
  
  int tree_size; 
  char command;

  int index;
  struct frac f;

  getline(&line, &len, stdin);
  sscanf(line, "%d", &tree_size);  
  
  A = calloc(tree_size, sizeof(node234));
  S = &A[tree_size-1];
  root = NULL;

  for (int i = 1; i < tree_size; i++){
    A[i].p[3] = &A[i-1];
  }

  while (sscanf(line, " %c", &command) != -1) {
  
    getline(&line, &len, stdin);
    sscanf(line, "%c", &command); 

    switch (command) {

            case 'S':
                
                if (sscanf(line, "S %d", &index) == 1) {
                    //printf("Showing node ‰llu", index);
                    showNode(&A[index]);
                }

                break;

            case 'F':
                
                if (sscanf(line, "F %llu/%llu", &f.a, &f.b) == 2) {
                    //printf("The fraction %llu/%llu can be found at index: ", &f.a, &f.b);
                    printf("%d\n", searchFrac(f));

                }

                break;

            case 'N':
                
                if (root != NULL){
                  inOrder(tree_size);
                }
                printf("\n");

                break;

            case 'P':
                if (root != NULL){
                  preOrder(root);
                }
                printf("\n");

                break;

            case 'I':

                if (sscanf(line, "I %llu/%llu", &f.a, &f.b) == 2) {
                    //printf("Inserting fraction %llu/%llu\n", f.a, f.b);
                    index = insert(f);
                    printf("%d\n", index);
                }

                break;

            case 'D':

                if (sscanf(line, "D %llu/%llu", &f.a, &f.b) == 2) {
                    //printf("Deleting fraction %llu/%llu\n", f.a, f.b);
                    index = delete(f);
                    printf("%d\n", index);
                }

                break;

            case 'L':

                //printf("\nLoading tree configuration\n");
                structLoad();
                break;

            case 'X':
                // printing final configuration: location of node S and root + all nodes
                printf("Final configuration:\n");
                printf("S: %d ", ptr2loc(S));
                printf("root: %d \n", ptr2loc(root));

                for (int i = 0; i < tree_size; i++){
                  showNode(&A[i]);
                }

                printf("X\n");
                return 0;

                break;

            case '#':

                printf("%s", line);
                break;

            default:

                printf("\nUnknown command: %c\n", command);
                break;
        }
  }

  free(A);
  free(S);
  free(root);

  //printf("%c\n", command);
  //process_file(filename);

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


