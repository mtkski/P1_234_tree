
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> 
#include <string.h>
#include <assert.h>


typedef struct point* point;
typedef struct node* node;
int n;
node root;           /* root node */
char *T;
point p;
int j = 0;
int LZ = 0;
int blockEnd = -1;

node lastInternalCreated; /*global variable to be implemented last*/
node newInternal;

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

/* help functions:*/

void findPath(point p, char t){
    
    if (p->a == p->b){               /* if we are in a node, set b to next node */
        
        node bro = p->b->child;
        char next;
        
        while(bro != NULL){
            
            next = T[bro->head + p->a->sdep];
            
            if(next == t || bro->sdep == 0){
                p->b = bro;
                break;
            } else {
                bro = bro->brother;
            }
        }

    } 
}

/* implemented functions:*/

int descendQ(point p, char t){ 

    if(p->a->sdep == -1){
        return 1;
    }

    if (p->a == p->b){                  /* if we are in a node or about to enter a node then check child then brothers */

        node bro = p->b->child;
        char next;
        
        while (bro != NULL){

            next = T[bro->head + p->a->sdep];
            
            if (next == t){         /* next head is t, i.e we can descend with t */
                return 1;
            } else {
                bro = bro->brother;        /* check brothers */
            }
        }

        return 0;

    } else {                            /* if we are in an edge then check next character */
        
        return T[p->b->head + p->a->sdep + p->s] == t;
    }

}

void descend(point p, char t){

    findPath(p, t); 
    
    if(p->s + 1 >= (p->b->sdep - p->a->sdep)){          /* if we are at the end of a branch: move to next node */
        p->a = p->b;
        p->s = 0;

    } else if (p->a != p->b) {                          /* else if we are in the middle of a branch: increment string depth, move forward in branch */
        p->s += 1;
    }
}


int addLeaf(point p, int a, int j, int i){
    
    if(j == blockEnd + 1){
        printf("LZ-Block %.*s %c\n",  p->a->sdep + p->s, T + j, '\0' == T[i] ? '$' : T[i]);  
        LZ++;
        blockEnd = i;
    }
    
    /*printf("\n  \033[1;32m addLeaf at: \n");
    shownode(p->a);
    shownode(p->b);
    printf("    %d\n \033[0m \n", p->s);*/

    if (p->a == p->b || p->s == (p->b->sdep - p->a->sdep)){                              /* if p is inside node we need to add a leaf with missing path label T[i]*/
        node N = &(root[a]);
        
        if (p->b->child){                           /* if there are already children then rearange so new leaf is first in linked list*/
            p->b->child->hook = &(N->brother);
            N->brother = p->b->child; 
        }
        
        p->b->child = N;
        N->hook = &(p->b->child);

        N->head = i - p->b->sdep;  
        N->sdep = n + 1 - i + p->b->sdep;

        printf("Leaf ");
        shownode(N);

        if(lastInternalCreated != NULL){
            lastInternalCreated->slink = p->a;
            /*printf("\n      addLeaf slinked: \n");
            shownode(lastInternalCreated);*/
        }

        lastInternalCreated = NULL;

        return 1;

    } else {
        
        node internal = &(root[a]);       
        node N = &(root[a+1]);

        internal->head = p->b->head;
        internal->sdep = p->a->sdep + p->s;
        

        *(p->b->hook) = internal;
        internal->hook = p->b->hook;

       
        internal->child = p->b;
        internal->brother = p->b->brother;


        if(internal->brother){
            *(internal->brother->hook) = NULL;
            internal->brother->hook = &internal->brother;
        } 

        printf("Internal ");
        shownode(internal);
        
        N->hook = &(internal->child);
        internal->child = N;
 
        p->b->hook = &(N->brother);
        N->brother = p->b;

        N->head = j;
        N->sdep = internal->sdep + (n - i) + 1;
        
        if (lastInternalCreated != NULL) {
            lastInternalCreated->slink = internal;
            /*printf("\n      addInternal slinked: \n");
            shownode(lastInternalCreated);*/
        }
        
        lastInternalCreated = internal;
        
        printf("Leaf ");
        shownode(N);

        return 2;
    }

}

void suffixLink(point p, int i){

    int h = j + p->a->sdep;
    int g = i - j + 1 - p->a->sdep;
    int G;

    if(p->a->slink){
        p->b = p->a->slink;
        p->a = p->a->slink;
        p->s = 0;

    } else {                            /* skip/count trick */
        
        p->a = &root[0];                /* go to root */
        p->b = &root[0];
        p->s = 0;
        
    }

    findPath(p, T[h]);
    G = p->b->sdep - p->a->sdep;

    /*printf("\n \033[0;35m g = %d, G = %d, h = %d \033[0m \n", g, G, h);*/

    while (G < g){               /* descend path to end of same substring with diff start */
        
        p->a = p->b;
        
        g = g - G;
        h = h + G;
        
        if(!descendQ(p, T[h])){
            return;
        }

        findPath(p, T[h]);
        
        G = p->b->sdep - p->a->sdep;
        /*printf("\n \033[0;35m g = %d, G = %d, h = %d \033[0m \n", g, G, h);*/
    }

    if(g - 1 == G){
        p->a = p->b;
        p->s = 0;
    } else {
        p->s = g - 1;
    }

}


/*main function*/ 

int main(void){

    int i = 0;              /* end index of subtring*/
    int a = 2;              /* position in root array, start after root node and sentinel */
    char format[10];                /* format string: only scan n characters into T */

    scanf("%d", &n);        
    
    T = (char*)malloc(n*sizeof(char));
    
    sprintf(format, "%%%ds", n);  
    scanf(format, T);     

    root = calloc(2*n + 2, sizeof(struct node)); /* Whole size of the tree */
    p = (point)malloc(sizeof(struct point));
    
    root[0].slink = &root[1];       /* the slink of the root is the sentinel */
    root[1].child = &root[0];       /* we can descend from the sentinel to the root */
    root[0].sdep = 0;
    root[1].sdep = -1;

    p->a = &root[0];                /* go to root */
    p->b = &root[0];
    p->s = 0;

    while(i < n + 1) {                                  /* i is end index of substring */
        
        printf("Letter %c\n", '\0' == T[i] ? '$' : T[i]);
        /*printf("\033[0;31m j = %d, i = %d \033[0m \n", j, i);*/

        lastInternalCreated = NULL;     /* Reset at start of phase */
                                                                      
        while(!descendQ(p, T[i])) {                     /* check if we can descend with next character in T */                         
            /*printf("\033[0;31m j = %d, i = %d \033[0m \n\n", j, i);*/

            a += addLeaf(p, a, j, i);
            suffixLink(p, i);
            j++;                                        /* next extension */

            /*printf("\n  \033[1;32m suffixLink moved p to: \n");
            shownode(p->a);
            shownode(p->b);
            printf("    %d\n \033[0m  \n", p->s);*/

        }
        
        if(lastInternalCreated != NULL){  
            lastInternalCreated->slink = p->a;
            /*printf("\n      main slinked: \n");
            shownode(lastInternalCreated);*/
        }

        descend(p, T[i]);                               /* descend in tree */
        i++;                                            /* next phase */

        /*printf("\n  \033[1;32m descend moved p to: \n");
        shownode(p->a);
        shownode(p->b);
        printf("    %d\n \033[0m \n", p->s);*/


    }

    printf("%d\n", LZ);
    printf("Final version\n");
    i = 0;

    while(i < a){
        shownode(&(root[i]));
        i++;
    }

    free(T);
    free(root);
    free(p);

}
