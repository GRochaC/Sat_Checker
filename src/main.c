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
int eval(st* st, ht* ht, long long bool_eval) {
    if(st->primary_operator == NULL) {
        return is_set(bool_eval, get(ht,st->symb));
    }

    int e1 = (st->left != NULL) ? eval(st->left,ht,bool_eval) : -1;
    int e2 = eval(st->right,ht,bool_eval);

    if(strncmp(st->primary_operator, "~",1) == 0) return !e2;
    if(strncmp(st->primary_operator, "&", 1) == 0) return (e1 && e2);
    if(strncmp(st->primary_operator, "|", 1) == 0) return (e1 || e2);
    if(strncmp(st->primary_operator, "->", 2) == 0) return (!e1 || e2);
    return ((!e1 || e2) && (!e2 || e1));
}

// verifica se a formula eh satisfativel
int is_satisfiable(st* st, ht* ht, long long* model) {
        int sats = 0;
        
        // modelo = bitmask
        // procura um modelo que satisfaca a formula dada sua arvore sintatica
        for(long long bool_eval = 0; bool_eval < (1 << ht->sz) && !sats; bool_eval++) {
            sats |= eval(st, ht, bool_eval);
            if(sats) *model = bool_eval;
        }

        return sats;
}

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("ERRO: Arquivo de entrada não informado.\n");
        return 1;
    }

    FILE* input;
    char* line = NULL;
    size_t len = 0;
    __ssize_t read;
    FILE* output;

    input = fopen(argv[1],"r");
    if(input == NULL) {
        printf("ERRO: Não foi possível abrir o arquivo de entrada.\n");
        return 1;
    }

    output = fopen("saida.txt","w");
    if(output == NULL) {
        printf("ERRO: Não foi possível criar o arquivo de saída.\n");
        return 1;
    }

    // le cada formula do arquvio, linha por linha
    while((read = getline(&line,&len,input)) != -1) {
        // constroi a arvore sintatica da formula
        st* syn_tree = build_syntax_tree(line);
        if(syn_tree == NULL) {
            free_syntax_tree(syn_tree);
            fclose(input);
            fclose(output);
            if(line) free(line);
            free(input);
            free(output);

            printf("ERRO: Não foi possível criar a árvore sintática.\n");
            return 1;
        }

        // cria uma hashtable
        ht* hashtable = new_hashtable();
        l* list = new_list();

        if(hashtable == NULL || list == NULL) {
            free_syntax_tree(syn_tree);
            fclose(input);
            fclose(output);
            if(line) free(line);
            free(input);
            free(output);

            printf("ERRO: Não foi possível criar as estruturas de dados.\n");
            return 1;
        }

        // contando quantos simbolos proposicionais diferentes existem
        count_prop_symb(syn_tree, hashtable, list);


        // verifica se a formula eh satisfativel
        long long model;
        if(is_satisfiable(syn_tree,hashtable, &model)) {
            fprintf(output,"SIM,");

            fprintf(output,"[");
            node* ptr = list->head;
            while(ptr != NULL) {
                if(is_set(model, get(hashtable,ptr->val))) fprintf(output,"[%s, V]", ptr->val);
                else fprintf(output,"[%s, F]",ptr->val);

                if(ptr->nxt != NULL) fprintf(output,",");
                ptr = ptr->nxt;
            }
            fprintf(output,"]\n");

        } else fprintf(output,"NAO,[]\n");

        // frees
        free_syntax_tree(syn_tree);
        free_hashtable(hashtable);
        free_list(list);
    }

    if(line) free(line);
    fclose(input);
    fclose(output);

    printf("\"saida.txt\" gerado com sucesso!\n");
    return 0;
}