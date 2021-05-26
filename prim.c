#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_VAlUE 1000
typedef long long int lli;

struct Adjacency_list
{
    lli item;
    lli weight;
    Adjacency_list *next;
};typedef struct Adjacency_list Adjacency_list;

struct Vertex
{
    lli index;
    lli index_heap;
    lli cost;
    lli previous;
    int visited;

    Adjacency_list *adjacency_list;
};typedef struct Vertex Vertex;

struct Graph
{
    Vertex **vertex;
    lli vertex_quantity;
};typedef struct Graph Graph;

typedef struct HEAP HEAP;
struct HEAP {
    lli size;
    lli max_size;
    struct Vertex **vertex;

};

int get_parent_index(int i)
{
    return (i / 2);
}

int get_left_index(int i)
{
    return (2 * i);
}

int get_right_index(int i)
{
    return (2 * i + 1);
}

void swap_vertex(void **item_1, void **item_2)
{
    void *aux = *item_1;
    *item_1 = *item_2;
    *item_2 = aux;
}

lli get_cost(HEAP *h, int index)
{
    return ((Vertex *) h->vertex[index])->cost;
}

void enqueue(HEAP *heap, Vertex *newVertex)
{
    if (heap->size >= heap->max_size - 1)
    {
        printf("Heap overflow.\n");
    }
    else
    {
        heap->vertex[++heap->size] = newVertex;
        newVertex->index_heap = heap->size;


        int key_index = heap->size;

        int parent_index = get_parent_index(key_index);

        while (parent_index >= 1 && get_cost(heap, key_index) < get_cost(heap, parent_index))
        {
            heap->vertex[parent_index]->index_heap = key_index;
            heap->vertex[key_index]->index_heap = parent_index;
            swap_vertex((void *) &heap->vertex[parent_index], (void *) &heap->vertex[key_index]);

            key_index = parent_index;

            parent_index = get_parent_index(key_index);
        }
    }
}

void min_heapify(HEAP *heap, int i)
{
    int smallest;
    int left_index = get_left_index(i);
    int right_index = get_right_index(i);

    Vertex *t = heap->vertex[i];
    Vertex *t_l = heap->vertex[left_index];
    Vertex *t_r = heap->vertex[right_index];

    if (left_index <= heap->size && t_l->cost < t->cost)
    {
        smallest = left_index;
    }
    else
    {
        smallest = i;
    }

    Vertex *t_s = heap->vertex[smallest];

    if (right_index <= heap->size && t_r->cost < t_s->cost)
    {
        smallest = right_index;
        t_s = heap->vertex[smallest];
    }

    if (t != t_s)
    {
        heap->vertex[i]->index_heap = smallest;
        heap->vertex[smallest]->index_heap = i;
        swap_vertex((void *) &heap->vertex[i], (void *) &heap->vertex[smallest]);
        min_heapify(heap, smallest);
    }
}

void bubble_up(HEAP *heap, int i) {
    if(i == 0) {
        return;
    }

    int parent_index = get_parent_index(i);
    if(heap->vertex[parent_index]->cost > heap->vertex[i]->cost) {
        heap->vertex[parent_index]->index_heap = i;
        heap->vertex[i]->index_heap = parent_index;
        swap_vertex((void *) &heap->vertex[parent_index], (void *) &heap->vertex[i]);
        bubble_up(heap, parent_index);
    }
}

void *dequeue(HEAP *heap)
{
    if (!(heap->size))
    {
        printf("Heap underflow.\n");
        return NULL;
    }
    else
    {
        Vertex *item = (Vertex *) heap->vertex[1];

        heap->vertex[1] = heap->vertex[heap->size];
        heap->vertex[1]->index_heap = 1;

        heap->vertex[heap->size] = NULL;

        heap->size -= 1;

        min_heapify(heap, 1);

        return item;
    }
}

void free_heap(HEAP *heap)
{
    for (int i = 1; i <= heap->size; ++i)
    {
        free((Vertex *) heap->vertex[i]);
    }

    free(heap);
}
HEAP *create_heap(int size, Vertex **vertex_list)
{
    HEAP *heap = (HEAP *) malloc(sizeof(HEAP));

    heap->vertex = (Vertex **) malloc((size + 1) * sizeof (Vertex *));

    heap->size = 0;
    heap->max_size = size + 1;

    for (int i = 0; i < size; ++i)
    {
        enqueue(heap, vertex_list[i]);
    }

    return heap;
}

Graph *prim_algorithm (Graph *graph, lli vertice_origin) {
    Vertex **vertex = graph->vertex;
    vertex[vertice_origin]->cost = 0;
    vertex[vertice_origin]->previous = vertice_origin;

    HEAP *heap = create_heap(graph->vertex_quantity, vertex);

    while (heap->size > 0) {
        Vertex *actual = dequeue(heap);
        actual->visited = 1;
        Adjacency_list *node = actual->adjacency_list;

        while (node != NULL) {          //enquanto houverem vizinhos
            Vertex *neighbor = vertex[node->item];
            if (neighbor->visited == 0) {         //se não foi visitado
                if (neighbor->cost > node->weight) {     //se o custo do vizinho é maior que o peso da aresta entre eles
                    neighbor->cost = node->weight;
                    neighbor->previous = actual->index;

                    bubble_up(heap, neighbor->index_heap);
                }
            }
            node = node->next;
        }
    }
    return graph;
}

void print_MST(Vertex **vertex, lli size) {
    lli total_cost = 0;
    for (lli i = 0; i < size; i++) {
        if(vertex[i]->index != vertex[i]->previous) {
            printf("%lld %lld\n", vertex[i]->index, vertex[i]->previous);
        }
        total_cost += vertex[i]->cost;
    }
    printf("Total Cost: %lld\n", total_cost);
}
Vertex *create_vertex(lli index) {
    Vertex *new_vertex = (Vertex *) malloc(sizeof(Vertex));
    new_vertex->index = index;
    new_vertex->previous = -1;
    new_vertex->visited = 0;
    new_vertex->cost = LLONG_MAX;
    new_vertex->adjacency_list = NULL;

    return  new_vertex;
}

Graph *create_graph(lli vertex_quantity) {
    Graph *graph = (Graph *) malloc(sizeof(Graph));
    graph->vertex_quantity = vertex_quantity;

    graph->vertex = (Vertex **) malloc(vertex_quantity * sizeof (Vertex *));
    for(lli i = 0; i < vertex_quantity; i++) {
        graph->vertex[i] = create_vertex(i);
    }

    return graph;
}


Adjacency_list *create_node(lli item, lli weight) {
    Adjacency_list *new_adjacency_list = (Adjacency_list *) malloc(sizeof(Adjacency_list));
    new_adjacency_list->item = item;
    new_adjacency_list->weight = weight;
    new_adjacency_list->next = NULL;

    return new_adjacency_list;
}


Adjacency_list* begin_add(Adjacency_list *head, lli item, lli weight)
{
    Adjacency_list *new_node = create_node(item, weight);
    new_node->next = head;
    head = new_node;

    return head;
}

Graph *add_weighted_simple_edge (Graph *graph, lli vertex1, lli vertex2, lli weight) {

    graph->vertex[vertex1]->adjacency_list = begin_add(graph->vertex[vertex1]->adjacency_list, vertex2, weight);
    graph->vertex[vertex2]->adjacency_list = begin_add(graph->vertex[vertex2]->adjacency_list, vertex1, weight);

    return graph;
}

Graph *get_graph() {

    FILE *in = fopen("../input", "r");

    // verifica se o arquivo existe
    if (!in) {
        printf("Erro! Nao foi possivel abrir o arquivo.\n");
        exit(-1);
    }

    lli n;
    fscanf(in, "%lli", &n);

    Graph *graph = create_graph(n);

    lli v1, v2, w;

    while (fscanf(in, "%lli", &v1) != EOF) {
        fscanf(in, "%lli %lli", &v2, &w);
        graph = add_weighted_simple_edge(graph, v1, v2, w);
    }

    return graph;
}

int main() {
    Graph *graph = get_graph();

    graph = prim_algorithm(graph, 0);

    print_MST(graph->vertex, graph->vertex_quantity);

    return 0;
}
