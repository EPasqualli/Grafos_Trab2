#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leitor_rede.h"
#include <ctype.h>  // Para a função isspace()

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

        // 1. Encontra o separador "->" para dividir a linha em duas partes.
        char* produtos_part = strstr(linha, "->");
        if (!produtos_part) continue; // Pula linhas mal formatadas

        // 2. Coloca um terminador nulo no lugar de "->" para separar as duas partes.
        *produtos_part = '\0';
        char* substratos_part = linha; // A primeira parte da linha (reação + substratos)
        
        // 3. Move o ponteiro dos produtos para depois do "->".
        produtos_part += 2;

        // --- Agora, processamos cada parte com segurança ---

        // 4. Pega o nome da reação da primeira parte.
        char* nome_reacao = strtok(substratos_part, " \t");
        if (!nome_reacao) continue;

        int id_reacao = adicionar_vertice_mapa(mapa, nome_reacao, REACAO, g);

        // 5. Processa os substratos, que são o resto da primeira parte.
        char* substrato = strtok(NULL, " +");
        while (substrato) {
            // Remove espaços em branco antes/depois se houver
            while (isspace((unsigned char)*substrato)) substrato++;
            
            int id_metabolito = adicionar_vertice_mapa(mapa, substrato, METABOLITO, g);
            adiciona_arco(id_metabolito, id_reacao, g); // Arco: Metabolito -> Reacao
            substrato = strtok(NULL, " +");
        }

        // 6. Processa os produtos da segunda parte.
        char* produto = strtok(produtos_part, " +");
        while (produto) {
            while (isspace((unsigned char)*produto)) produto++;

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