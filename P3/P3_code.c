
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> 
#include <stdbool.h>
#include <string.h>
#include <assert.h>


typedef struct point* point;
typedef struct node* node;
node root;           // root node 
char* T;
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

    if (p->a == p->b){                          // if we are in a node then check child then brothers
        
        node bro = p->b->child;
        char next;
        
        while (bro != NULL){

            next = T[bro->head];

            if (next == t){
                return true;
            } else {
                bro = bro->brother;
            }
        }

        return false;

    } else {                                    // check one character
        return T[p->b->head + p->s] == t;
    }

}

void descend(point p, char t){

    if (descendQ(p, t)){
       
        if (p->a == p->b){                          // if we are in a node: move to first location in next branch 
        
            p->b = p->b->child;
            p->s = 1;

        } else if(p->s + 1 == p->b->sdep){          // if we are at the end of a branch (??? condition might be wrong): move to next node

            p->a = p->b;
            p->s = 0;

        } else {                                    // else if we are in the middle of a branch: increment string depth, move forward in branch
            p->s += 1;
        }
    }
}

int addLeaf(point p, node N, int i){

    if (p->a == p->b){
        node leaf;

        leaf->child = NULL;
        leaf->slink = NULL;

        return 1;

    } else {
        node internal;
        node leaf;

        p->a->child = internal;
        //internal->child = p->b;
        internal->child = leaf;
        leaf->brother = p->b;

        return 1;
    }

}

void suffixLink(point p){

}


// main function

int main(){
    
    int n;
    int i = 0;
    int a = 0;

    scanf("%d", &n);
    
    T = (char*)malloc(n*sizeof(char));
    scanf("%s", T);     

    while(i < n) {
        printf("Letter %c\n", '\0' == T[i] ? '$' : T[i]);
        
        while(!descendQ(p, T[i])) {
            a += addLeaf(p, &(root[a]), i);
            suffixLink(p);
            //j++;
        }

        descend(p, T[i]);
        i++;
    }

}