/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "listaind.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

LISTAINDICE * listaIndCriar(void) {
    LISTAINDICE *lista = (LISTAINDICE *) malloc(sizeof (LISTAINDICE));

    lista->inicio = NULL;
    lista->fim = NULL;

    lista->tamanho = 0;

    return lista;
}

void listaIndApagar(LISTAINDICE *lista) {

    NOIND *cabeca = lista->inicio;

    while (cabeca != NULL) {
        NOIND *aux = cabeca;
        cabeca = cabeca->proximo;

        free(aux);

        lista->tamanho--;
    }

    lista->fim = NULL;
    lista->inicio = NULL;

}

int comparar(NOIND * n1, NOIND * n2) {
    int ret = 0;

    ret = strncasecmp(n1->chave, n2->chave, 28);

    //se for a mesma chave retorna o rrn menor
    if (ret == 0) {
        ret = n1->RRN - n2->RRN;
    }

    return ret;
}

int listaIndInserirOrdenado(LISTAINDICE * lista, char chave[28], int RRN) {
    NOIND *no_novo = (NOIND *) malloc(sizeof (NOIND));

    no_novo->RRN = RRN;
    strncpy(no_novo->chave, chave, 28);

    if (no_novo != NULL) {
        if (lista->tamanho == 0 || (comparar(no_novo, lista->inicio) < 0)) {
            //insere como primeiro elemento
            no_novo->proximo = lista->inicio;
            lista->inicio = no_novo;

            if (lista->fim == NULL) {
                lista->fim = no_novo;
            }

            lista->tamanho++;

            return 1;
        }



        NOIND *aux;

        if (no_novo != NULL) {
            aux = lista->inicio;

            while ((aux->proximo != NULL) && (comparar(aux->proximo, no_novo) < 0)) {
                aux = aux->proximo;
            }

            if (aux == lista->fim) {
                lista->fim = no_novo;
            }

            no_novo->proximo = aux->proximo;
            aux->proximo = no_novo;



            lista->tamanho++;

            return 1;
        }

    }

    return 0;
}

int listaIndInserirFim(LISTAINDICE * lista, char chave[28], int RRN) {

    NOIND *no_novo = (NOIND *) malloc(sizeof (NOIND));

    no_novo->RRN = RRN;
    strncpy(no_novo->chave, chave, 28);

    if (no_novo != NULL) {
        if (lista->tamanho == 0) {
            //insere como primeiro elemento
            no_novo->proximo = lista->inicio;
            lista->inicio = no_novo;
            lista->fim = no_novo;
            lista->tamanho++;

            return 1;
        } else {

            no_novo->proximo = NULL;
            lista->fim->proximo = no_novo;
            lista->fim = no_novo;
            lista->tamanho++;

            return 1;
        }
    }



    return 0;
}

int listaIndRemover(LISTAINDICE *lista, int RRN) {

    NOIND *no = lista->inicio;
    NOIND *aux = NULL;

    while (no != NULL && no->RRN != RRN) {
        aux = no;
        no = no->proximo;
    }

    if (no != NULL) {
        if (no == lista->inicio) {
            lista->inicio = no->proximo;
        } else
            aux->proximo = no->proximo;

        if (no == lista->fim) {
            lista->fim = aux;
        }

        lista->tamanho--;

        free(no);

        return 1;
    }

    return 0;
}

void listaIndImprimir(LISTAINDICE *lista) {

    NOIND *no = lista->inicio;
    while (no != NULL) {
        printf("[%s, %d]\n", no->chave, no->RRN);

        no = no->proximo;
    }
}
