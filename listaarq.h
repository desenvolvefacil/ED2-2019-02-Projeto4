/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   lista.h
 * Author: Carlao
 *
 * Created on 14 de Maio de 2019, 13:35
 */

#ifndef LISTAARQ_H
#define LISTAARQ_H

/* substitua "elemento" pelo tipo de dado utilizado */
typedef struct node {
    int nroInscricao;
    double nota;
    char data[11];
    int tamanhoCidade;
    char cidade[100];
    int tamanhoEscola;
    char nomeEscola[100];

    struct node *proximo;
} NOARQ;

typedef struct {
    NOARQ *inicio;
} LISTAARQ;

/********************************************************************
 * Metodos da Lista
 *******************************************************************/
LISTAARQ *listaArqCriar(void);

int listaArqInserirInicio(LISTAARQ *lista, int nroInscricao, double nota, char data[11], int tamanhoCidade, char cidade[100], int tamanhoEscola, char nomeEscola[100]);

void listaArqImprimir(LISTAARQ *lista);

void listaArqApagar(LISTAARQ *lista);

/******************************************************************
 * Metodos do Quick Sort
 *******************************************************************/


/**
 * Retorna o ultimo no da lista
 * @param cur
 * @return 
 */
NOARQ * qsArqUltimoNo(NOARQ *cur);

/**
 * Particiona a lista tomando o último elemento como o pivo
 * @param inicio
 * @param fim
 * @param novoInicio
 * @param novoFim
 * @return retorna o novo pivot
 */
NOARQ * qsArqParticionar(NOARQ *inicio, NOARQ *fim, NOARQ **novoInicio, NOARQ **novoFim);

/**
 * Faz a classificação exclusiva do no final
 * @param inicio
 * @param fim
 * @return novo inicio da lista
 */
NOARQ * qsArqRecursao(NOARQ *inicio, NOARQ *fim);

/**
 * Função para ordenar uma lista
 * @param cabeca da lista
 */
void qsArqOrdernarLista(NOARQ **cabeca);


#endif /* LISTAARQ_H */

