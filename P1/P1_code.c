
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> 
#include <string.h>
#include <assert.h>


/* structures */

typedef struct node234* node234;

struct frac{                                    /* Stores a fraction*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"
  unsigned long long int a;                     /* Numerator*/
  unsigned long long int b;                     /* Denominator*/
#pragma GCC diagnostic pop
};

struct node234{
  struct frac V[3];         /* Key values*/
  node234 p[4];             /* Pointers*/
};


/* global variables */

struct node234 *A;             /* Array A of nodes*/
struct node234 *root;          /* root node */

struct node234 *S;             /* Stack of nodes */

/* given functions*/

int ptr2loc(node234 v)     /* return index i of A[i] that contains node v */
{
  int r;
  r = -1;

  if(NULL != v)
    r = ((size_t) v - (size_t) A) / sizeof(struct node234);

  return (int)r;
}

void showNode(node234 v)    /* prints node*/
{
  int f;
  int k;
  assert(NULL != v && "Trying to show NULL node.");

  printf("node: %d ", ptr2loc(v));
  k = 0;
  
  while(1){
    printf("%d ", ptr2loc(v->p[k]));
    if(3 == k) break;
    printf("%llu/%llu ", v->V[k].a, v->V[k].b);
    k++;
  }
  
  printf("\n");

}

void structLoad(void)          /* load a config directly to array A*/
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

      while(1){
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

void vizShow(FILE *f, int n)          /* produce description of current state in dot language*/
{
  int i;
  node234 *Q = calloc(n+1, sizeof(node234));  /* Queue of nodes to print */
  int in = 0;
  int out = 0;

  fprintf(f, "digraph {\n");
  fprintf(f, "node [shape=record];");
  fprintf(f, "splines=false;\n");
  Q[in] = root;
  in++;

  while(NULL != Q[out]){                      /* Non-empty Queue */
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

  while(NULL != Q[out]){                      /* Non-empty Queue */
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


/* help functions*/

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
  unsigned long long lhs = f1.a * f2.b;
  unsigned long long rhs = f2.a * f1.b;

  if(f2.b == 0) { /* case where second value is not defined, so we know we have to descend to the pointer to the left*/
    return -1;
  }
  
  if (lhs < rhs) return -1;
  if (lhs > rhs) return 1;
  if (lhs == rhs) return 0;
  return 0;
}

/* ------------------------*/

void splitNode(node234 c, int i){ 
  /*i is to check what pointer do we split
    c is the current node
    we have to move up p[i] to the correct position in the current node c */
  if(i == 0){
    c->V[2] = c->V[1];
    c->V[1] = c->V[0];
    c->V[0] = c->p[0]->V[1]; /*here we moved all the values of the current to the right*/

    c->p[3] = c->p[2];
    c->p[2] = c->p[1]; /*move the pointers to the right, p0 is already good*/

    c->p[1] = S; /*new node insertion*/
    S = S->p[3];
    c->p[1]->p[3] = NULL;
    c->p[1]->V[0] = c->p[0]->V[2]; 
    c->p[1]->p[0] = c->p[0]->p[2];
    c->p[1]->p[1] = c->p[0]->p[3];
    c->p[0]->p[2] = NULL;
    c->p[0]->p[3] = NULL; 
    
    c->p[0]->V[1].a = 0 ;
    c->p[0]->V[1].b = 0 ;

    c->p[0]->V[2].a = 0 ;
    c->p[0]->V[2].b = 0 ; /*here we delete everything from p0 (only the value of v0 stays)*/
  }

  if(i == 1){
    c->V[2] = c->V[1];
    c->V[1] = c->p[1]->V[1]; /*here we moved all the values of the current to the right*/

    c->p[3] = c->p[2];

    c->p[2] = S; /*new node insertion*/
    S = S->p[3];
    c->p[2]->p[3] = NULL;
    c->p[2]->V[0] = c->p[1]->V[2];
    c->p[2]->p[0] = c->p[1]->p[2];
    c->p[2]->p[1] = c->p[1]->p[3];
    c->p[1]->p[2] = NULL;
    c->p[1]->p[3] = NULL;

    c->p[1]->V[1].a = 0 ;
    c->p[1]->V[1].b = 0 ;

    c->p[1]->V[2].a = 0 ;
    c->p[1]->V[2].b = 0 ; /*here we delete everything from p1 (only the value of v0 stays)*/
  }

  if(i == 2){
    c->V[2] = c->p[2]->V[1]; /*here we moved all the values of the current to the right*/

    c->p[3] = S; /*new node insertion*/
    S = S->p[3];
    c->p[3]->p[3] = NULL;
    c->p[3]->V[0] = c->p[2]->V[2];
    c->p[3]->p[0] = c->p[2]->p[2];
    c->p[3]->p[1] = c->p[2]->p[3];
    c->p[2]->p[2] = NULL;
    c->p[2]->p[3] = NULL;

    c->p[2]->V[1].a = 0 ;
    c->p[2]->V[1].b = 0 ;

    c->p[2]->V[2].a = 0 ;
    c->p[2]->V[2].b = 0 ; /*here we delete everything from p2 (only the value of v0 stays)*/
  }
  
}

void leafInsert(struct frac f, struct node234 *leaf){     /* only called at non-empty and non-full leaf nodes, i.e either one or two values exist in the leaf*/

  if (leaf->V[2].b != 0) {
    printf("Error: Attempted to insert into a full leaf.\n");
    return;
  }


  if(compare_frac(f, leaf->V[0]) == -1){        /* if fraction f is less than first value -> insert at begining and shift all values to the right */
    
    leaf->V[2] = leaf->V[1];
    leaf->V[1] = leaf->V[0];

    leaf->V[0] = f;
  
  } else if (leaf->V[1].b == 0){                /* if second value is empty, insert at second slot*/

    leaf->V[1] = f;
  
  } else if (compare_frac(f, leaf->V[1]) == -1){   /* if fraction f is less than second value -> insert at second slot and shift last value to third spot */

      leaf->V[2] = leaf->V[1];
      leaf->V[1] = f;

  } else {                                         /* if fraction f is larger than second value -> insert at last spot*/
    
      leaf->V[2] = f;
  
  }
}


/* implemented functions*/

int searchFrac(struct frac f){

  int found = 0;
  struct node234 *v = root;
  int next;

  while (!found){
    int i;
    next = 3;

    for (i = 0; i < 3; i++){ 

      /*printf("Looking for %llu/%llu in node %d, comparing to %llu/%llu\n", f.a, f.b, ptr2loc(v), v->V[i].a, v->V[i].b);*/

      if (compare_frac(f, v->V[i]) == 0){
        found = 1;
        return ptr2loc(v);

      } else if (compare_frac(f, v->V[i]) == -1){
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
}

void inOrder(node234 v){

  int i;
  for (i = 0; i < 3; i++){
    
    if (v->p[i] != NULL){
      inOrder(v->p[i]);
    }

    if(v->V[i].b != 0){
      printf("%llu/%llu ", v->V[i].a, v->V[i].b);
    }
  }

  if (v->p[3] != NULL){
    inOrder(v->p[3]);
  }

}

void preOrder(node234 v) { 
  int i;
  printf("%d ", ptr2loc(v));
  
  for (i = 0; i < 4; i++) {
    if (v->p[i] != NULL) {
      preOrder(v->p[i]);
    }
  }
}

int isLeaf(node234 v) {
  return (v->p[0] == NULL && v->p[1] == NULL && v->p[2] == NULL && v->p[3] == NULL);
}

int is4node(node234 v ){
  return v->V[2].b != 0;
}

int is3node(node234 v){
  return v->V[2].b == 0 && v->V[1].b != 0;
}

int is2node(node234 v){
  return v->V[1].b == 0;
}

int isFracInNode(struct frac f, node234 v) {
  /* returns 1 if f is in node v, 0 otherwise */
  return (compare_frac(f, v->V[0]) == 0 || 
          compare_frac(f, v->V[1]) == 0 || 
          compare_frac(f, v->V[2]) == 0);
}

int positionInNode(struct frac f, node234 v) {
  /* returns position of f in node v, if not found returns -1 */
  if (compare_frac(f, v->V[0]) == 0) return 0;
  if (compare_frac(f, v->V[1]) == 0) return 1;
  if (compare_frac(f, v->V[2]) == 0) return 2;
  return -1; /* not found */
}

void joinNode(int vi, node234 c, node234 n1, node234 n2){
  printf("Joining nodes %d %d\n", ptr2loc(n1), ptr2loc(n2));
  if (is2node(n2))
  { /* ici on sait que n1 et n2 sont des 2nodes */
    /*put values in n1 */
    n1->V[1] = c->V[vi];
    n1->V[2] = n2->V[0]; 
    n1->p[2] = n2->p[0];
    n1->p[3] = n2->p[1];
    showNode(n1);
    /*put the empty node back to the stack (n2)*/
    // TODO put back to S (don't know how)
    n2->V[0].a = 0 ;
    n2->V[0].b = 0 ;
    n2->p[2] = NULL ;

    n2->p[3] = S ;
    S = n2 ;
    /* change values and pointers of c*/
    switch (vi)
    {
    case 0 :
      c->V[0] = c->V[1]; c->V[1] = c->V[2]; c->V[2].a = 0; c->V[2].b = 0;
      c->p[1] = c->p[2]; c->p[2] = c->p[3]; c->p[3] = NULL;
      break;
    case 1 :
      c->V[1] = c->V[2]; c->V[2].a = 0; c->V[2].b = 0;
      c->p[2] = c->p[3]; c->p[3] = NULL;
      break;
    case 2:
      c->V[2].a = 0; c->V[2].b = 0;
      c->p[3] = NULL;
      break;
    default:
      break;
    }
  }

  else if(is3node(n2)){

  }

  else if(is4node(n2)){

  }
}

int insert(struct frac f){

  if (root == NULL){ /* first insertion (check if relevant?)*/
    root = S;
    S = S->p[3];
    root->V[0] = f;
    root->p[3] = NULL;
    return ptr2loc(root);

  } else {

    int inserted = 0;
    struct node234 *current_node = root;
    /*root is current_node because before the while loop*/
    if(is4node(current_node)){
      printf("Spliting node %d\n", ptr2loc(current_node));

      root = S;
      S = S->p[3];
      root->p[3] = NULL;

      root->V[0] = current_node->V[1];
      root->p[0] = current_node;

      root->p[1] = S ;
      S = S->p[3];
      root->p[1]->V[0] = current_node->V[2];
      root->p[1]->p[0] = current_node->p[2];
      root->p[1]->p[1] = current_node->p[3];
      current_node->p[2] = NULL;
      current_node->p[3] = NULL;
      root->p[1]->p[3] = NULL;

      current_node->V[1].a = 0 ;
      current_node->V[1].b = 0 ;

      current_node->V[2].a = 0 ;
      current_node->V[2].b = 0 ;

      current_node = root; /* now we can continue with the current node as root*/

    }

    else if(isLeaf(current_node)){  /* if root is leaf node, insert directly*/
      leafInsert(f, current_node);
      inserted = 1;

      return ptr2loc(current_node);
    }


    while(!inserted){
      /*Check for every pointer if we have to descend to it 
        then split if it's 4 node and redo the loop. 
        Insert inside only leaf nodes. */
      if(compare_frac(f, current_node->V[0]) == -1){  /* navigate down p0 */
        if(is4node(current_node->p[0])){ 
          printf("Spliting node %d\n", ptr2loc(current_node->p[0]));
          splitNode(current_node, 0);
          continue;                       
        }

        current_node = current_node->p[0];

        if(isLeaf(current_node)){ 
          leafInsert(f, current_node);
          inserted = 1;

        return ptr2loc(current_node);
        }
      } 
      
 
      else if(compare_frac(f, current_node->V[1]) == -1){ /* Navigate down p1*/
        if(is4node(current_node->p[1])){ 
          printf("Spliting node %d\n", ptr2loc(current_node->p[1]));
          splitNode(current_node, 1);
          continue;                        
        }

        current_node = current_node->p[1];

        if(isLeaf(current_node)){  
          leafInsert(f, current_node);
          inserted = 1;
        return ptr2loc(current_node);
        }
      } 
      
      else if(compare_frac(f, current_node->V[2]) == -1){    /* navigate down p2*/
        if(is4node(current_node->p[2])){ 
          printf("Spliting node %d\n", ptr2loc(current_node->p[2]));
          splitNode(current_node, 2);
          continue;
        }

        current_node = current_node->p[2];

        if(isLeaf(current_node)){ 
          leafInsert(f, current_node);
          inserted = 1;
        return ptr2loc(current_node);
        }
      } 
      else { /* navigate down p3 (we only do this 'if' when we splitted before)*/

        current_node = current_node->p[3];

        if(isLeaf(current_node)){ 
          leafInsert(f, current_node);
          inserted = 1;
        return ptr2loc(current_node);
        }
      }
    }
  }
  return -1; /* if we reach here, something went wrong*/
}

int deleteFromLeaf(struct frac f, struct node234 *leaf) {
  /* delete fraction f from leaf node */
  int pos = positionInNode(f, leaf);  
  switch (pos)
  {
  case 0 : leaf->V[0] = leaf->V[1]; leaf->V[1] = leaf->V[2]; leaf->V[2].a = 0; leaf->V[2].b = 0; break;
  case 1 : leaf->V[1] = leaf->V[2]; leaf->V[2].a = 0; leaf->V[2].b = 0; break;
  case 2 : leaf->V[2].a = 0; leaf->V[2].b = 0; break;
  default: break;
  }
  return ptr2loc(leaf); 
}

int delete(struct frac f){
  int index = searchFrac(f);
  int deleted = 0;
  struct node234 *c = root;
  printf("___Deleting %llu/%llu\n", f.a, f.b);

  while(!deleted){
    if (compare_frac(f, c->V[0]) == -1){ /*move to p0*/
      if(is2node(c->p[0])){ /*p0 is a 2-node*/
        /*check for sibling in p[1], otherwhise take p[2] ?*/ /*TO CHECK*/
        /*p[1] should always exist*/
        joinNode(0, c, c->p[0], c->p[1]);
      }

      c = c->p[0];

      if (isFracInNode(f, c))
      {
        printf("Found %llu/%llu in node %d\n", f.a, f.b, ptr2loc(c));
        if (isLeaf(c)) {
          printf("%d is a leaf node\n", ptr2loc(c));
          index = deleteFromLeaf(f, c);
          deleted = 1;
        } 
        
        else { /*successor case */
          printf("NON LEAF NODE, need to find successor.\n");
          return -1;
        }
      }
    }
  }

  return index;
}


int main(void){
  
  /*const char* filename = "P1-tests/T03/input";*/
  /*freopen(filename, "r", stdin);*/

  size_t len = 256;
  char *line = (char*)malloc(len*sizeof(char));
  
  int tree_size; 
  char command;

  int index;
  struct frac f;
  int i ;

  getline(&line, &len, stdin);
  sscanf(line, "%d", &tree_size);  
  
  A = calloc(tree_size, sizeof(struct node234));
  S = &A[tree_size-1];
  root = NULL;


  for (i = 1; i < tree_size; i++){
    A[i].p[3] = &A[i-1];
  }

  while (sscanf(line, " %c", &command) != -1) {
    int i;
    getline(&line, &len, stdin);
    sscanf(line, "%c", &command); 

    switch (command) {

            case 'S':
                
                if (sscanf(line, "S %d", &index) == 1) {
                    showNode(&A[index]);
                }

                break;

            case 'F':
                
                if (sscanf(line, "F %llu/%llu", &f.a, &f.b) == 2) {
                    printf("%d\n", searchFrac(f));

                }

                break;

            case 'N':
                
                if (root != NULL){
                  inOrder(root);
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
                    index = insert(f);
                    if (index == -1) {
                        printf("Error inserting fraction %llu/%llu\n", f.a, f.b);
                    } else {
                        printf("%d\n", index);
                    }
                }

                break;

            case 'D':

                if (sscanf(line, "D %llu/%llu", &f.a, &f.b) == 2) {
                    index = delete(f);
                    printf("%d\n", index);
                }

                break;

            case 'L':
                structLoad();
                break;

            case 'X':
                /* printing final configuration: location of node S and root + all nodes*/
                
                printf("Final configuration:\n");
                printf("S: %d ", ptr2loc(S));
                printf("root: %d \n", ptr2loc(root));
                
                
                for (i = 0; i < tree_size; i++){
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

 

  /* debugging code, will be removed by preprocessor in mooshak: */
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


