/*Implementarea parcurgerii grafurilor cu DFS (Depth-First Search) si BFS (Breadth-First Search)*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct Graph {
    int num_vertices;
    bool *visited;
    Node **adjacency_list;
} Graph;

Node *create_node(int vertex) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        perror("Eroare la alocarea memoriei pentru nod");
        return NULL;
    }
    new_node->data = vertex;
    new_node->next = NULL;
    return new_node;
}

Graph *create_graph(int num_vertices) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    if (graph == NULL) {
        perror("Eroare la alocarea memoriei pentru graf");
        return NULL;
    }
    graph->num_vertices = num_vertices;
    graph->adjacency_list = (Node **)malloc(num_vertices * sizeof(Node *));
    if (graph->adjacency_list == NULL) {
        perror("Eroare la alocarea memoriei pentru listele de adiacenta");
        free(graph);
        return NULL;
    }
    graph->visited = (bool *)malloc(num_vertices * sizeof(bool));
    if (graph->visited == NULL) {
        perror("Eroare la alocarea memoriei pentru vizitat");
        free(graph->adjacency_list);
        free(graph);
        return NULL;
    }

    for (int i = 0; i < num_vertices; i++) {
        graph->adjacency_list[i] = NULL;
        graph->visited[i] = false;
    }
    return graph;
}

bool add_edge(Graph *graph, int source, int destination) {
    if (graph == NULL || source < 0 || source >= graph->num_vertices || destination < 0 || destination >= graph->num_vertices) {
        fprintf(stderr, "Indici de noduri invalizi pentru adaugarea muchiei.\n");
        return false;
    }

    Node *new_node_src = create_node(destination);
    if (new_node_src == NULL) {
        return false;
    }
    new_node_src->next = graph->adjacency_list[source];
    graph->adjacency_list[source] = new_node_src;

    Node *new_node_dest = create_node(source);
    if (new_node_dest == NULL) {
        Node *temp = graph->adjacency_list[source];
        if (temp != NULL && temp->data == destination) {
            graph->adjacency_list[source] = temp->next;
            free(temp);
        }
        free(new_node_src);
        return false;
    }
    new_node_dest->next = graph->adjacency_list[destination];
    graph->adjacency_list[destination] = new_node_dest;

    return true;
}

bool input_edges(Graph *graph, int num_edges) {
    if (graph == NULL || num_edges < 0) {
        fprintf(stderr, "Parametri invalizi pentru citirea muchiilor.\n");
        return false;
    }

    int source, destination;
    printf("Adauga %d muchii (nodurile sunt numerotate de la 0 la %d):\n", num_edges, graph->num_vertices - 1);
    for (int i = 0; i < num_edges; i++) {
        printf("Muchia %d (sursa destinatie): ", i + 1);
        if (scanf("%d %d", &source, &destination) != 2) {
            fprintf(stderr, "Introducere invalida pentru muchie.\n");
            return false;
        }
        if (source < 0 || source >= graph->num_vertices || destination < 0 || destination >= graph->num_vertices) {
            fprintf(stderr, "Nod invalid introdus.\n");
            return false;
        }
        if (!add_edge(graph, source, destination)) {
            fprintf(stderr, "Eroare la adaugarea muchiei %d.\n", i + 1);
            return false;
        }
    }
    return true;
}

typedef struct QueueNode {
    int data;
    struct QueueNode *next;
} QueueNode;

QueueNode *create_queue_node(int data) {
    QueueNode *new_node = (QueueNode *)malloc(sizeof(QueueNode));
    if (new_node == NULL) {
        perror("Eroare la alocarea memoriei pentru nodul cozii");
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

bool is_queue_empty(QueueNode *front) {
    return front == NULL;
}

bool enqueue(QueueNode **front, QueueNode **rear, int data) {
    QueueNode *new_node = create_queue_node(data);
    if (new_node == NULL) {
        return false;
    }
    if (is_queue_empty(*front)) {
        *front = new_node;
        *rear = new_node;
    } else {
        (*rear)->next = new_node;
        *rear = new_node;
    }
    return true;
}

int dequeue(QueueNode **front) {
    if (is_queue_empty(*front)) {
        fprintf(stderr, "Coada este goala, nu se poate extrage.\n");
        return -1;
    }
    QueueNode *temp = *front;
    int data = temp->data;
    *front = (*front)->next;
    free(temp);
    return data;
}

void reset_visited(Graph *graph) {
    if (graph != NULL) {
        for (int i = 0; i < graph->num_vertices; i++) {
            graph->visited[i] = false;
        }
    }
}

void DFS(Graph *graph, int vertex) {
    if (graph == NULL || vertex < 0 || vertex >= graph->num_vertices || graph->visited == NULL) {
        fprintf(stderr, "Parametri invalizi pentru DFS.\n");
        return;
    }
    graph->visited[vertex] = true;
    printf("%d ", vertex);

    Node *adjacent = graph->adjacency_list[vertex];
    while (adjacent != NULL) {
        if (!graph->visited[adjacent->data]) {
            DFS(graph, adjacent->data);
        }
        adjacent = adjacent->next;
    }
}

void BFS(Graph *graph, int start_vertex) {
    if (graph == NULL || start_vertex < 0 || start_vertex >= graph->num_vertices || graph->visited == NULL) {
        fprintf(stderr, "Parametri invalizi pentru BFS.\n");
        return;
    }

    QueueNode *front = NULL;
    QueueNode *rear = NULL;

    graph->visited[start_vertex] = true;
    if (!enqueue(&front, &rear, start_vertex)) {
        fprintf(stderr, "Eroare la adaugarea nodului de start in coada pentru BFS.\n");
        return;
    }

    while (!is_queue_empty(front)) {
        int current_vertex = dequeue(&front);
        printf("%d ", current_vertex);

        Node *adjacent = graph->adjacency_list[current_vertex];
        while (adjacent != NULL) {
            int adjacent_vertex = adjacent->data;
            if (!graph->visited[adjacent_vertex]) {
                graph->visited[adjacent_vertex] = true;
                if (!enqueue(&front, &rear, adjacent_vertex)) {
                    fprintf(stderr, "Eroare la adaugarea nodului adiacent in coada pentru BFS.\n");
                    return;
                }
            }
            adjacent = adjacent->next;
        }
    }
    while (!is_queue_empty(front)) {
        dequeue(&front);
    }
}

void free_graph(Graph *graph) {
    if (graph == NULL) {
        return;
    }
    if (graph->adjacency_list != NULL) {
        for (int i = 0; i < graph->num_vertices; i++) {
            Node *current = graph->adjacency_list[i];
            while (current != NULL) {
                Node *temp = current;
                current = current->next;
                free(temp);
            }
        }
        free(graph->adjacency_list);
    }
    if (graph->visited != NULL) {
        free(graph->visited);
    }
    free(graph);
}

int main() {
    int num_vertices;
    int num_edges;
    int start_vertex_dfs, start_vertex_bfs;
    Graph *graph = NULL;
    bool input_success;

    printf("Introduceti numarul de noduri ale grafului: ");
    if (scanf("%d", &num_vertices) != 1 || num_vertices <= 0) {
        fprintf(stderr, "Numar invalid de noduri.\n");
        return 1;
    }

    printf("Introduceti numarul de muchii ale grafului: ");
    if (scanf("%d", &num_edges) != 1 || num_edges < 0) {
        fprintf(stderr, "Numar invalid de muchii.\n");
        return 1;
    }

    graph = create_graph(num_vertices);
    if (graph == NULL) {
        fprintf(stderr, "Eroare la crearea grafului.\n");
        return 1;
    }

    input_success = input_edges(graph, num_edges);
    if (!input_success) {
        fprintf(stderr, "Eroare la introducerea muchiilor.\n");
        free_graph(graph);
        return 1;
    }

    printf("Introduceti nodul de start pentru DFS (0 la %d): ", num_vertices - 1);
    if (scanf("%d", &start_vertex_dfs) != 1 || start_vertex_dfs < 0 || start_vertex_dfs >= num_vertices) {
        fprintf(stderr, "Nod de start invalid pentru DFS.\n");
        free_graph(graph);
        return 1;
    }

    printf("Parcurgere DFS incepand cu nodul %d: ", start_vertex_dfs);
    DFS(graph, start_vertex_dfs);
    printf("\n");

    reset_visited(graph);

    printf("Introduceti nodul de start pentru BFS (0 la %d): ", num_vertices - 1);
    if (scanf("%d", &start_vertex_bfs) != 1 || start_vertex_bfs < 0 || start_vertex_bfs >= num_vertices) {
        fprintf(stderr, "Nod de start invalid pentru BFS.\n");
        free_graph(graph);
        return 1;
    }

    printf("Parcurgere BFS incepand cu nodul %d: ", start_vertex_bfs);
    BFS(graph, start_vertex_bfs);
    printf("\n");

    free_graph(graph);

    return 0;
}