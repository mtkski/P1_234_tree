
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


/* implemented functions:*/

int descendQ(point p, char t){ 

    if (p->a == p->b){                  /* if we are in a node or about to enter a node then check child then brothers */

        node bro = p->b->child;
        char next;
        
        while (bro != NULL){

            next = T[bro->head + p->a->sdep];
            
            if(bro->sdep == 0){           /* next is the root and we are in the sentinel */
                return 1;
            } else if (next == t){         /* next head is t, i.e we can descend with t */
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

    if (p->a == p->b){               /* if we are in a node, set b to next node */
        
        node bro = p->b->child;
        char next;
        
        while(bro != NULL){
            
            next = T[bro->head + p->a->sdep];
            
            if(bro->sdep == 0){           /* next is the root and we are in the sentinel */
                p->a = bro;
                p->b = bro;
                p->s = 0;
                break;
            } else if(next == t){
                p->b = bro;
                break;
            } else {
                bro = bro->brother;
            }
        }

    }
    
    if(p->s + 1 == (p->b->sdep - p->a->sdep)){      /* if we are at the end of a branch (??? condition might be wrong) or next node is root): move to next node */
        p->a = p->b;
        p->s = 0;

    } else if (p->a != p->b) {                         /* else if we are in the middle of a branch: increment string depth, move forward in branch */
        p->s += 1;
    }
}


int addLeaf(point p, int a, int j, int i){

    if (p->a == p->b){                              /* if p is inside node we need to add a leaf with missing path label T[i]*/
        node N = &(root[a]);
        
        if (p->b->child){                           /* if there are already children then rearange so new leaf is first in linked list*/
            p->b->child->hook = &(N->brother);
            N->brother = p->b->child; 
        }
        
        p->b->child = N;
        N->hook = &(p->b->child);

        N->head = i;  
        N->sdep = n + 1 - i;

        printf("Leaf ");
        shownode(N);

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

        newInternal = internal;
        
        printf("Leaf ");
        shownode(N);

        return 2;
    }

}

void suffixLink(point p, int a, int j, int i){

    int path = j + 1;
    int s = p->a->sdep + p->s - 1;

    if (lastInternalCreated != NULL){

        if (lastInternalCreated != newInternal){
            lastInternalCreated->slink = newInternal;
        } else {
            lastInternalCreated->slink = &(root[0]);
        }

        if(newInternal->sdep == p->s){          /* enter node */
            p->a = newInternal;
            p->b = newInternal;
            p->s = 0;
        }
    } 

    lastInternalCreated = newInternal;

    if(p->b->slink){
        p->a = p->b->slink;
        p->b = p->b->slink;

    } else {                            /* skip/count trick */
        
        p->a = &root[0];                /* go to root */
        p->b = &root[0];
        p->s = 0;

        while (p->a->sdep + p->s < s){               /* descend path to end of same substring with diff start */
            descend(p, T[path]);
            path++;
        }
        
    }

}


/*main function*/ 

int main(void){
    
    int j = 0;              /* start index of substring */
    int i = 0;              /* end index of subtring*/
    int a = 2;              /* position in root array, start after root node and sentinel */
    int LZ = 0;
    char format[10];                /* format string: only scan n characters into T */

    scanf("%d", &n);        
    
    T = (char*)malloc(n*sizeof(char));
    
    sprintf(format, "%%%ds", n);  
    scanf(format, T);     

    root = calloc(n + 2, sizeof(struct node)); /* Whole size of the tree */
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

        if (!descendQ(p, T[i])){
            printf("LZ-Block %.*s %c\n",  p->a->sdep + p->s, T + j, '\0' == T[i] ? '$' : T[i]);  
            LZ++;
        }
         
        while(!descendQ(p, T[i])) {                 /* check if we can descend with next character in T */                         
           
            a += addLeaf(p, a, j, i);
            suffixLink(p, a, j, i);
            j++;                                        /* next extension */

            /*printf("\n  \033[1;32msuffixLink moved p to:\033[0m\n");
            shownode(p->a);
            shownode(p->b);
            printf("    %d\n\n", p->s);*/

        }

        descend(p, T[i]);                           /* descend in tree */
        i++;                                            /* next phase */

        /*printf("\n  \033[1;32mdescend moved p to:\033[0m\n");
        shownode(p->a);
        shownode(p->b);
        printf("    %d\n\n", p->s);*/
    }

    printf("%d\n", LZ);
    printf("Final version\n");
    i = 0;

    while(i < a){
        shownode(&(root[i]));
        i++;
    }

}
