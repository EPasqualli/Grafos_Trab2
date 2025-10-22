#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leitor_rede.h"

// Funcao auxiliar para adicionar um vertice (metabolito ou reacao) ao grafo
// e ao mapa de nomes, evitando duplicatas. Retorna o ID do vertice.
static int adicionar_vertice_mapa(mapaNomes* mapa, const char* nome, int tipo, grafo G) {
    // Verifica se o nome ja existe no mapa
    for (int i = 0; i < mapa->contador; i++) {
        if (strcmp(mapa->nomes[i], nome) == 0) {
            return i + 1; // Retorna o ID (1-based) existente
        }
    }

    // Se nao existe, adiciona um novo
    if (mapa->contador >= mapa->capacidade) {
        // Logica para expandir o mapa se necessario (nao implementado por simplicidade)
        fprintf(stderr, "Capacidade maxima de vertices atingida.\n");
        exit(1);
    }

    int novo_id_idx = mapa->contador;
    mapa->nomes[novo_id_idx] = strdup(nome);
    mapa->tipos[novo_id_idx] = tipo;
    mapa->contador++;

    // Adiciona o vertice ao grafo com ID 1-based
    adiciona_vertice(novo_id_idx + 1, G);
    return novo_id_idx + 1;
}

grafo processar_arquivo_met(const char* nome_arquivo, mapaNomes* mapa) {
    FILE* f = fopen(nome_arquivo, "r");
    if (!f) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    grafo g = cria_grafo(); // Usa a funcao da sua biblioteca

    // Inicializa o mapa de nomes
    mapa->capacidade = MAX_VERTICES; // Usa a mesma capacidade do grafo
    mapa->nomes = malloc(mapa->capacidade * sizeof(char*));
    mapa->tipos = malloc(mapa->capacidade * sizeof(int));
    mapa->contador = 0;

    char linha[1024];
    while (fgets(linha, sizeof(linha), f)) {
        linha[strcspn(linha, "\n\r")] = 0; // Remove quebras de linha

        char* nome_reacao = strtok(linha, " \t");
        char* substratos_str = strtok(NULL, "->");
        char* produtos_str = strtok(NULL, "");

        if (!nome_reacao || !substratos_str || !produtos_str) continue;

        // Adiciona o vertice da reacao
        int id_reacao = adicionar_vertice_mapa(mapa, nome_reacao, REACAO, g);

        // Processa substratos
        char* substrato = strtok(substratos_str, " +");
        while (substrato) {
            int id_metabolito = adicionar_vertice_mapa(mapa, substrato, METABOLITO, g);
            adiciona_arco(id_metabolito, id_reacao, g); // Arco: Metabolito -> Reacao
            substrato = strtok(NULL, " +");
        }

        // Processa produtos
        char* produto = strtok(produtos_str, " +");
        while (produto) {
            int id_metabolito = adicionar_vertice_mapa(mapa, produto, METABOLITO, g);
            adiciona_arco(id_reacao, id_metabolito, g); // Arco: Reacao -> Metabolito
            produto = strtok(NULL, " +");
        }
    }

    fclose(f);
    return g;
}

void destruir_mapa_nomes(mapaNomes* mapa) {
    for (int i = 0; i < mapa->contador; i++) {
        free(mapa->nomes[i]);
    }
    free(mapa->nomes);
    free(mapa->tipos);
}