/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "listaarq.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

LISTAARQ *listaArqCriar(void) {
    LISTAARQ *lista = (LISTAARQ *) malloc(sizeof (LISTAARQ));

    lista->inicio = NULL;

    return lista;
}

int listaArqInserirInicio(LISTAARQ *lista, int nroInscricao, double nota, char data[11], int tamanhoCidade, char cidade[100], int tamanhoEscola, char nomeEscola[100]) {

    NOARQ *no_novo = (NOARQ *) malloc(sizeof (NOARQ));

    no_novo->nroInscricao = nroInscricao;
    no_novo->nota=nota;
    strncpy(no_novo->data,data,10);
    no_novo->tamanhoCidade = tamanhoCidade;
    if(tamanhoCidade){
        strncpy(no_novo->cidade,cidade,(tamanhoCidade-1));
    }
    no_novo->tamanhoEscola = tamanhoEscola;
    if(tamanhoEscola){
        strncpy(no_novo->nomeEscola,nomeEscola,(tamanhoEscola-1));
    }

    no_novo->proximo = lista->inicio;

    lista->inicio = no_novo;

    return 1;
}

void listaArqImprimir(LISTAARQ *lista) {

    if (lista != NULL) {
        NOARQ *no = lista->inicio;
        while (no != NULL) {
            printf("[%d]-> ", no->nroInscricao);
            
            //printf("%lf %s %d %s %d %s" ,no->nota,no->data,no->tamanhoCidade,no->cidade,no->tamanhoEscola,no->nomeEscola);
            
            no = no->proximo;
        }
    }
    printf("\n");
}

void listaArqApagar(LISTAARQ*lista) {
    if (lista != NULL) {
        NOARQ *cabeca = lista->inicio;

        while (cabeca != NULL) {
            NOARQ *aux = cabeca;
            cabeca = cabeca->proximo;

            free(aux);
        }

        lista->inicio = NULL;

    }
}

/*******
 * 
 * 
 * 
 * 
 * 
 * */


NOARQ *qsArqUltimoNo(NOARQ * cur) {
    while (cur != NULL && cur->proximo != NULL)
        cur = cur->proximo;
    return cur;
}

NOARQ *qsArqParticionar(NOARQ *incio, NOARQ *fim, NOARQ **novoInicio, NOARQ **novoFim) {
    NOARQ *pivot = fim;
    NOARQ *prev = NULL, *cur = incio, *cauda = pivot;

    // Durante a partição, tanto o inio como o final da lista podem mudar
    // valores atualizados nas variáveis novoInicio e novoFim
    while (cur != pivot) {
        if (cur->nroInscricao < pivot->nroInscricao) {
            //O primeiro nó que tem um valor menor que o pivô - torna-se o novo inicio
            if ((*novoInicio) == NULL)
                (*novoInicio) = cur;

            prev = cur;
            cur = cur->proximo;
        } else {
            // Mover o nó cur para a próxima cauda e mudar a cauda
            if (prev) {
                prev->proximo = cur->proximo;
            }
            NOARQ *tmp = cur->proximo;
            cur->proximo = NULL;
            cauda->proximo = cur;
            cauda = cur;
            cur = tmp;
        }
    }

    // Se os dados dinâmicos forem o menor elemento na lista atual o pivô se torna a cabeça
    if ((*novoInicio) == NULL)
        (*novoInicio) = pivot;

    // atualiza o novoFim para o último nó atual
    (*novoFim) = cauda;


    return pivot;
}

NOARQ *qsArqRecursao(NOARQ *inicio, NOARQ *fim) {
    //condição para rodar o metodo
    if (!inicio || inicio == fim) {
        return inicio;
    }

    NOARQ *novoInicio = NULL, *novoFim = NULL;

    // Particiona a lista, novoInicio e novoFim serão atualizados
    // pela função de partição
    NOARQ *pivot = qsArqParticionar(inicio, fim, &novoInicio, &novoFim);


    // Se pivot é o menor elemento - não há necessidade de de chamar a recursão par ao lado esquerdo do pivo.
    if (novoInicio != pivot) {
        //Definir o nó antes do nó tmp como NULL
        NOARQ *tmp = novoInicio;
        while (tmp->proximo != pivot)
            tmp = tmp->proximo;
        tmp->proximo = NULL;

        //chama o metodo recursivo para sublista antes do pivot
        novoInicio = qsArqRecursao(novoInicio, tmp);

        // Altera o próximo do último nó da metade esquerda para girar
        tmp = qsArqUltimoNo(novoInicio);
        tmp->proximo = pivot;
    }

    //chama o metodo recursivo para sublista após do pivot
    pivot->proximo = qsArqRecursao(pivot->proximo, novoFim);

    return novoInicio;
}

void qsArqOrdernarLista(NOARQ **inicio) {
    (*inicio) = qsArqRecursao(*inicio, qsArqUltimoNo(*inicio));
    return;
}
