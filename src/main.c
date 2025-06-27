#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "syntax_tree.h"
#include "data_structures.h"

// conta a quantidade de simb. prop. diferentes na formula
void count_prop_symb(st* st, ht* ht, l* l) {
    if(st->symb != NULL) {
        // se ja nao foi adiciona, adiciona a hashtable: ht[symb] = sz
        if(!find(ht,st->symb)) {
            add(ht,st->symb,ht->sz);
            append(l,st->symb);
        }
        return;
    }

    if(st->left != NULL) count_prop_symb(st->left,ht,l);
    count_prop_symb(st->right,ht,l);

    return;
}

// verifica se o idx-esimo bit esta ligado
int is_set(int bitmask, int idx) {
    return (bitmask & (1 << idx));
}

// avalia a valorcao booleana de uma formula dada sua arvore sintatica e uma valoracao booleana
int eval(st* st, ht* ht, int model) {
    if(st->primary_operator == NULL) {
        return is_set(model, get(ht,st->symb));
    }

    int e1 = (st->left != NULL) ? eval(st->left,ht,model) : -1;
    int e2 = eval(st->right,ht,model);

    if(strncmp(st->primary_operator, "~",1) == 0) return !e2;
    if(strncmp(st->primary_operator, "&", 1) == 0) return (e1 && e2);
    if(strncmp(st->primary_operator, "|", 1) == 0) return (e1 || e2);
    if(strncmp(st->primary_operator, "->", 2) == 0) return (!e1 || e2);
    return ((!e1 || e2) && (!e2 || e1));
}

// procura um modelo que satisfaca a formula dada sua arvore sintatica
void get_model(st* st, ht* ht, l* l) {
        // modelo = bitmask
        int final_model;

        // verifica se a formula eh satisfativel
        int sats = 0;
        for(int model = 0; model < (1 << ht->sz) && !sats; model++) {
            sats |= eval(st, ht, model);
            if(sats) final_model = model;
        }

        if(!sats) printf("NAO");
        else {
            printf("SIM, [");

            node* ptr = l->head;
            while(ptr != NULL) {
                if(is_set(final_model, get(ht,ptr->val))) printf("[%s, V]", ptr->val);
                else printf("[%s, F]",ptr->val);

                if(ptr->nxt != NULL) printf(", ");
                ptr = ptr->nxt;
            }

            printf("]");
        }
        printf("\n");

        return;
}

int main() {
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    __ssize_t read;

    fp = fopen("entrada.txt","r");
    if(fp == NULL) return 1;

    // le cada formula do arquvio, linha por linha
    while((read = getline(&line,&len,fp)) != -1) {
        // constroi a arvore sintatica da formula
        printf("formula: %s\n",line);
        st* syn_tree = build_syntax_tree(line);

        printf("arvore construida: ");
        print_syntax_tree(syn_tree);
        printf("\n");

        // cria uma hashtable
        ht* hash_table = new_hashtable();
        l* list = new_list();

        // contando quantos simbolos proposicionais diferentes existem
        count_prop_symb(syn_tree, hash_table, list);
        printf("Quantidade de simbolos = %d =?= %d\n", hash_table->sz, list->sz);

        // procura o modelo que satisfaz a formula
        get_model(syn_tree,hash_table,list);

        // frees
        free_syntax_tree(syn_tree);
        free_hashtable(hash_table);
        free_list(list);
    }

    fclose(fp);

    if(line) free(line);

    return 0;
}