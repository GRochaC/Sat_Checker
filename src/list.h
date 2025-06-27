#include "stdlib.h"
#include "string.h"

typedef struct Node {
    char* val;
    struct Node* prx;
} node;

typedef struct List {
    node* head;
    node* tail;
    int sz;
} l;

l* new_list() {
    l* li = (l*) malloc(sizeof(l));
    li->head=NULL;
    li->tail=NULL;
    li->sz=0;

    return li;
}

void add(l* l, char* str) {
    node* no = (node*) malloc(sizeof(node));
    no->val = strdup(str);
    no->prx = NULL;

    if(l->sz == 0) l->head = l->tail = no;
    else l->tail->prx = no;

    l->sz++;
    return;
}

void free_list(l* l) {
    node* aux = l->head;
    while(aux != NULL) {
        l->head = aux->prx;
        free(aux);
        aux = l->head;
    }
    free(l);
    return;
}
