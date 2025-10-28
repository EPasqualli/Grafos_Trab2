/*******************************************
 * Implementação de biblioteca para grafos.
 *
 * Algoritmos em Grafos e Otimização
 * Departamento de Informática - UFPR
 * prof. Guilherme Derenievicz
 *******************************************/

#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>

//---------------------------------------------------------
// getters:

int vertice_id(vertice v) {
    return v->id;
};
lista fronteira(vertice v) {
    return v->fronteira;
};
int aresta_id(aresta e) {
    return e->id;
};
int peso_aresta(aresta e) { 
    return e->peso; 
}; 
vertice vertice_u(aresta e) { // NOVO GETTER
    return e->u;
};
vertice vertice_v(aresta e) {
    return e->v;
};
lista vertices(grafo G) {
    return G->vertices;
};
lista arestas(grafo G) {
    return G->arestas;
};

//---------------------------------------------------------
// funcoes para construcao/desconstrucao do grafo:

// cria grafo vazio e o retorna
grafo cria_grafo() {
    grafo G = (grafo) malloc(sizeof(t_grafo));
    if (!G) exit(1);
    G->vertices = cria_lista();
    G->arestas = cria_lista();
    return G;
}

// destroi grafo G (desaloca toda a memoria), modificada para o t2 por conta dos arcos
void destroi_grafo(grafo G) {
    while (!vazio(arestas(G))) {
        aresta e = (aresta)desempilha(arestas(G));
        free(e);
    }
    free(G->arestas);
    while (!vazio(vertices(G))) {
        vertice v = (vertice)desempilha(vertices(G));
        while (!vazio(fronteira(v))) {
            desempilha(fronteira(v));
        }
        free(v->fronteira);
        free(v);
    }
    free(G->vertices);
    free(G);
}

// cria novo vertice com id <id> e adiciona ao grafo G
void adiciona_vertice(int id, grafo G) {
    vertice v = (vertice) malloc(sizeof(t_vertice));
    if (!v) exit(1);
    v->id = id;
    v->fronteira = cria_lista();
    empilha(v, vertices(G));
}

// remove vertice com id <id> do grafo G e o destroi
// [deve remover e destruir tambem as arestas incidentes]
void remove_vertice(int id, grafo G) {
    vertice v = busca_chave(id, vertices(G), (int_f_obj) vertice_id);
    if (!v) return;
    while(!vazio(fronteira(v))) {
        aresta e = desempilha(fronteira(v));
        remove_aresta(aresta_id(e), G);
    }
    remove_chave(id, vertices(G), (int_f_obj) vertice_id);
    free(v->fronteira);
    free(v);
}

// cria aresta com id <id> incidente a vertices com
// ids <u_id> e <v_id> e adiciona ao grafo G
void adiciona_aresta(int id, int u_id, int v_id, grafo G) {
    vertice u = busca_chave(u_id, vertices(G), (int_f_obj) vertice_id);
    vertice v = busca_chave(v_id, vertices(G), (int_f_obj) vertice_id);
    if (!u || !v) return;

    aresta e = (aresta) malloc(sizeof(t_aresta));
    if (!e) exit(1);
    
    e->id = id;
    e->u = u;
    e->v = v;
    e->peso = 0; // <-- INICIALIZA PESO COM 0
    
    empilha(e, arestas(G));
    empilha(e, fronteira(u));
    empilha(e, fronteira(v));
}

// remove aresta com id <id> do grafo G e a destroi
void remove_aresta(int id, grafo G) {
    aresta e = remove_chave(id, arestas(G), (int_f_obj) aresta_id);
    if (!e) return;
    remove_chave(id, fronteira(vertice_u(e)), (int_f_obj) aresta_id);
    remove_chave(id, fronteira(vertice_v(e)), (int_f_obj) aresta_id);
    free(e);
}

//---------------------------------------------------------
// funcoes para operacoes com o grafo pronto:

// calcula e atribui o peso para todas as arestas do grafo G
// peso({u,v}) = grau(u) + grau(v)
void calcula_pesos_arestas(grafo G) {
    for (no n = primeiro_no(arestas(G)); n; n = proximo(n)) {
        aresta e = (aresta) conteudo(n);
        vertice u = vertice_u(e);
        vertice v = vertice_v(e);
        e->peso = grau(u) + grau(v);
    }
}

// calcula e devolve o grau do vertice v
int grau(vertice v) {
    int d_v = 0;
    for (no n = primeiro_no(fronteira(v)); n; n = proximo(n))
        ++d_v;
    return d_v;
}

// imprime o grafo G
void imprime_grafo(grafo G) {
    printf("\n--- IMPRESSAO DO GRAFO ---\n");
    printf("Vertices: <id> - [grau]( <fronteira> )\n");
    imprime_lista(vertices(G), (void_f_obj) imprime_vertice);
    printf("\n\nArestas: <id>:[peso]{u,v}\n");
    imprime_lista(arestas(G), (void_f_obj) imprime_aresta);
    printf("\n--------------------------\n");
}

// imprime o vertice v
void imprime_vertice(vertice v) {
    printf("\n%d - [%d]( ", vertice_id(v), grau(v));
    imprime_lista(fronteira(v), (void_f_obj) imprime_aresta);
    printf(")");
}

// imprime a aresta e
void imprime_aresta(aresta e) {
    int u_id = vertice_id(vertice_u(e));
    int v_id = vertice_id(vertice_v(e));
    // Novo formato: id:[peso]{u,v}
    printf("%d:[%d]{%d,%d} ", aresta_id(e), peso_aresta(e), u_id, v_id);
}

// cria arco com id <id> de <u_id> para <v_id> e adiciona ao grafo G
void adiciona_arco(int id, int u_id, int v_id, grafo G) {
    // Busca os vértices de origem e destino
    vertice u = busca_chave(u_id, vertices(G), (int_f_obj) vertice_id);
    vertice v = busca_chave(v_id, vertices(G), (int_f_obj) vertice_id);
    if (!u || !v) return; // Se algum não existir, não faz nada

    // Cria a estrutura da aresta (representa um arco)
    aresta e = (aresta) malloc(sizeof(t_aresta));
    if (!e) exit(1);
    
    e->id = id;
    e->u = u; // Vértice de origem
    e->v = v; // Vértice de destino
    e->peso = 0; 
    
    // Adiciona o arco à lista principal de arestas do grafo
    empilha(e, arestas(G));
    
    // Adiciona o arco APENAS na fronteira do vértice de ORIGEM (u)
    empilha(e, fronteira(u));
}