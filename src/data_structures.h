#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "stdlib.h"
#include "string.h"

typedef struct HashTable {
    int MOD;        // tamanho da tabela
    int P;          // primo
    int* p;         // tabela das potencias do primo P
    int* table;     // tabela em si
    int sz;         // quantidade de itens
} ht;

// cria uma hashtable
ht* new_hashtable() {
    ht* hat = (ht*) malloc(sizeof(ht));
    if(hat == NULL) return NULL;

    hat->MOD=199999;
    hat->P=31;
    hat->sz = 0;

    int* p = (int*) malloc(hat->MOD*sizeof(int));
    if(p == NULL) return NULL;

    p[0] = 1;
    for(int i = 1; i < hat->MOD; i++) p[i] = (p[i-1] * hat->P) % hat->MOD;
    hat->p=p;

    int* table = (int*) malloc(hat->MOD*sizeof(int));
    if(table == NULL) return NULL;

    for(int i = 0; i < hat->MOD; i++) table[i] = -1;
    hat->table=table;

    return hat;
}

// funcao hash
int hash(ht* ht, char* str) {
    int ret = 0;
    for(int i = 0; i < strlen(str); i++) ret += (str[i] * ht->p[i]) % ht->MOD;
    return ret;
}

// mapeia um chave a um valor
void add(ht* ht, char* str, int val) {
    ht->table[hash(ht,str)] = val;
    ht->sz++;
}

// adquire o valor de uma chave
int get(ht* ht, char* str) {
    return ht->table[hash(ht, str)];
}

// verifica se a chave esta no hashmap
int find(ht* ht, char* str) {
    return (ht->table[hash(ht, str)] != -1);
}

// libera a hashtable
void free_hashtable(ht* ht) {
    if(ht == NULL) return;

    free(ht->p);
    free(ht->table);
    free(ht);
    return;
}

typedef struct Node {
    char* val;
    struct Node* nxt;
} node;

typedef struct List {
    node* head;
    node* tail;
    int sz;
} l;

// cria uma lista
l* new_list() {
    l* li = (l*) malloc(sizeof(l));
    if(li == NULL) return NULL;

    li->head=NULL;
    li->tail=NULL;
    li->sz=0;

    return li;
}

// adiciona um valor ao final da lista
void append(l* l, char* str) {
    node* no = (node*) malloc(sizeof(node));

    no->val = strdup(str);
    no->nxt = NULL;

    if(l->sz == 0) {
        l->head = no;
        l->tail = no;
    }
    else {
        l->tail->nxt = no;
        l->tail = no;
    }

    l->sz++;

    return;
}

// libera a lista
void free_list(l* l) {
    if(l == NULL) return;

    node* aux = l->head;
    while(aux != NULL) {
        l->head = aux->nxt;
        free(aux);
        aux = l->head;
    }
    free(l);
    return;
}

#endif