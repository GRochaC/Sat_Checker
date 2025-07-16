#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include "stdlib.h"
#include "string.h"

typedef struct Syntax_Tree {
    char* primary_operator;
    struct Syntax_Tree* left;
    struct Syntax_Tree* right;
    char* symb;
} st;

// pega o intervalo str[start, end)
char* slice(char* str, int start, int end) {
    int sz = end-start;
    char* ret = (char*) malloc((sz+1)+sizeof(char));
    strncpy(ret, str+start, sz);
    ret[sz] = '\0';
    return ret;
}

// constroi a arvore sintatica de uma formula bem-formada
st* build_syntax_tree(char* formula) {
    //printf("formula atual: %s\n",formula);
    st* syn_tree = (st*) malloc(sizeof(st));
    if(syn_tree == NULL) return NULL;
    
    syn_tree->primary_operator = NULL;
    syn_tree->left = NULL;
    syn_tree->right = NULL;
    syn_tree->symb = NULL;

    // remove os '\0'
    for(int i = 0; i < strlen(formula); i++) formula[i] = (formula[i] == '\n') ? '\0' : formula[i];
    
    // caso negacao
    if(formula[0] == '~') {
        syn_tree->primary_operator = strdup("~");
        st* r = build_syntax_tree(&formula[1]);
        syn_tree->right = r;
        
        return syn_tree;
    }
    
    // caso simbolo proposicional
    if(formula[0] != '(') {
        syn_tree->symb = strdup(formula);
        return syn_tree;
    }
    

    // caso op binario
    int open_par = 0;
    int i = 0;
    int j = 0;

    // procura o espaco entre a subformula da esquerda e o operador principal
    while(j < strlen(formula)) {
        if(formula[j] == '(') open_par++;
        else if(formula[j] == ')') open_par--;

        if(open_par == 1 && formula[j] != ' ') {} //continue 
        else if(open_par == 1 && formula[j] == ' ') break;
        j++;
    }

    // pega a subformula da esquerda
    char* subformula = slice(formula, ++i, j);
    syn_tree->left = build_syntax_tree(subformula);
    i = j;
    j++;

    // pega o operador principal
    while(formula[j] != ' ') j++;
    char* op = slice(formula, ++i, j);
    syn_tree->primary_operator = op;
    i = j;

    // pega a subformula da direita
    j = strlen(formula)-1;
    while(formula[j] != ')') j--;
    subformula = slice(formula, ++i, j);
    syn_tree->right = build_syntax_tree(subformula);

    free(subformula);

    return syn_tree;
}

// imprime a arvore sintatica
void print_syntax_tree(st* syn_tree) {
    // simb prop
    if(syn_tree->primary_operator == NULL) {
        printf("%s",syn_tree->symb);
        return;
    }

    // negacao
    if(strncmp(syn_tree->primary_operator,"~",1) == 0) {
        printf("~");
        printf("( ");
        print_syntax_tree(syn_tree->right);
        printf(" )");

        return;
    }

    // binario
    printf("( ");
    print_syntax_tree(syn_tree->left);
    printf(" %s ", syn_tree->primary_operator);
    print_syntax_tree(syn_tree->right);
    printf(" )");

    return;
}

// libera a arvore sintatica
void free_syntax_tree(st* syn_tree) {
    if(syn_tree == NULL) return;

    if(syn_tree->left != NULL) free_syntax_tree(syn_tree->left);
    if(syn_tree->right != NULL) free_syntax_tree(syn_tree->right);
    if(syn_tree->primary_operator != NULL) free(syn_tree->primary_operator);
    if(syn_tree->symb != NULL) free(syn_tree->symb);
    free(syn_tree);
    return;
}

#endif