
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> 
#include <stdbool.h>
#include <string.h>
#include <assert.h>


typedef struct point* point;
typedef struct node* node;
int n;
node root;           // root node 
char *T;
point p;

/* lastInternalCreated (global variable to be implemented last) */

struct point {
    node a; /**< node above */
    node b; /**< node bellow */
    int s; /**< String-Depth */
    };
    
struct node
{
int head; /**< The path-label start at &(Ti[head]) */
int sdep; /**< String-Depth, from the root down to node */
node child; /**< First child */
node brother; /**< Brother */
node slink; /**< Suffix-Link */
node* hook; /**< What keeps this node linked? */
};

int ptr2loc(node v, node A)
{
    int r;
    r = -1;
    if(NULL != v)
        r = ((size_t) v - (size_t) A) / sizeof(struct node);
    return (int)r;
}

void shownode(node v)
{
    if(NULL == v)
        printf("NULL\n");
    else {
        printf("node: %d ", ptr2loc(v, root));
        printf("head: %d ", v->head);
        printf("sdep: %d ", v->sdep);
        printf("child: %d ", ptr2loc(v->child, root));
        printf("brother: %d ", ptr2loc(v->brother, root));
        printf("slink: %d", ptr2loc(v->slink, root));
        printf("\n");
    }
}

// help functions:


// implemented functions:

bool descendQ(point p, char t){ 

    if (p->a == p->b){                  /* if we are in a node then check child then brothers */
        
        node bro = p->b->child;
        char next;
        
        while (bro != NULL){

            next = T[bro->head];
            
            if(bro->sdep == 0){           /* next is the root and we are in the sentinel */
                return true;
            } else if (next == t){         /* next head is t, i.e we can descend with t */
                return true;
            } else {
                bro = bro->brother;        /* check brothers */
            }
        }

        return false;

    } else {                            /* if we are in an edge then check next character */
        
        return T[p->b->head + p->s] == t;
    }

}

void descend(point p, char t){

    if (p->a == p->b){               /* if we are in a node, set b to next node */
        p->b = p->b->child;          
    }
    
    if(p->s + 1 == p->b->sdep || p->b->sdep == 0){      /* if we are at the end of a branch (??? condition might be wrong) or next node is root): move to next node */

        p->a = p->b;
        p->s = 0;

    } else {                         /* else if we are in the middle of a branch: increment string depth, move forward in branch */
        p->s += 1;
    }
}


int addLeaf(point p, int a, int j){

    node N = &(root[a]);

    if (p->a == p->b){

        p->a->child = N;

        N->head = j;
        N->child = NULL;
        N->slink = NULL;    
        N->sdep = n + 1 - N->head;

        printf("Leaf ");
        shownode(N);

        return 1;

    } else {
        
        node internal = &(root[a+1]);

        internal->head = p->b->head;
        p->a->child = internal;
        //internal->child = p->b;
        internal->child = N;
        internal->sdep = p->s;

        N->head = j;
        N->brother = p->b;
        N->sdep = n + 1 - N->head;

        printf("Internal ");
        shownode(internal);
        printf("Leaf ");
        shownode(N);

        return 2;
    }

}

void suffixLink(point p){

    if (p->a == p->b && p->a->child == NULL){
        p->a->slink;
    } // ???

}


// main function

int main(){
    
    int j;
    int i = 0;              /* position in string T */
    int a = 2;              /* position in root array, start after root node and sentinel */

    scanf("%d", &n);
    
    T = (char*)malloc(n*sizeof(char));
    scanf("%s", T);     

    root = calloc(n + 2, sizeof(struct node));
    p = (point)malloc(sizeof(struct point));
    
    root[0].slink = &root[1];       /* the slink of the root is the sentinel */
    root[1].child = &root[0];       /* we can descend from the sentinel to the root */
    root[1].sdep = -1;
    
    p->a = &root[i];                /* go to root */
    p->b = &root[i];
    p->s = 0;

    while(i < n + 1) {

        printf("Letter %c\n", '\0' == T[i] ? '$' : T[i]);

        j = i;
        
        while(!descendQ(p, T[i])) {
            a += addLeaf(p, a, j);
            suffixLink(p);
            j++;
        }

        descend(p, T[i]);
        i++;
    }

    printf("Final version\n");
    i = 0;

    while(i < a){
        shownode(&(root[i]));
        i++;
    }

}