#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "syntax_tree.h"
#include "hash_table.h"
#include "list.h"

// conta a quantidade de simb. prop. diferentes na formula
void count_prop_symb(st* st, ht* ht, l* l) {
    if(st->symb != NULL) {
        // se ja nao foi adiciona, adiciona a hashtable: ht[symb] = sz
        if(!find(ht,st->symb)) {
            add(ht,st->symb,ht->sz);
            
        }
        return;
    }

    if(st->left != NULL) count_prop_symb(st->left,ht,l);
    count_prop_symb(st->right,ht,l);

    return;
}

int is_set(int bitmask, int idx) {
    return (bitmask & (1 << idx));
}

int eval(st* st, ht* ht, int modelo) {
    if(st->primary_operator == NULL) {
        return is_set(modelo, get(ht,st->symb));
    }

    int e1 = (st->left != NULL) ? eval(st->left,ht,modelo) : -1;
    int e2 = eval(st->right,ht,modelo);

    if(strncmp(st->primary_operator, "~",1) == 0) return !e2;
    if(strncmp(st->primary_operator, "&", 1) == 0) return (e1 && e2);
    if(strncmp(st->primary_operator, "|", 1) == 0) return (e1 || e2);
    if(strncmp(st->primary_operator, "->", 2) == 0) return (!e1 || e2);
    return ((!e1 || e2) && (!e2 || e1));
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
        ht* hash_table = build_hash_table();
        l* list = new_list();

        // contando quantos simbolos proposicionais diferentes existem
        count_prop_symb(syn_tree, hash_table, list);
        printf("Quantidade de simbolos = %d\n", hash_table->sz);

        // modelo = bitmask
        int modelo_final;

        // verifica se a formula eh satisfativel
        int sats = 0;
        printf("Satisfativel? ");
        for(int modelo = 0; modelo < (1 << hash_table->sz) && !sats; modelo++) {
            sats |= eval(syn_tree, hash_table, modelo);
            if(sats) modelo_final = modelo;
        }

        if(!sats) printf("NAO\n");
        else printf("SIM, %d\n",modelo_final);

        // frees
        free_syntax_tree(syn_tree);
        free_hash_table(hash_table);
    }

    fclose(fp);

    if(line) free(line);

    return 0;
}