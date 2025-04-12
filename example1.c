/*Determina daca exista sau nu drum direct intre doua restaurante dintr-o retea de tip graf*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct Graph {
    int numNodes;
    bool *visited;
    Node **adjList;
} Graph;

typedef struct Stack {
    int top;
    int capacity;
    int *array;
} Stack;

Node *create_node(int data) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void add_edge(Graph *graph, int src, int dest) {
    Node *newNode = create_node(dest);
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;

    newNode = create_node(src);
    newNode->next = graph->adjList[dest];
    graph->adjList[dest] = newNode;
}

Graph *create_graph(int numNodes) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->numNodes = numNodes;
    graph->adjList = (Node **)malloc(numNodes * sizeof(Node *));
    graph->visited = (bool *)malloc(numNodes * sizeof(bool));

    for (int i = 0; i < numNodes; i++) {
        graph->adjList[i] = NULL;
        graph->visited[i] = false;
    }
    return graph;
}

Stack *create_stack(int capacity) {
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int *)malloc(capacity * sizeof(int));
    return stack;
}

void push(Stack *stack, int item) {
    stack->array[++stack->top] = item;
}

int pop(Stack *stack) {
    return stack->array[stack->top--];
}

void dfs(Graph *graph, int node, Stack *stack) {
    graph->visited[node] = true;
    printf("%d ", node);
    push(stack, node);

    Node *adjacentNodes = graph->adjList[node];
    Node *current = adjacentNodes;
    while (current != NULL) {
        int connectedNode = current->data;
        if (!graph->visited[connectedNode]) {
            dfs(graph, connectedNode, stack);
        }
        current = current->next;
    }
}

void insert_edges(Graph *graph, int numEdges, int numNodes) {
    int src, dest, i;
    printf("Adauga %d muchii (de la 1 la %d)\n", numEdges, numNodes);
    for (i = 0; i < numEdges; i++) {
        scanf("%d %d", &src, &dest);
        add_edge(graph, src - 1, dest - 1);
    }
}

void reset_visited(Graph *graph) {
    for (int i = 0; i < graph->numNodes; i++) {
        graph->visited[i] = false;
    }
}

bool can_reach(Graph *graph, int startNode, int endNode) {
    reset_visited(graph);
    Stack *stack = create_stack(graph->numNodes);
    push(stack, startNode);
    graph->visited[startNode] = true;

    while (stack->top != -1) {
        int currentNode = pop(stack);
        if (currentNode == endNode) {
            free(stack->array);
            free(stack);
            return true;
        }

        Node *adjacentNodes = graph->adjList[currentNode];
        Node *current = adjacentNodes;
        while (current != NULL) {
            int neighbor = current->data;
            if (!graph->visited[neighbor]) {
                graph->visited[neighbor] = true;
                push(stack, neighbor);
            }
            current = current->next;
        }
    }

    free(stack->array);
    free(stack);
    return false;
}

int main() {
    int numNodes, numEdges, startRestaurant, endRestaurant;

    printf("Introduceti numarul de restaurante (noduri): ");
    scanf("%d", &numNodes);

    Graph *graph = create_graph(numNodes);

    printf("Introduceti numarul de conexiuni directe (muchii): ");
    scanf("%d", &numEdges);

    insert_edges(graph, numEdges, numNodes);

    printf("Introduceti restaurantul de start (1 la %d): ", numNodes);
    scanf("%d", &startRestaurant);

    printf("Introduceti restaurantul destinatie (1 la %d): ", numNodes);
    scanf("%d", &endRestaurant);

    if (can_reach(graph, startRestaurant - 1, endRestaurant - 1)) {
        printf("Exista un drum intre restaurantul %d si restaurantul %d.\n", startRestaurant, endRestaurant);
    } else {
        printf("Nu exista un drum intre restaurantul %d si restaurantul %d.\n", startRestaurant, endRestaurant);
    }

    for (int i = 0; i < numNodes; i++) {
        Node *current = graph->adjList[i];
        while (current != NULL) {
            Node *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph->adjList);
    free(graph->visited);
    free(graph);

    return 0;
}