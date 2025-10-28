#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "leitor_rede.h"

// Funcao auxiliar para adicionar um vertice (metabolito ou reacao) ao grafo
// e ao mapa de nomes, evitando duplicatas. Retorna o ID do vertice.
static int adicionar_vertice_mapa(mapaNomes* mapa, const char* nome, int tipo, grafo G) {
    for (int i = 0; i < mapa->contador; i++) {
        if (strcmp(mapa->nomes[i], nome) == 0) {
            return i + 1;
        }
    }

    if (mapa->contador >= mapa->capacidade) {
        fprintf(stderr, "Capacidade maxima de vertices atingida.\n");
        exit(1);
    }

    int novo_id_idx = mapa->contador;
    mapa->nomes[novo_id_idx] = strdup(nome);
    mapa->tipos[novo_id_idx] = tipo;
    mapa->contador++;

    adiciona_vertice(novo_id_idx + 1, G);
    return novo_id_idx + 1;
}

// Funcao principal que le o arquivo e constroi o grafo.
// Retorna o grafo construido e preenche a estrutura do mapa.
grafo processar_arquivo_met(const char* nome_arquivo, mapaNomes* mapa) {
    FILE* f = fopen(nome_arquivo, "r");
    if (!f) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    grafo g = cria_grafo();

    mapa->capacidade = 2000; // Capacidade inicial do mapa (para mais elementos ajustar aqui)
    mapa->nomes = malloc(mapa->capacidade * sizeof(char*));
    mapa->tipos = malloc(mapa->capacidade * sizeof(int));
    mapa->contador = 0;

    char linha[1024];
    int id_arco_atual = 1;

    while (fgets(linha, sizeof(linha), f)) {
        linha[strcspn(linha, "\n\r")] = 0;
        char* produtos_part = strstr(linha, "->");
        if (!produtos_part) continue;
        *produtos_part = '\0';
        char* substratos_part = linha;
        produtos_part += 2;

        char* nome_reacao = strtok(substratos_part, " \t");
        if (!nome_reacao) continue;
        int id_reacao = adicionar_vertice_mapa(mapa, nome_reacao, REACAO, g);

        char* substrato = strtok(NULL, " +");
        while (substrato) {
            while (isspace((unsigned char)*substrato)) substrato++;
            int id_metabolito = adicionar_vertice_mapa(mapa, substrato, METABOLITO, g);
            // função de arco
            adiciona_arco(id_arco_atual++, id_metabolito, id_reacao, g);
            substrato = strtok(NULL, " +");
        }

        char* produto = strtok(produtos_part, " +");
        while (produto) {
            while (isspace((unsigned char)*produto)) produto++;
            int id_metabolito = adicionar_vertice_mapa(mapa, produto, METABOLITO, g);
            // função de arco
            adiciona_arco(id_arco_atual++, id_reacao, id_metabolito, g);
            produto = strtok(NULL, " +");
        }
    }

    fclose(f);
    return g;
}

// Libera a memoria alocada pelo mapa
void destruir_mapa_nomes(mapaNomes* mapa) {
    for (int i = 0; i < mapa->contador; i++) {
        free(mapa->nomes[i]);
    }
    free(mapa->nomes);
    free(mapa->tipos);
}