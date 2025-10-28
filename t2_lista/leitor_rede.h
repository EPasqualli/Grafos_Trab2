#ifndef LEITOR_REDE_H
#define LEITOR_REDE_H

#include "grafo.h"

#define METABOLITO 0
#define REACAO 1

// Estrutura para mapear nomes (string) para IDs (int) no grafo.
typedef struct {
    char** nomes; // Array com os nomes de cada vertice
    int* tipos;   // Array com o tipo de cada vertice (METABOLITO ou REACAO)
    int contador;
    int capacidade;
} mapaNomes;

// Funcao principal que le o arquivo e constroi o grafo.
// Retorna o grafo construido e preenche a estrutura do mapa.
grafo processar_arquivo_met(const char* nome_arquivo, mapaNomes* mapa);

// Libera a memoria alocada pelo mapa
void destruir_mapa_nomes(mapaNomes* mapa);

#endif