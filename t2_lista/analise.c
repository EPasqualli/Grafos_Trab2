#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "leitor_rede.h"
#include "grafo.h"
#include "lista.h"

// --- ALGORITMO UNIFICADO (DFS) PARA LISTA DE ADJACÊNCIA ---

bool dfs_util(int u_id, grafo g, int* estado, lista ordenacao_topologica) {
    int u_idx = u_id - 1; // Converte ID para índice 0-based
    estado[u_idx] = 1; // 1: Marca como "visitando" (cinza)

    // Encontra o vértice 'u' na lista de vértices do grafo
    vertice u = busca_chave(u_id, vertices(g), (int_f_obj)vertice_id);
    if (!u) return false; // Vértice não encontrado, não deveria acontecer

    // Percorre a lista de adjacência
    for (no n_arco = primeiro_no(fronteira(u)); n_arco; n_arco = proximo(n_arco)) {
        aresta arco = (aresta)conteudo(n_arco);
        vertice vizinho = vertice_v(arco); // O vizinho é o destino do arco
        int v_id = vertice_id(vizinho);
        int v_idx = v_id - 1;

        if (estado[v_idx] == 1) {
            return true; // tem ciclo
        }
        if (estado[v_idx] == 0) {
            if (dfs_util(v_id, g, estado, ordenacao_topologica)) {
                return true;
            }
        }
    }

    estado[u_idx] = 2; // 2: Marca como "finalizado" (preto)
    
    int* id_ptr = malloc(sizeof(int));
    *id_ptr = u_idx;
    empilha(id_ptr, ordenacao_topologica);
    
    return false;
}

bool analisar_rede(grafo g, lista lista_ordenada, int num_total_vertices) {
    if (num_total_vertices == 0) return false;

    // Estados: 0 = não visitado, 1 = visitando, 2 = finalizado
    int* estado = (int*)calloc(num_total_vertices, sizeof(int));
    
    // Itera pela lista de vértices do grafo
    for (no n_vert = primeiro_no(vertices(g)); n_vert; n_vert = proximo(n_vert)) {
        vertice v = (vertice)conteudo(n_vert);
        int v_id = vertice_id(v);
        int v_idx = v_id - 1;

        if (estado[v_idx] == 0) {
            if (dfs_util(v_id, g, estado, lista_ordenada)) {
                free(estado);
                return true; // Ciclo encontrado
            }
        }
    }
    
    free(estado);
    return false;
}

// --- FUNÇÃO PRINCIPAL ---

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo.met>\n", argv[0]);
        return 1;
    }

    mapaNomes mapa;
    grafo g = processar_arquivo_met(argv[1], &mapa);
    if (!g) return 1;

    lista lista_ordenada = cria_lista();
    
    if (analisar_rede(g, lista_ordenada, mapa.contador)) {
        printf("rede cíclica\n");
    } else {
        no n = primeiro_no(lista_ordenada);
        while (n) {
            int vertice_idx = *(int*)conteudo(n);
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
        free(conteudo(n));
        n = proximo(n);
    }
    free(lista_ordenada);

    destroi_grafo(g);
    destruir_mapa_nomes(&mapa);

    return 0;
}