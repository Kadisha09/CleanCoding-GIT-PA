#include <stdio.h>
#include <stdlib.h>

// utils

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct Graph {
    int vertices;
    int *visited;
    Node **adjacency_lists;
} Graph;

Node *create_node(int vertex) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memorie insuficienta pentru crearea unui nod.\n");
        exit(1);
    }
    new_node->data = vertex;
    new_node->next = NULL;
    return new_node;
}

Graph *create_graph(int vertex_count) {
    Graph *graph = malloc(sizeof(Graph));
    if (graph == NULL) {
        fprintf(stderr, "Memorie insuficienta pentru crearea grafului.\n");
        exit(1);
    }

    graph->vertices = vertex_count;
    graph->adjacency_lists = malloc(vertex_count * sizeof(Node *));
    graph->visited = malloc(vertex_count * sizeof(int));

    if (graph->adjacency_lists == NULL || graph->visited == NULL) {
        fprintf(stderr, "Memorie insuficienta pentru listele de adiacenta sau lista de vizitate.\n");
        exit(1);
    }

    for (int i = 0; i < vertex_count; i++) {
        graph->adjacency_lists[i] = NULL;
        graph->visited[i] = 0;
    }

    return graph;
}

void add_edge(Graph *graph, int source, int destination) {
    Node *new_node = create_node(destination);
    new_node->next = graph->adjacency_lists[source];
    graph->adjacency_lists[source] = new_node;

    new_node = create_node(source);
    new_node->next = graph->adjacency_lists[destination];
    graph->adjacency_lists[destination] = new_node;
}

void read_edges(int edge_count, Graph *graph) {
    int source, destination;
    printf("Introduceti %d muchii (de la 0 la %d):\n", edge_count, graph->vertices - 1);

    for (int i = 0; i < edge_count; i++) {
        scanf("%d %d", &source, &destination);
        add_edge(graph, source, destination);
    }
}

// BFS utils

int is_empty(Node *queue) {
    return queue == NULL;
}

void enqueue(Node **queue, int data) {
    Node *new_node = create_node(data);
    if (is_empty(*queue)) {
        *queue = new_node;
    } else {
        Node *temp = *queue;
        while (temp->next) temp = temp->next;
        temp->next = new_node;
    }
}

int dequeue(Node **queue) {
    if (is_empty(*queue)) {
        fprintf(stderr, "Coada este goala!\n");
        exit(1);
    }

    int data = (*queue)->data;
    Node *temp = *queue;
    *queue = (*queue)->next;
    free(temp);
    return data;
}

// Parcurgeri

void reset_visited(Graph *graph) {
    for (int i = 0; i < graph->vertices; i++) {
        graph->visited[i] = 0;
    }
}

void dfs(Graph *graph, int vertex) {
    Node *adj_list = graph->adjacency_lists[vertex];
    graph->visited[vertex] = 1;
    printf("%d ", vertex);

    while (adj_list != NULL) {
        int connected_vertex = adj_list->data;
        if (!graph->visited[connected_vertex]) {
            dfs(graph, connected_vertex);
        }
        adj_list = adj_list->next;
    }
}

void bfs(Graph *graph, int start_vertex) {
    Node *queue = NULL;
    graph->visited[start_vertex] = 1;
    enqueue(&queue, start_vertex);

    while (!is_empty(queue)) {
        int current_vertex = dequeue(&queue);
        printf("%d ", current_vertex);

        Node *temp = graph->adjacency_lists[current_vertex];
        while (temp) {
            int adj_vertex = temp->data;
            if (!graph->visited[adj_vertex]) {
                graph->visited[adj_vertex] = 1;
                enqueue(&queue, adj_vertex);
            }
            temp = temp->next;
        }
    }
}

int main() {
    int vertex_count, edge_count, start_vertex;

    printf("Cate noduri are graful? ");
    scanf("%d", &vertex_count);

    printf("Cate muchii are graful? ");
    scanf("%d", &edge_count);

    Graph *graph = create_graph(vertex_count);
    read_edges(edge_count, graph);

    printf("Introduceti nodul de start pentru DFS: ");
    scanf("%d", &start_vertex);
    printf("Parcurgere cu DFS: ");
    dfs(graph, start_vertex);
    printf("\n");

    reset_visited(graph);

    printf("Introduceti nodul de start pentru BFS: ");
    scanf("%d", &start_vertex);
    printf("Parcurgere cu BFS: ");
    bfs(graph, start_vertex);
    printf("\n");

    free(graph->visited);
    free(graph->adjacency_lists);
    free(graph);

    return 0;
}
