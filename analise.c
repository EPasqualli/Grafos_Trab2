#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "leitor_rede.h"
#include "grafo.h"

// --- DETECCAO DE CICLO (DFS) ---

bool detectar_ciclo_util(int u_idx, grafo g, int* estado) {
    estado[u_idx] = 1; // 1: Visitando (cinza)

    int num_vertices = obter_num_vertices(g);
    int** matriz_adj = obter_matriz_adj(g);

    for (int v_idx = 0; v_idx < num_vertices; v_idx++) {
        if (matriz_adj[u_idx][v_idx]) { // Se existe arco u -> v
            if (estado[v_idx] == 1) {
                return true; // Ciclo detectado!
            }
            if (estado[v_idx] == 0 && detectar_ciclo_util(v_idx, g, estado)) {
                return true;
            }
        }
    }

    estado[u_idx] = 2; // 2: Visitado (preto)
    return false;
}

bool tem_ciclo(grafo g) {
    int num_vertices = obter_num_vertices(g);
    if (num_vertices == 0) return false;

    // Estados: 0 = nao visitado (branco), 1 = visitando (cinza), 2 = visitado (preto)
    int* estado = (int*)calloc(num_vertices, sizeof(int));
    
    for (int i = 0; i < num_vertices; i++) {
        if (estado[i] == 0) {
            if (detectar_ciclo_util(i, g, estado)) {
                free(estado);
                return true;
            }
        }
    }
    
    free(estado);
    return false;
}

// --- ORDENACAO TOPOLOGICA (Kahn's Algorithm) ---

void ordenacao_topologica(grafo g, mapaNomes* mapa) {
    int num_vertices = obter_num_vertices(g);
    int** matriz_adj = obter_matriz_adj(g);

    int* grau_entrada = (int*)calloc(num_vertices, sizeof(int));

    // Calcula o grau de entrada de todos os vertices
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            if (matriz_adj[i][j]) {
                grau_entrada[j]++;
            }
        }
    }

    // Fila para vertices com grau de entrada 0
    int* fila = (int*)malloc(num_vertices * sizeof(int));
    int inicio_fila = 0, fim_fila = 0;

    for (int i = 0; i < num_vertices; i++) {
        if (grau_entrada[i] == 0) {
            fila[fim_fila++] = i;
        }
    }

    // Processa a fila
    while (inicio_fila < fim_fila) {
        int u_idx = fila[inicio_fila++];

        if (mapa->tipos[u_idx] == METABOLITO) {
            printf("%s ", mapa->nomes[u_idx]);
        }

        for (int v_idx = 0; v_idx < num_vertices; v_idx++) {
            if (matriz_adj[u_idx][v_idx]) {
                grau_entrada[v_idx]--;
                if (grau_entrada[v_idx] == 0) {
                    fila[fim_fila++] = v_idx;
                }
            }
        }
    }
    printf("\n");

    free(grau_entrada);
    free(fila);
}

// --- FUNCAO PRINCIPAL ---

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo.met>\n", argv[0]);
        return 1;
    }

    mapaNomes mapa;
    grafo g = processar_arquivo_met(argv[1], &mapa);
    if (!g) {
        return 1;
    }

    if (tem_ciclo(g)) {
        printf("rede cíclica\n");
    } else {
        ordenacao_topologica(g, &mapa);
    }

    // Liberar memoria
    destroi_grafo(g);
    destruir_mapa_nomes(&mapa);

    return 0;
}