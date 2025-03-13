#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int src, dest, weight;
} Edge;

typedef struct {
    int V, E;
    Edge* edges;
} Graph;

typedef struct {
    int parent;
    int rank;
} Subset;

int compareEdges(const void* a, const void* b) {
    return ((Edge*)a)->weight - ((Edge*)b)->weight;
}

int find(Subset subsets[], int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}

void unionSets(Subset subsets[], int x, int y) {
    // Union by rank -> smaller rank tree gets attached to the larger rank tree
    int rootX = find(subsets, x);
    int rootY = find(subsets, y);

    if (subsets[rootX].rank < subsets[rootY].rank)
        subsets[rootX].parent = rootY;
    else if (subsets[rootX].rank > subsets[rootY].rank)
        subsets[rootY].parent = rootX;
    else {
        subsets[rootY].parent = rootX;
        subsets[rootX].rank++;
    }
}

void kruskalMST(Graph* graph, int budget) {
    int V = graph->V;
    Edge result[graph->V]; // MST is max the size of the graph
    int e = 0;  
    int i = 0; 
    int totalCost = 0;  

    qsort(graph->edges, graph->E, sizeof(graph->edges[0]), compareEdges);

    Subset subsets[graph->V]; // Max the size of the graph
    int subsetSize = V;

    for (int v = 0; v < V; v++) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    while (e < V - 1 && i < graph->E) {
        Edge nextEdge = graph->edges[i++];

        int x = find(subsets, nextEdge.src);
        int y = find(subsets, nextEdge.dest);

        // Cycle = same parent, so x == y
        if (x != y && (totalCost + nextEdge.weight <= budget)) {

            result[e++] = nextEdge;
            totalCost += nextEdge.weight;
            unionSets(subsets, x, y);
            subsetSize--; // Decrease the number of subsets every time we unionize them

        }
    }

    printf("%i\n",totalCost);
    printf("%i\n",subsetSize);
}

int main() {
    Graph graph;
    int budget;

    scanf("%d", &graph.V);
    scanf("%d", &budget);
    scanf("%d", &graph.E);

    graph.edges = (Edge*)malloc(graph.E * sizeof(Edge));
    for (int i = 0; i < graph.E; i++) {
        scanf("%d %d %d", &graph.edges[i].src, &graph.edges[i].dest, &graph.edges[i].weight);
        graph.edges[i].src--;  
        graph.edges[i].dest--;
    }

    kruskalMST(&graph, budget);

    free(graph.edges);
    return 0;
}
