#include "stdlib.h"
#include "string.h"

typedef struct Hash_Table {
    int MOD;        // tamanho da tabela
    int P;          // primo
    int* p;         // tabela das potencias do primo P
    int* table;     // tabela em si
    int sz;
} ht;

ht* build_hash_table() {
    ht* hat = (ht*) malloc(sizeof(ht));
    hat->MOD=199999;
    hat->P=31;
    hat->sz = 0;

    int* p = (int*) malloc(hat->MOD*sizeof(int));
    p[0] = 1;
    for(int i = 1; i < hat->MOD; i++) p[i] = (p[i-1] * hat->P) % hat->MOD;
    hat->p=p;

    int* table = (int*) malloc(hat->MOD*sizeof(int));
    for(int i = 0; i < hat->MOD; i++) table[i] = -1;
    hat->table=table;

    return hat;
}

int hash(ht* ht, char* str) {
    int ret = 0;
    for(int i = 0; i < strlen(str); i++) ret += (str[i] * ht->p[i]) % ht->MOD;
    return ret;
}

void add(ht* ht, char* str, int val) {
    ht->table[hash(ht,str)] = val;
    ht->sz++;
}

int get(ht* ht, char* str) {
    return ht->table[hash(ht, str)];
}

int find(ht* ht, char* str) {
    return (ht->table[hash(ht, str)] != -1);
}

void free_hash_table(ht* ht) {
    free(ht->p);
    free(ht->table);
    free(ht);
    return;
}