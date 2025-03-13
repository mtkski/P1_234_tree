#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct point* point;
typedef struct node* node;

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
