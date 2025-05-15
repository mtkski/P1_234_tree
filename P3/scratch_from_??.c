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

        if (p->a != &root[0] && p->b != &root[0]){
            p->a = &root[1];                          /* go to sentinel */
            p->b = &root[1];
            p->s = 0;
        }
    }