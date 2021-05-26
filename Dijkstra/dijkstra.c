#include <stdio.h>


struct grafo{
    int eh_ponderado;
    int eh_digrafo;
    int nro_vertices;
    int grau_max;
    int** arestas;
    int** pesos;
    int* grau;
};typedef struct grafo Grafo;

int procura_menor_distancia(int *dist, int *visitado, int NV){
    int i, menor = -1, primeiro = 1;
    for(i = 0; i < NV; i++){
        if(dist[i] >= 0 && visitado[i] == 0){
            if(primeiro){
                menor = i;
                primeiro = 0;
            }else{
                if(dist[menor] > dist[i]){
                    menor = i;
                }
            }
        }
    }

    return menor;
}

void dijkstra(Grafo *gr, int inicial, int *ant, int *dist){
    int i, cont, NV, ind, *visitado, u;

    cont = NV = gr->nro_vertices;

    visitado = (int*) malloc(NV * sizeof(int));

    for(i = 0; i < NV; i++){
        ant[i] = -1;
        dist[i] = -1;
        visitado[i] = 0;
    }

    dist[inicial] = 0;

    while(cont > 0){
        u = procura_menor_distancia(dist, visitado, NV);

        if(u == -1){
            break;
        }

        visitado[u] = 1;
        cont--;

        for(i = 0; i < gr->grau[u]; i++){
            ind = gr->arestas[u][i];
            if(dist[ind] < 0){
                dist[ind] = dist[u] + gr->pesos[u][i];

                ant[ind] = u;
            }else{
                if(dist[ind] > dist[u] + gr->pesos[u][i]){
                    dist[ind] = dist[u] + gr->pesos[u][i];

                    ant[ind] = u;
                }
            }
        }
    }

    free(visitado);
}


int encontra_caminho(int origem, int objetivo, int n_vertices, int *caminho, int *ant, int cont) {
    int i;
    for(i = 0; i < n_vertices; i++){
        caminho[i] = -1;
    }

    while(objetivo != origem) {
        if(cont < 0 || objetivo < 0) {
            printf("Não existe caminho\n");
            return 0;
        }
        caminho[cont] = objetivo;
        objetivo = ant[objetivo];
        cont--;
    }

    return 1;
}

void imprime_caminho(int origem, int n_vertices, int *caminho) {
    int i;
    printf("%d ->", origem);
    for(i = 0; i < n_vertices; i++){
        if(caminho[i] != -1){
            printf(" %d", caminho[i]);
            if(i + 1 != n_vertices){
                printf(" ->");   
            }
        }
    }
    printf("\n");
}

Grafo *cria_grafo(int nro_vertices, int grau_max, int eh_ponderado, int eh_digrafo){
    Grafo *gr = (Grafo*) malloc(sizeof(struct grafo));

    if(gr != NULL){
        int i, j;
        gr->nro_vertices = nro_vertices;
        gr->grau_max = grau_max;
        gr->eh_ponderado = eh_ponderado;
        gr->eh_digrafo = eh_digrafo;

        gr->grau = (int*) calloc(nro_vertices, sizeof(int));

        gr->arestas = (int**)malloc(nro_vertices*sizeof(int*));

        for(i = 0; i < nro_vertices; i++){
            gr->arestas[i] = (int*)malloc(grau_max*sizeof(int));

            if(gr->eh_ponderado){
                gr->pesos = (int**) malloc(nro_vertices*sizeof(int*));

                for(j = 0; j < nro_vertices; j++){
                    gr->pesos[j] = (int*)malloc(grau_max*sizeof(int));
                }
            }
        }
    }

    return gr;
}

int insere_aresta(Grafo* gr, int origem, int destino, int peso){
    if(gr == NULL) return 0;

    if(origem < 0 || origem >= gr->nro_vertices) return 0;

    if(destino < 0 || destino >= gr->nro_vertices) return 0;

    gr->arestas[origem][gr->grau[origem]] = destino;

    if(gr->eh_ponderado){
        gr->pesos[origem][gr->grau[origem]] = peso;
    }

    gr->grau[origem]++;

    if(gr->eh_digrafo == 0) {
        gr->arestas[destino][gr->grau[destino]] = origem;
        if(gr->eh_ponderado){
            gr->pesos[destino][gr->grau[destino]] = peso;
        }
        gr->grau[destino]++;
    }

    return 1;
}

void libera_grafo(Grafo *gr){
    if(gr != NULL){
        int i;
        for(i = 0; i < gr->nro_vertices; i++){
            free(gr->arestas[i]);
        }
        free(gr->arestas);

        if(gr->eh_ponderado){
            for(i = 0; i < gr->nro_vertices; i++){
                free(gr->pesos[i]);
            }
            free(gr->pesos);
        }
        free(gr->grau);
        free(gr);
    }
}

void printa_grafo(Grafo *gr, int nro_vertices){
    int i, j;

    if(gr == NULL) return;

    for(i = 0; i < gr->nro_vertices; i++){
        printf("%d -> ", i);
        
        for(j = 0; j < gr->grau[i]; j++){
            printf("%d", gr->arestas[i][j]);
            if(gr->eh_ponderado){
                printf("[%d]", gr->pesos[i][j]);
            }
            if(j + 1 < gr->grau[i]){
                printf(", ");
            }
        }
        printf("\n");
    }
}


int main() {
    int numero_vertices, grau_maximo, numero_arestas, eh_digrafo;
    scanf("%d %d %d %d", &numero_vertices, &grau_maximo, &numero_arestas, &eh_digrafo);

    
    Grafo *gr = cria_grafo(numero_vertices, grau_maximo, 1, eh_digrafo);

    int origem, objetivo;
    scanf("%d %d", &origem, &objetivo);


    int i, v1, v2, peso;
    for(i = 0; i < numero_arestas; i++) {
        scanf("%d %d %d", &v1, &v2, &peso);
    }

    int ant[numero_vertices];
    int dist[numero_vertices];
    int caminho[numero_vertices];
    int contador = numero_vertices - 1;

    for(i = 0; i < numero_vertices; i++){
        ant[i] = -1;
    }

    dijkstra(gr, origem, ant, dist);

    int encontrado = encontra_caminho(origem, objetivo, numero_vertices, caminho, ant, contador);
    
    if(encontrado) {
        imprime_caminho(origem, numero_vertices, caminho);
    }

    libera_grafo(gr);

    return 0;
}
