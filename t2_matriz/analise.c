#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "leitor_rede.h"
#include "grafo.h"
#include "lista.h"

// --- ALGORITMO UNIFICADO (DFS) PARA CICLO E ORDENAÇÃO TOPOLÓGICA ---

// Função recursiva auxiliar para a DFS
// Retorna 'true' se um ciclo for detectado, 'false' caso contrário.
bool dfs_util(int u_idx, grafo g, int* estado, lista ordenacao_topologica) {
    estado[u_idx] = 1; // 1: Marca como "visitando" (cinza)

    int num_vertices = obter_num_vertices(g);
    int** matriz_adj = obter_matriz_adj(g);

    for (int v_idx = 0; v_idx < num_vertices; v_idx++) {
        // Se existe o arco u -> v
        if (matriz_adj[u_idx][v_idx]) {
            // Se o vizinho 'v' está sendo visitado (cinza), encontramos um ciclo.
            if (estado[v_idx] == 1) {
                return true; // Ciclo detectado!
            }
            // Se o vizinho 'v' ainda não foi visitado (branco), chama a recursão.
            if (estado[v_idx] == 0) {
                if (dfs_util(v_idx, g, estado, ordenacao_topologica)) {
                    return true; // Propaga a detecção do ciclo.
                }
            }
        }
    }

    estado[u_idx] = 2; // 2: Marca como "finalizado" (preto)
    
    // Adiciona o vértice no INÍCIO da lista de ordenação.
    // Usamos um truque: empilhamos um ponteiro para o int do índice.
    // Precisamos alocar memória para cada índice para não perder o valor.
    int* id_ptr = malloc(sizeof(int));
    *id_ptr = u_idx;
    empilha(id_ptr, ordenacao_topologica);
    
    return false; // Nenhum ciclo encontrado a partir deste vértice.
}

// Função principal que executa a análise.
// Retorna 'true' se a rede é cíclica, 'false' caso contrário.
// Se não for cíclica, a 'lista_ordenada' é preenchida corretamente.
bool analisar_rede(grafo g, lista lista_ordenada) {
    int num_vertices = obter_num_vertices(g);
    if (num_vertices == 0) return false;

    // Estados: 0 = não visitado (branco), 1 = visitando (cinza), 2 = finalizado (preto)
    int* estado = (int*)calloc(num_vertices, sizeof(int));
    
    for (int i = 0; i < num_vertices; i++) {
        if (estado[i] == 0) { // Se o vértice ainda não foi visitado
            if (dfs_util(i, g, estado, lista_ordenada)) {
                free(estado);
                return true; // Ciclo encontrado
            }
        }
    }
    
    free(estado);
    return false; // Nenhum ciclo encontrado no grafo inteiro
}


// --- FUNÇÃO PRINCIPAL ---

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

    lista lista_ordenada = cria_lista();
    
    if (analisar_rede(g, lista_ordenada)) {
        printf("rede cíclica\n");
    } else {
        // Se não for cíclica, imprime a ordenação topológica.
        // A lista já está na ordem correta, basta percorrê-la.
        no n = primeiro_no(lista_ordenada);
        while (n) {
            int* id_ptr = (int*)conteudo(n);
            int vertice_idx = *id_ptr;

            if (mapa.tipos[vertice_idx] == METABOLITO) {
                printf("%s ", mapa.nomes[vertice_idx]);
            }
            n = proximo(n);
        }
        printf("\n");
    }

    // Liberar memoria
    no n = primeiro_no(lista_ordenada);
    while (n) {
        free(conteudo(n)); // Libera os ponteiros de int que alocamos
        n = proximo(n);
    }
    free(lista_ordenada); // A função 'destroi_lista' seria ideal aqui

    destroi_grafo(g);
    destruir_mapa_nomes(&mapa);

    return 0;
}