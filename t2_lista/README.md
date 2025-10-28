# Trabalho 2 - Análise de Redes Metabólicas

## Autor(es):
- Elen Pasqualli Gesser - GRR20213400
- Erick da Silva Santos - GRR20242593

## Biblioteca de Grafos Utilizada:
Foi utilizada a implementação com `LISTAS DE ADJACÊNCIA`, baseada no
código do Trabalho 1, `versão 1 da Elen`. A biblioteca foi adaptada para suportar as
necessidades de um `grafo direcionado`.

## Detalhes da Implementação:
O programa foi estruturado nos seguintes módulos:

1.  **analise.c**: Arquivo principal que contém a função `main` e a
    lógica central de análise da rede. De acordo como visto em aula,
    foi implementado um algoritmo unificado baseado na Busca em
    Profundidade (DFS) que realiza as duas tarefas principais do trabalho.

    *   **Detecção de Ciclo:** A DFS utiliza um sistema de três estados
      (0: não visitado, 1: visitando, 2: finalizado) para cada vértice.
      Um ciclo é detectado se, durante a busca, um arco leva a um
      vértice que já está na pilha de recursão (estado 1),
      caracterizando uma aresta de retorno (back edge).
      
    *   **Ordenação Topológica:** Se a busca termina sem encontrar ciclos,
      uma ordenação topológica é gerada. À medida que a DFS finaliza a
      exploração de um vértice (quando seu estado muda para 2), ele é
      adicionado no início de uma lista encadeada. Ao final do
      processo, a lista contém os vértices em ordem topológica, e o
      programa imprime apenas os que são do tipo "metabólito".

2.  **leitor_rede.c / leitor_rede.h**: Módulo criado para ler e interpretar o
    arquivo de entrada `.met`. Ele utiliza as funções `strstr` e `strtok` para dividir
    corretamente as linhas do arquivo e traduz cada reação em vértices
    e arcos no grafo. Também gerencia a `struct mapaNomes`, que mapeia os
    nomes (strings) para os IDs numéricos (inteiros) usados pelo grafo.

3.  **grafo.c / grafo.h**: A biblioteca de grafos do Trabalho 1 (versão com 
    listas de adjacência) foi adaptada com as seguintes modificações:
    *   **`adiciona_arco`**: Uma nova função foi criada para adicionar arcos
      direcionados (u -> v), inserindo a aresta apenas na lista de
      fronteira do vértice de origem (u).
    *   **`destroi_grafo` (Liberação de Memória)**: A função original foi
      completamente reescrita. A implementação anterior, projetada para
      grafos não-direcionados, causava erros de "Segmentation Fault"
      ao tentar remover arcos de listas onde eles não existiam. A nova
      versão libera a memória de forma segura e direta, primeiro
      liberando todas as arestas, depois todos os vértices e suas
      respectivas listas de fronteira.

## Limitações de Funcionamento:
- A capacidade máxima de nomes únicos (metabólitos + reações) é
  pré-definida na `struct mapaNomes` dentro do `leitor_rede.h`. Redes
  com mais elementos únicos que a capacidade definida exigirão a
  alteração do código.
- O leitor de arquivo (`leitor_rede`) espera que o formato de entrada seja
  estritamente o definido no enunciado.

## Como Compilar e Executar:
1.  Para compilar, execute o comando `make` no terminal.
2.  Para executar o programa, utilize o seguinte formato:
    $ ./analise <nome_do_arquivo.met>

    Exemplos:
    $ ./analise rede1.met
    $ ./analise rede2.met