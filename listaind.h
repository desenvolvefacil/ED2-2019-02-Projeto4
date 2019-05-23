/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   listaind.h
 * Author: Carlao
 *
 * Created on 23 de Maio de 2019, 15:11
 */

#ifndef LISTAIND_H
#define LISTAIND_H

typedef struct nodei {
    char chave[28];
    int RRN;

    struct nodei *proximo;
} NOIND;

typedef struct {
    NOIND *inicio;
    NOIND *fim;
    int tamanho;
} LISTAINDICE;

LISTAINDICE * listaIndCriar(void);
void listaIndApagar(LISTAINDICE *lista);
int listaIndInserirOrdenado(LISTAINDICE * lista, char chave[28], int RRN);
int listaIndInserirFim(LISTAINDICE * lista, char chave[28], int RRN);
int listaIndRemover(LISTAINDICE *lista, int RRN);
void listaIndImprimir(LISTAINDICE *lista);

#endif /* LISTAIND_H */

