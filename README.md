# Trabalho 2 - Análise de Redes Metabólicas

## Autor(es):
- Elen Pasqualli Gesser - GRR2021340

## Biblioteca de Grafos Utilizada:
Foi utilizada a implementação com MATRIZ DE ADJACÊNCIA (Versão 2)

## Detalhes da Implementação:
O programa foi estruturado nos seguintes módulos:

1.  **analise.c**: Arquivo principal que contém a função `main` e a
    implementação dos algoritmos de análise (detecção de ciclo via DFS
    e ordenação topológica via algoritmo de Kahn).

2.  **leitor_rede.c / leitor_rede.h**: Módulo criado para ler e interpretar o
    arquivo de entrada `.met`. Ele é responsável por:
    - Mapear os nomes de metabólitos e reações (strings) para IDs
      numéricos (inteiros) através da struct `MapaNomes`.
    - Construir o grafo direcionado bipartido utilizando as funções da
      biblioteca de grafos.

3.  **grafo.c / grafo.h**: Biblioteca base. Foram adicionadas as funções
    `adiciona_arco`, `obter_num_vertices` e `obter_matriz_adj` para
    suportar grafos direcionados e permitir que os algoritmos de
    análise acessem a estrutura do grafo de forma segura.

## Limitações de Funcionamento:
- A capacidade máxima da rede está limitada pela constante `MAX_VERTICES`
  (definida como 100) em `grafo.h`. Redes maiores exigirão a alteração
  desta constante e a recompilação do programa.
- O leitor_rede espera que o formato do arquivo de entrada seja estritamente
  o definido no enunciado.

## Como Compilar e Executar:
1.  Para compilar, execute o comando `make` no terminal.
2.  Para executar o programa, utilize o seguinte formato:
    $ ./analise <nome_do_arquivo.met>

    Exemplos:
    $ ./analise rede1.met
    $ ./analise rede2.met