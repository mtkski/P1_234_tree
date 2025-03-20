
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> 
#include <stdbool.h>
#include <string.h>
#include <assert.h>


typedef struct point* point;
typedef struct node* node;
struct node234* root;          // root node 
char T;

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

            next = T[&bro->head];

            if (next == t){
                return true;
            } else {
                bro = bro->brother;
            }
        }

        return false;

    } else {                                    // check one character
        return T[&p->b->head + p->s] == t;
    }

}

void descend(point p, char t){

    if (descendQ(p, t)){
       
        if (p->a == p->b){                          // move to first location in next branch
        
            p->b = p->b->child;
            p->s = 1;

        } else {                                    // increment string depth
            p->s += 1;
        }
    }
}

void addLeaf(point p, /* &(root[a]), */int i){

}

void suffixLink(point p){

}


// main function

void main(){
    
    int n;

    scanf("%d", n);
    
    T = (char*)malloc(n*sizeof(char));
    scanf("%s\0", T);

    int i = 0;

    while(i < n) {
        printf("Letter %c\n", "\0" == T[&i] ? "$" : T[&i]);
        
        while(!DescendQ(p, T[i])) {
            a += AddLeaf(p, &(root[a]), i);
            SuffixLink(p);
            j++;
        }

        Descend(p, T[i]);
        i++;
    }

}