/*******************************************
 * Implementação de biblioteca para grafos.
 *
 * Algoritmos em Grafos e Otimização
 * Departamento de Informática - UFPR
 * prof. Guilherme Derenievicz
 *
 * VERSÃO 2: MATRIZ DE ADJACÊNCIA
 *******************************************/

#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef _ERRO_MALLOC_
#define _ERRO_MALLOC_ 1
#endif

struct t_grafo {
    int n_vertices;
    int capacidade;
    int **adj_matrix;
};

//---------------------------------------------------------
// funcoes para construcao/desconstrucao do grafo;

grafo cria_grafo() {
    grafo G = (grafo)malloc(sizeof(struct t_grafo));
    if (!G) exit(_ERRO_MALLOC_);

    G->n_vertices = 0;
    G->capacidade = MAX_VERTICES;
    G->adj_matrix = (int **)malloc(G->capacidade * sizeof(int *));
    if (!G->adj_matrix) exit(_ERRO_MALLOC_);

    for (int i = 0; i < G->capacidade; i++) {
        G->adj_matrix[i] = (int *)calloc(G->capacidade, sizeof(int));
        if (!G->adj_matrix[i]) exit(_ERRO_MALLOC_);
    }
    return G;
}

void destroi_grafo(grafo G) {
    if (!G) return;
    for (int i = 0; i < G->capacidade; i++) {
        free(G->adj_matrix[i]);
    }
    free(G->adj_matrix);
    free(G);
}

void adiciona_vertice(int id, grafo G) {
    if (id <= 0 || id > G->capacidade) return;
    if (id > G->n_vertices) {
        G->n_vertices = id;
    }
}

void adiciona_aresta(int id, int u_id, int v_id, grafo G) {
    int u_idx = u_id - 1;
    int v_idx = v_id - 1;
    if (u_id > G->n_vertices || v_id > G->n_vertices || u_id <= 0 || v_id <= 0) return;
    G->adj_matrix[u_idx][v_idx] = id;
    G->adj_matrix[v_idx][u_idx] = id;
}

//---------------------------------------------------------
// funcoes para operacoes com o grafo

int grau_por_id(int id, grafo G) {
    int v_idx = id - 1;
    int d = 0;
    if (id > G->n_vertices) return 0;
    for (int i = 0; i < G->n_vertices; i++) {
        if (G->adj_matrix[v_idx][i] > 0) {
            d++;
        }
    }
    return d;
}

void imprime_grafo(grafo G) {
    printf("\nVertices: <id> - [grau]( <fronteira> )");
    printf("\nVertices: ");
    for (int i = 0; i < G->n_vertices; i++) {
        int u_id = i + 1;
        printf("%d - [%d]( ", u_id, grau_por_id(u_id, G));
        for (int j = 0; j < G->n_vertices; j++) {
            if (G->adj_matrix[i][j] > 0) {
                printf("%d:{%d,%d} ", G->adj_matrix[i][j], u_id, j + 1);
            }
        }
        printf(") ");
    }

    printf("\nArestas: <id>:{u,v}");
    printf("\nArestas: ");
    for (int i = 0; i < G->n_vertices; i++) {
        for (int j = i; j < G->n_vertices; j++) {
            if (G->adj_matrix[i][j] > 0) {
                printf("%d:{%d,%d} ", G->adj_matrix[i][j], i + 1, j + 1);
            }
        }
    }
    printf("\n");
}

// funcao para arcos direcionados
void adiciona_arco(int u_id, int v_id, grafo G) {
    // A biblioteca original usa IDs 1-based, mas a matriz eh 0-based
    int u_idx = u_id - 1;
    int v_idx = v_id - 1;

    if (u_id > G->n_vertices || v_id > G->n_vertices || u_id <= 0 || v_id <= 0) return;
    
    // Cria uma aresta de u para v (u -> v)
    G->adj_matrix[u_idx][v_idx] = 1; // 1 para indicar a existencia de um arco
}


//---------------------------------------------------------------
// Funções apenas para satisfazer o grafo.h e manter tudo igual
// Funções implementadas no v1 
//---------------------------------------------------------------

void remove_vertice(int id, grafo G) {}
void remove_aresta(int id, grafo G) {}
int grau(vertice v) { return 0; }
lista vertices(grafo G) { return NULL; }
lista arestas(grafo G) { return NULL; }
int vertice_id(vertice v) { return v ? v->id : -1; }
lista fronteira(vertice v) { return NULL; }
int aresta_id(aresta e) { return e ? e->id : -1; }
vertice vertice_u(aresta e) { return e ? e->u : NULL; }
vertice vertice_v(aresta e) { return e ? e->v : NULL; }
void imprime_vertice(vertice v) {}
void imprime_aresta(aresta e) {}

// getters
int obter_num_vertices(grafo G) {
    return G->n_vertices;
}

int** obter_matriz_adj(grafo G) {
    return G->adj_matrix;
}