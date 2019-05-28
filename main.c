/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: carlao
 * N USP 8901184
 * Created on 29 de Março de 2019, 13:03
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "listaarq.h"
#include "listaind.h"

#define TAMANHO_PAGINA 16000
#define TAMANHO_REGISTRO_DADO 80
#define TAMANHO_CHAVE_BUSCA 28
#define NOME_ARQUIVO_WB_SAVE "arquivoTrab1si.bin"
#define REMOVIDO '*'
#define NAO_REMOVIDO '-'
#define TAG_CAMPO_CIDADE '4'
#define TAG_CAMPO_ESCOLA '5'
#define ARQUIVO_ABERTO_ESCRITA '0'
#define ARQUIVO_FECHADO_ESCRITA '1'
#define MSG_ERRO "Falha no processamento do arquivo."
#define MODO_ESCRITA "wb"
#define MODO_EDICAO "r+b"

//nome dos campos para comparacao
#define NRO_INSCRICAO "nroInscricao"
#define NOTA "nota"
#define DATA "data"
#define CIDADE "cidade"
#define NOME_ESCOLA "nomeEscola"

#define NULO "NULO"

void escreverDadosEmArquivo(FILE * fileWb, int nroInscricao, double nota, char * data, int tamanhoCidade, char * cidade, int tamanhoEscola, char * nomeEscola) {

    //grava o valor de removido
    char removido = NAO_REMOVIDO;
    fwrite(&removido, sizeof (char), 1, fileWb);

    //grava o encadeamento
    int encadeamento = -1;
    fwrite(&encadeamento, sizeof (int), 1, fileWb);


    fwrite(&nroInscricao, sizeof (int), 1, fileWb);

    //grava no arquivo binario
    fwrite(&nota, sizeof (int), 1, fileWb);

    //grava a data no arquivo binario
    if (strlen(data) > 0) {
        fwrite(data, 10, 1, fileWb);
    } else {
        char aux[10] = "\0@@@@@@@@@";
        fwrite(&aux, 10, 1, fileWb);
    }

    //pega o tamanho dos campos fixo
    size_t totalBytes = 27;


    if (tamanhoCidade) {

        //salva o tamanho do campo
        fwrite(&tamanhoCidade, sizeof (int), 1, fileWb);

        //escreve a tag do campo
        char tagCampoCidade = TAG_CAMPO_CIDADE;
        fwrite(&tagCampoCidade, sizeof (char), 1, fileWb);

        //escreve a string cidade no arquivo
        fwrite(cidade, (tamanhoCidade - 1), 1, fileWb);

        // int (4)  + tamanhoCidade
        totalBytes += 4 + tamanhoCidade;
    }


    if (tamanhoEscola) {

        //salva o tamanho do campo
        fwrite(&tamanhoEscola, sizeof (tamanhoEscola), 1, fileWb);


        //salva a tag do campo
        char tagCampoEscola = TAG_CAMPO_ESCOLA;
        fwrite(&tagCampoEscola, sizeof (char), 1, fileWb);

        fwrite(nomeEscola, (tamanhoEscola - 1), 1, fileWb);

        //int tamanho(4) + char tag
        totalBytes += 4 + tamanhoEscola;
    }



    //for para setar @ nos bytes faltantes
    char arr = '@';
    int i;
    for (i = totalBytes; i < TAMANHO_REGISTRO_DADO; i++) {
        fwrite(&arr, 1, 1, fileWb);
    }

}

void escreverNoEmArquivo(FILE * wbFile, NOARQ * no) {

    //grava o valor de removido
    char removido = NAO_REMOVIDO;
    fwrite(&removido, sizeof (removido), 1, wbFile);

    //grava o encadeamento
    int encadeamento = -1;
    fwrite(&encadeamento, sizeof (int), 1, wbFile);

    //grava no arquivo binario
    fwrite(&no->nroInscricao, sizeof (int), 1, wbFile);

    //grava no arquivo binario
    fwrite(&no->nota, sizeof (double), 1, wbFile);

    //grava a data no arquivo binario
    if (strlen(no->data) > 0) {
        fwrite(&no->data, 10, 1, wbFile);
    } else {
        char aux[10] = "\0@@@@@@@@@";
        fwrite(&aux, 10, 1, wbFile);
    }





    //pega o tamanho dos campos fixo
    size_t totalBytes = 27;


    if (no->tamanhoCidade) {

        //salva o tamanho do campo
        fwrite(&no->tamanhoCidade, sizeof (int), 1, wbFile);

        //escreve a tag do campo
        char tagCampoCidade = TAG_CAMPO_CIDADE;
        fwrite(&tagCampoCidade, sizeof (char), 1, wbFile);

        //escreve a string cidade no arquivo
        fwrite(no->cidade, (no->tamanhoCidade - 1), 1, wbFile);

        //int (4) + tamanhoCidade
        totalBytes += 4 + no->tamanhoCidade;
    }


    if (no->tamanhoEscola) {

        //salva o tamanho do campo
        fwrite(&no->tamanhoEscola, sizeof (int), 1, wbFile);

        //salva a tag do campo
        char tagCampoEscola = TAG_CAMPO_ESCOLA;
        fwrite(&tagCampoEscola, sizeof (char), 1, wbFile);

        fwrite(no->nomeEscola, (no->tamanhoEscola - 1), 1, wbFile);

        //int (4) + tamanhoEscola
        totalBytes += 4 + no->tamanhoEscola;
    }


    char lixo = '@';
    //for para setar @ nos bytes faltantes
    for (; totalBytes < TAMANHO_REGISTRO_DADO; totalBytes++) {
        fwrite(&lixo, 1, 1, wbFile);
    }


}

void escreverCabecalho(FILE * wbFile) {
    //escreve os dados do cabecalho

    //grava o status como arquivo aberto pra escrita
    char status = ARQUIVO_ABERTO_ESCRITA;
    fwrite(&status, sizeof (status), 1, wbFile);

    //grava o topo da pilha
    int topoPilha = -1;
    fwrite(&topoPilha, sizeof (topoPilha), 1, wbFile);

    //grava os dados do campo 1
    char tagCampo1 = '1';
    fwrite(&tagCampo1, sizeof (tagCampo1), 1, wbFile);

    char desCampo1[55] = "numero de inscricao do participante do ENEM\0@@@@@@@@@@@";
    fwrite(&desCampo1, sizeof (desCampo1), 1, wbFile);

    //grava os dados do campo 2
    char tagCampo2 = '2';
    fwrite(&tagCampo2, sizeof (tagCampo2), 1, wbFile);

    char desCampo2[55] = "nota do participante do ENEM na prova de matematica\0@@@";
    fwrite(&desCampo2, sizeof (desCampo2), 1, wbFile);

    //grava os dados do campo 3
    char tagCampo3 = '3';
    fwrite(&tagCampo3, sizeof (tagCampo3), 1, wbFile);

    char desCampo3[55] = "data\0@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
    fwrite(&desCampo3, sizeof (desCampo3), 1, wbFile);

    //grava os dados do campo 4
    char tagCampo4 = '4';
    fwrite(&tagCampo4, sizeof (tagCampo4), 1, wbFile);

    char desCampo4[55] = "cidade na qual o participante do ENEM mora\0@@@@@@@@@@@@";
    fwrite(&desCampo4, sizeof (desCampo4), 1, wbFile);


    //grava os dados do campo 4
    char tagCampo5 = '5';
    fwrite(&tagCampo5, sizeof (tagCampo5), 1, wbFile);

    char desCampo5[55] = "nome da escola de ensino medio\0@@@@@@@@@@@@@@@@@@@@@@@@";
    fwrite(&desCampo5, sizeof (desCampo5), 1, wbFile);

    //for para completar com @ e deixar o cabeçalho em uma pagina só
    int i;


    char lixo = '@';
    // por algum motivo o fwrite so esta escrevendo ate 13000 caracteres por vez
    for (i = 285; i < TAMANHO_PAGINA; i++) {
        fwrite(&lixo, 1, 1, wbFile);
    }


    //break;
}

/**
 * Recebe uma lista e cria o arquivo de indice
 * @param nomeArquivo
 * @param lista
 * @return 1 se criado corretamente 0 em caso de erro
 */
int escreverIndicie(char * nomeArquivo, LISTAINDICE * lista) {

    FILE * wbFile = fopen(nomeArquivo, MODO_ESCRITA);

    if (wbFile) {
        char status = ARQUIVO_ABERTO_ESCRITA;
        fwrite(&status, 1, 1, wbFile);

        int nroRegistros = lista->tamanho;
        fwrite(&nroRegistros, sizeof (int), 1, wbFile);

        int i, total = TAMANHO_PAGINA - 5;
        char lixo = '@';

        //comprela o registro de cabeçalho com @
        for (i = 0; i < total; i++) {
            fwrite(&lixo, 1, 1, wbFile);
        }

        //escreve os dados
        NOIND * aux = lista->inicio;

        while (aux != NULL) {

            int tamanho = strlen(aux->chave) + 1;

            fwrite(&aux->chave, tamanho, 1, wbFile);

            //completa o espaço com @
            for (; tamanho < TAMANHO_CHAVE_BUSCA; tamanho++) {
                fwrite(&lixo, 1, 1, wbFile);
            }

            //salva o RRN da chave de busca
            fwrite(&aux->RRN, sizeof (int), 1, wbFile);

            aux = aux->proximo;
        }

        fclose(wbFile);
        return 1;
    }

    return 0;
}

/**
 * Recebe o path de um arquivo binario e mostra em tela
 * @param nomeArquivoBinario
 */
void binarioNaTela(char *nomeArquivoBinario) {

    /* Escolha essa função se você já fechou o ponteiro de arquivo 'FILE *'.
     *  Ela vai abrir de novo para leitura e depois fechar. */

    unsigned char *mb;
    unsigned long i;
    size_t fl;
    FILE *fs;
    if (nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
        fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela2): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar? Se você não fechou ele, pode usar a outra função, binarioNaTela1, ou pode fechar ele antes de chamar essa função!\n");
        return;
    }
    fseek(fs, 0, SEEK_END);
    fl = ftell(fs);
    fseek(fs, 0, SEEK_SET);
    mb = (unsigned char *) malloc(fl);
    fread(mb, 1, fl, fs);
    for (i = 0; i < fl; i += sizeof (unsigned char)) {
        printf("%02X ", mb[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    free(mb);
    fclose(fs);
}

/**
 * Recebe os dados de uma linha e imprime em tela
 * @param nroInscricao
 * @param nota
 * @param data
 * @param cidade
 * @param nomeEscola
 */
void imprimirLinhaEmTela(int nroInscricao, double nota, char * data, char * cidade, char * nomeEscola) {
    //imprime o número de inscrição
    printf("%d", nroInscricao);

    //se nota>=0 imprime
    if (nota >= 0) {
        printf(" %.1lf", nota);
    }

    //se tiver caracteres na data imprime
    if (strlen(data) > 0) {
        printf(" %s", data);
    }

    //verifica se tem cidade
    if (cidade != NULL && strlen(cidade) > 0) {
        printf(" %d %s", (int) strlen(cidade), cidade);
    }

    //verifica se tem escola
    if (nomeEscola != NULL && strlen(nomeEscola) > 0) {
        printf(" %d %s", (int) strlen(nomeEscola), nomeEscola);
    }

    printf("\n");
}

/**
 * Recebe o arquivo e RRN para ler o valor das variaveis
 * @param fileWb
 * @param RRN
 * @param removido
 * @param nroInscricao
 * @param nota
 * @param data
 * @param cidade
 * @param nomeEscola
 * @return nroInscricao caso seja lido ou 0 em caso de erro
 */
int lerLinha(FILE * fileWb, int RRN, char * removido, int * nroInscricao, double * nota, char * data, int * tamanhoCidade, char * cidade, int * tamanhoEscola, char * nomeEscola) {

    int encadeamento;

    int auxTamanho;
    char auxTagCampo;

    //posicao do proximo registro
    int pular = TAMANHO_PAGINA + RRN * TAMANHO_REGISTRO_DADO;

    //posicao atual do ponteiro no arquivo
    int posAtual = ftell(fileWb);

    //ajusta o tamanho do salto tirando o valor atual do ponteiro do registro a ser obtido
    pular -= posAtual;

    //tenta pular pra posição
    int seek = fseek(fileWb, pular, SEEK_CUR);

    //fssek retorna 0 caso seja lido sem erros
    if (!seek) {
        //pega o atributo para verificar se o registro esta excluido logicamente
        int read = fread(removido, sizeof (char), 1, fileWb);

        //verifica se o registro não esta excluido e imprime em tela
        if (read) {
            //le o valor de encadeamento
            fread(&encadeamento, sizeof (int), 1, fileWb);

            //le somente se o registro nao foi removido
            if (*removido == NAO_REMOVIDO) {

                //le o numero de inscricao
                fread(nroInscricao, sizeof (int), 1, fileWb);

                //le a nota
                fread(nota, sizeof (double), 1, fileWb);

                //le a data
                fread(data, 10, 1, fileWb);

                //tenta ler o tamanho da string
                read = fread(&auxTamanho, sizeof (int), 1, fileWb);

                if (read) {
                    //le a tag
                    fread(&auxTagCampo, sizeof (char), 1, fileWb);

                    auxTamanho--;

                    //verifica se é uma tagValida
                    if (auxTagCampo == TAG_CAMPO_CIDADE) {

                        //pega o tamanho
                        *tamanhoCidade = auxTamanho + 1;

                        //le o campo cidade
                        fread(cidade, auxTamanho, 1, fileWb);

                        read = fread(&auxTamanho, sizeof (int), 1, fileWb);

                        auxTamanho--;

                        if (read) {
                            //le a tag
                            fread(&auxTagCampo, sizeof (char), 1, fileWb);

                            //verifica se é uma tag validae
                            if (auxTagCampo == TAG_CAMPO_ESCOLA) {
                                //le o campo escolha
                                fread(nomeEscola, auxTamanho, 1, fileWb);
                                *tamanhoEscola = auxTamanho + 1;
                            }

                        }

                    } else if (auxTagCampo == TAG_CAMPO_ESCOLA) {
                        //le o campo escolha
                        fread(nomeEscola, auxTamanho, 1, fileWb);
                        *tamanhoEscola = auxTamanho + 1;
                    }
                }
            }
        }
    }

    //retorn o numero de inscricao ou 0
    return * nroInscricao;
}

/**
 * Abre um arquivo e verifica sua integridade
 * @param nomeArquivo
 * @return arquivo aberto ou nulo caso ocorra erro
 */
FILE * abrirArquivoBinarioLeitura(char * nomeArquivo) {
    FILE * file = fopen(nomeArquivo, "rb");

    if (file) {
        char status = ARQUIVO_ABERTO_ESCRITA;

        //le o primeiro char para verificar a integridade
        fread(&status, sizeof (char), 1, file);

        //se o arquivo estiver aberto, fecha e retorna nulo
        if (status == ARQUIVO_ABERTO_ESCRITA) {
            fclose(file);
            file = NULL;
        }

    }

    return file;
}

/**
 * Abre o arqvuio para escrita
 * Seta o status para ARQUIVO_ABERTO_ESCRITA
 * @param nomeArquivo
 * @return 
 */
FILE * abrirArquivoBinarioEscritra(char * nomeArquivo, char * modo) {
    FILE * file = fopen(nomeArquivo, modo);

    if (strncasecmp(modo, MODO_EDICAO, 3) && file) {
        char status = ARQUIVO_ABERTO_ESCRITA;

        //le o primeiro char para verificar a integridade
        fread(&status, sizeof (char), 1, file);

        //se o arquivo estiver aberto, fecha e retorna nulo
        if (status == ARQUIVO_ABERTO_ESCRITA) {
            fclose(file);
            file = NULL;
        } else {
            //volta o cursor pro inicio do arquivo
            fseek(file, -1, SEEK_CUR);
            //seta o flag de arquivo aberto pra escrita
            char status = ARQUIVO_ABERTO_ESCRITA;
            fwrite(&status, sizeof (char), 1, file);
        }

    }

    return file;
}

/**
 * Seta o status para ARQUIVO_FECHADO_ESCRITA
 * Fechar o arquivo
 * @param file
 */
void fecharArquivoBinarioEscrita(FILE * file) {
    if (file != NULL) {
        //move o ponteiro pro inicio do arquivo

        //atualiza o status para ARQUIVO_FECHADO_ESCRITA 1
        //posiciona o cursor pro inicio do arquivo
        fseek(file, 0, SEEK_SET);
        char status = ARQUIVO_FECHADO_ESCRITA;
        fwrite(&status, sizeof (char), 1, file);

        //fecha o arquivo binario
        fclose(file);
    }
}

/**
 * Le uma nova linha de comando
 * @return nova linha de comando
 */
char * lerComando() {
    //realoca memoria pra variavel comando
    char * comando = calloc(100, sizeof (char));
    fgets(comando, 100, stdin);

    //pega o tamnho do comando lido
    size_t ln = strlen(comando) - 1;

    //elimina o \n caso houver para ajudar e evitar erros no processamento
    //remove o enter e o carriage return
    if (comando[ln] == '\n') {
        comando[ln] = '\0';

        if (comando[ln - 1] == '\r') {
            comando[ln - 1] = '\0';
        }
    }

    return comando;
}

/**
 * Le um arquivo csv e mostra em tela
 * Entrada Modelo: 1 arquivo.csv
 * @param comando
 */
void opc1(char * comando) {

    char * nomeArquivo = strsep(&comando, "\0");

    //tenta abrir o arquivo csv em modo leitura
    FILE * file = fopen(nomeArquivo, "r");

    //se conseguiu abrir o arquivo
    if (file) {

        //cria o arquivo para salvar os dados em modo binario
        FILE * wbFile = fopen(NOME_ARQUIVO_WB_SAVE, "wb");

        char buff[200];

        //vez = 1 lendo dados do cacecalho
        int vez = 1;

        while (!feof(file)) {
            // Lê uma linha (inclusive com o '\n')
            char * result = fgets(buff, 200, file); // o 'fgets' lê até 199 caracteres ou até o '\n'

            //verifica se e uma linha valida
            if (result != NULL) {
                //printf("%s\n", result);

                size_t ln = strlen(result) - 1;
                //remove o enter e o carriage return
                if (result[ln] == '\n') {
                    result[ln] = '\0';

                    if (result[ln - 1] == '\r') {
                        result[ln - 1] = '\0';
                    }
                }

                if (vez == 1) {

                    //escreve os dados do cabecalho

                    //grava o status como arquivo aberto pra escrita
                    char status = ARQUIVO_ABERTO_ESCRITA;
                    fwrite(&status, sizeof (status), 1, wbFile);

                    //grava o topo da pilha
                    int topoPilha = -1;
                    fwrite(&topoPilha, sizeof (topoPilha), 1, wbFile);

                    //grava os dados do campo 1
                    char tagCampo1 = '1';
                    fwrite(&tagCampo1, sizeof (tagCampo1), 1, wbFile);

                    char desCampo1[55] = "numero de inscricao do participante do ENEM\0@@@@@@@@@@@";
                    fwrite(&desCampo1, sizeof (desCampo1), 1, wbFile);

                    //grava os dados do campo 2
                    char tagCampo2 = '2';
                    fwrite(&tagCampo2, sizeof (tagCampo2), 1, wbFile);

                    char desCampo2[55] = "nota do participante do ENEM na prova de matematica\0@@@";
                    fwrite(&desCampo2, sizeof (desCampo2), 1, wbFile);

                    //grava os dados do campo 3
                    char tagCampo3 = '3';
                    fwrite(&tagCampo3, sizeof (tagCampo3), 1, wbFile);

                    char desCampo3[55] = "data\0@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
                    fwrite(&desCampo3, sizeof (desCampo3), 1, wbFile);

                    //grava os dados do campo 4
                    char tagCampo4 = '4';
                    fwrite(&tagCampo4, sizeof (tagCampo4), 1, wbFile);

                    char desCampo4[55] = "cidade na qual o participante do ENEM mora\0@@@@@@@@@@@@";
                    fwrite(&desCampo4, sizeof (desCampo4), 1, wbFile);


                    //grava os dados do campo 4
                    char tagCampo5 = '5';
                    fwrite(&tagCampo5, sizeof (tagCampo5), 1, wbFile);

                    char desCampo5[55] = "nome da escola de ensino medio\0@@@@@@@@@@@@@@@@@@@@@@@@";
                    fwrite(&desCampo5, sizeof (desCampo5), 1, wbFile);

                    //for para completar com @ e deixar o cabeçalho em uma pagina só
                    int i;


                    char lixo = '@';
                    // escreve o lixo no resto do arquivo ate completar uma pagina
                    for (i = 285; i < TAMANHO_PAGINA; i++) {
                        fwrite(&lixo, 1, 1, wbFile);
                    }

                    //break;

                } else {
                    //grava o valor de removido
                    char removido = NAO_REMOVIDO;
                    fwrite(&removido, sizeof (removido), 1, wbFile);

                    //grava o encadeamento
                    int encadeamento = -1;
                    fwrite(&encadeamento, sizeof (encadeamento), 1, wbFile);

                    //começa a ler os dados
                    char * tmp = strsep(&result, ",");

                    //convert a string pra inteiro
                    int nroInscricao = atoi(tmp);

                    //grava no arquivo binario
                    fwrite(&nroInscricao, sizeof (int), 1, wbFile);

                    //pega  a nota
                    tmp = strsep(&result, ",");

                    double nota = -1;

                    if (strlen(tmp)) {
                        //caso haja texto, converte o mesmo pra double
                        nota = strtod(tmp, NULL);
                    }

                    //grava no arquivo binario
                    fwrite(&nota, sizeof (nota), 1, wbFile);

                    //seta data nula padrao
                    tmp = strsep(&result, ",");
                    char data[10] = "\0@@@@@@@@@";

                    //caso conseguiui ler campo data do arquivo copia para variavel
                    if (strlen(tmp)) {
                        strncpy(data, tmp, sizeof (data));
                    }

                    //grava a data no arquivo binario
                    fwrite(&data, sizeof (data), 1, wbFile);

                    //pega o tamanho dos campos fixo
                    size_t totalBytes = 27;

                    //tenta ler o campo cidade
                    char * cidade = strsep(&result, ",");

                    //add 1 para o \0
                    int tamanhoCidade = strlen(cidade) + 1;
                    if (tamanhoCidade > 1) {
                        //concatena com \0 no final d string
                        cidade = strncat(cidade, "\0", tamanhoCidade);

                        //add o char tagCampoCidade no tamanho do campo
                        tamanhoCidade++;

                        //salva o tamanho do campo
                        fwrite(&tamanhoCidade, sizeof (tamanhoCidade), 1, wbFile);

                        //remove o char tagCampoCidade para salvar a string cidade
                        tamanhoCidade--;

                        //escreve a tag do campo
                        char tagCampoCidade = TAG_CAMPO_CIDADE;
                        fwrite(&tagCampoCidade, sizeof (char), 1, wbFile);

                        //escreve a string cidade no arquivo
                        fwrite(cidade, tamanhoCidade, 1, wbFile);

                        // 5 = int (4) + tagCampoCIdade(1)
                        totalBytes += 5 + tamanhoCidade;
                    }

                    //tenta ler o nomeEscola
                    char * nomeEscola = strsep(&result, ",");

                    //soma 1 do \0
                    int tamanhoEscola = strlen(nomeEscola) + 1;

                    if (tamanhoEscola > 1) {

                        //concatena com \0
                        nomeEscola = strncat(nomeEscola, "\0", tamanhoEscola);

                        //soma 1 do char tagCampoEscola
                        tamanhoEscola++;

                        //salva o tamanho do campo
                        fwrite(&tamanhoEscola, sizeof (tamanhoEscola), 1, wbFile);

                        //remove 1 do tagCampoEscola
                        tamanhoEscola--;

                        //salva a tag do campo
                        char tagCampoEscola = TAG_CAMPO_ESCOLA;
                        fwrite(&tagCampoEscola, sizeof (char), 1, wbFile);

                        fwrite(nomeEscola, tamanhoEscola, 1, wbFile);

                        //5 = int tamanho + char tag
                        totalBytes += 5 + tamanhoEscola;
                    }


                    //escreve o lixo para completar o registro
                    int i, total = TAMANHO_REGISTRO_DADO - totalBytes;
                    //cria a variavel com tamanho que falta
                    char * lixo = calloc(total, 1);
                    //for para setar @ nos bytes faltantes
                    for (i = 0; i < total; i++) {
                        lixo[i] = '@';
                    }
                    //escreve em arquivo o lixo
                    fwrite(&lixo, total, 1, wbFile);
                    //free(lixo);
                    lixo = NULL;
                }
            }


            result = NULL;
            vez++;



        }

        //atualiza o status para ARQUIVO_FECHADO_ESCRITA 1
        //posiciona o cursor pro inicio do arquivo
        fseek(wbFile, 0, SEEK_SET);
        char status = ARQUIVO_FECHADO_ESCRITA;
        fwrite(&status, sizeof (char), 1, wbFile);

        //fecha o arquivo binario
        fclose(wbFile);

        //fecha o arquivo csv
        fclose(file);

        //mostra o nome do arquivo salvo
        printf(NOME_ARQUIVO_WB_SAVE);

    } else {
        printf("Falha no carregamento do arquivo.");
    }
}

/**
 * Le um arquivo binario e mostra em tela
 * Entrada Modelo: 2 arquivoTrablsi.bin
 * @param comando
 */
void opc2(char * comando) {

    char * nomeArquivo = strsep(&comando, "\0");

    //numero do registro que esta sendo lido ou RRN
    int vez = 0;

    //tenta abrir o arquivo pra modo leitura binario
    FILE *fileWb = abrirArquivoBinarioLeitura(nomeArquivo);

    if (fileWb) {

        while (!feof(fileWb)) {
            char removido;
            //int encadeamento;
            int nroInscricao = 0;
            double nota = -1;
            char data[11] = "\0";
            //data[10] = '\0';

            char cidade[100] = "\0"; // = NULL;
            char nomeEscola[100] = "\0"; // = NULL;

            int tamanhoCidade = 0;
            int tamanhoEscola = 0;

            //se conseguiu ler a linha
            if (lerLinha(fileWb, vez, &removido, &nroInscricao, &nota, data, &tamanhoCidade, cidade, &tamanhoEscola, nomeEscola)) {
                //se o registro não esta removido logicamente
                if (removido == NAO_REMOVIDO) {
                    imprimirLinhaEmTela(nroInscricao, nota, data, cidade, nomeEscola);
                }
            }

            vez++;
        }

        //fecha o arquivo
        fclose(fileWb);


        //Calcula qtas paginas foram acessadas
        int totalBytes = TAMANHO_REGISTRO_DADO * (vez);

        int totalPaginasAcessadas = totalBytes / TAMANHO_PAGINA;

        int diff = totalBytes % TAMANHO_PAGINA;

        totalPaginasAcessadas += (diff > 1) ? 1 : 0;

        //soma 1 da leitura do cabcelaho
        totalPaginasAcessadas++;

        printf("Número de páginas de disco acessadas: %d", totalPaginasAcessadas);
    } else {
        printf(MSG_ERRO);
    }
}

/**
 * Pesquisa dentro de um arquivo binario conforme parametros e mostra em tela
 * Entrada Modelo: 3 arquivoTrab1si.bin nroInscricao 332
 * Entrada Modelo: 3 arquivoTrab1si.bin nota 561.3
 * Entrada Modelo: 3 arquivoTrab1si.bin data 23/01/2017
 * Entrada Modelo: 3 arquivoTrab1si.bin cidade Recife
 * Entrada Modelo: 3 arquivoTrab1si.bin cidade Joao Pessoa    
 * Entrada Modelo: 3 arquivoTrab1si.bin nomeEscola FRANCISCO RIBEIRO CARRI
 * @param comando
 */
void opc3(char * comando) {

    char * nomeArquivo = strsep(&comando, " ");

    char * parametroNome = strsep(&comando, " ");

    char * parametroValor = strsep(&comando, "\0");

    int erro = 0;
    int imprimiu = 0;

    //verifica se é um parametro válido
    if (strcasecmp(parametroNome, NRO_INSCRICAO) == 0 || strcasecmp(parametroNome, NOTA) == 0 || strcasecmp(parametroNome, DATA) == 0 || strcasecmp(parametroNome, CIDADE) == 0 || strcasecmp(parametroNome, NOME_ESCOLA) == 0) {
        //printf("ok");

        int vez = 0;

        FILE *fileWb = abrirArquivoBinarioLeitura(nomeArquivo);

        if (fileWb) {

            while (!feof(fileWb)) {
                char removido;
                //int encadeamento;
                int nroInscricao = 0;
                double nota = -1;
                char data[11] = "\0";
                //data[10] = '\0';

                char cidade[100] = "\0"; // = NULL;
                char nomeEscola[100] = "\0"; // = NULL;

                int tamanhoCidade = 0;
                int tamanhoEscola = 0;

                //se conseguiu ler a linha
                if (lerLinha(fileWb, vez, &removido, &nroInscricao, &nota, data, &tamanhoCidade, cidade, &tamanhoEscola, nomeEscola)) {
                    if (removido == NAO_REMOVIDO) {


                        //verificar se o valor corresponde ao parametro
                        if (strcasecmp(parametroNome, NRO_INSCRICAO) == 0) {
                            int nroAux = atoi(parametroValor);

                            if (nroInscricao == nroAux) {
                                imprimirLinhaEmTela(nroInscricao, nota, data, cidade, nomeEscola);
                                imprimiu = 1;
                                vez += 2;
                                break;
                            }
                        }

                        if (strcasecmp(parametroNome, NOTA) == 0) {
                            double notaAux = strtod(parametroValor, NULL);

                            if (notaAux >= 0 && notaAux == nota) {
                                imprimirLinhaEmTela(nroInscricao, nota, data, cidade, nomeEscola);
                                imprimiu = 1;
                            }
                        }

                        if (strcasecmp(parametroNome, DATA) == 0) {
                            if (strcasecmp(parametroValor, data) == 0) {
                                imprimirLinhaEmTela(nroInscricao, nota, data, cidade, nomeEscola);
                                imprimiu = 1;
                            }
                        }

                        if (strcasecmp(parametroNome, CIDADE) == 0) {
                            if (strcasecmp(parametroValor, cidade) == 0) {
                                imprimirLinhaEmTela(nroInscricao, nota, data, cidade, nomeEscola);
                                imprimiu = 1;
                            }
                        }

                        if (strcasecmp(parametroNome, NOME_ESCOLA) == 0) {
                            if (strcasecmp(parametroValor, nomeEscola) == 0) {
                                imprimirLinhaEmTela(nroInscricao, nota, data, cidade, nomeEscola);
                                imprimiu = 1;
                            }
                        }

                    }
                }

                vez++;
            }


            if (imprimiu) {
                //Calcula qtas paginas foram acessadas
                //soma uma pagina para contar o cabecallho
                int totalBytes = TAMANHO_REGISTRO_DADO * (vez - 1) + TAMANHO_PAGINA;

                int totalPaginasAcessadas = totalBytes / TAMANHO_PAGINA;

                int diff = totalBytes % TAMANHO_PAGINA;

                totalPaginasAcessadas += (diff > 0) ? 1 : 0;

                printf("Número de páginas de disco acessadas: %d", totalPaginasAcessadas);
            } else {
                printf("Registro inexistente.");
            }


        } else {
            erro == 1;
        }

    } else {
        erro = 1;
    }


    if (erro) {
        printf(MSG_ERRO);
    }
}

/**
 * Pesquisa um registro pelo RRN e mostra em tela
 * Entrada Modelo: 4 arquivoTrab1si.bin 4999
 * @param comando
 */
void opc4(char * comando) {
    char * nomeArquivo = strsep(&comando, " ");

    int RRN = -1;
    RRN = atoi(strsep(&comando, "\0"));

    int erro = 0;


    if (RRN >= 0) {

        FILE *fileWb = abrirArquivoBinarioLeitura(nomeArquivo);

        if (fileWb) {

            char removido;
            //int encadeamento;
            int nroInscricao = 0;
            double nota = -1;
            char data[11] = "\0";
            //data[10] = '\0';

            char cidade[100] = "\0"; // = NULL;
            char nomeEscola[100] = "\0"; // = NULL;

            int tamanhoCidade = 0;
            int tamanhoEscola = 0;

            //se conseguiu ler a linha
            if (lerLinha(fileWb, RRN, &removido, &nroInscricao, &nota, data, &tamanhoCidade, cidade, &tamanhoEscola, nomeEscola)) {
                if (removido == NAO_REMOVIDO) {
                    imprimirLinhaEmTela(nroInscricao, nota, data, cidade, nomeEscola);
                    //2 cabcalho + seek direto pro registro
                    printf("Número de páginas de disco acessadas: 2");
                } else {
                    printf("Registro inexistente.");
                }
            } else {
                printf("Registro inexistente.");
            }



        } else {
            erro = 1;
        }



    } else {
        erro = 1;
    }

    if (erro) {
        printf(MSG_ERRO);
    }

}

/**
 * Esclui registros conforme parametros informados
 * Entrada Modelo: 

5 arquivoTrab1si.bin 2
nroInscricao 13893
cidade "Coimbra"

 * Entrada Modelo: 

5 arquivoTrab1si.bin 1
nomeEscola "DONIZETTI TAVARES DE LI"

 * @param comando
 */
void opc5(char * comando) {

    char * nomeArquivo = strsep(&comando, " ");

    int erro = 0;

    int numeroIteracoes = 0;
    numeroIteracoes = atoi(strsep(&comando, "\0"));

    FILE * fileWb = abrirArquivoBinarioEscritra(nomeArquivo, MODO_EDICAO);

    if (fileWb) {

        //pega a posicao do topo para poder ser atualizada posteriormente
        int posTopoPilha = ftell(fileWb);

        //pega o topo da pilha de removidos
        int topoPilha = 0;
        fread(&topoPilha, sizeof (int), 1, fileWb);

        int vez;
        //for para ler os comandos a serem executados
        for (vez = 0; vez < numeroIteracoes; vez++) {

            //le uma nova linha
            comando = lerComando();

            //pega o campo a ser comparado
            char * parametroNome = strsep(&comando, " ");
            //pega o valor do campo a ser comparado
            char * parametroValor = strsep(&comando, "\"");

            //remove " do valor do campo
            if (strcasecmp(parametroNome, NOME_ESCOLA) == 0 || strcasecmp(parametroNome, CIDADE) == 0) {
                parametroValor = strsep(&comando, "\"");
            }

            //verifica se a data esta vindo com aspas
            if (strcasecmp(parametroNome, DATA) == 0) {
                //pega o valor entre aspas
                if (strcasecmp(parametroValor, "") == 0) {
                    parametroValor = strsep(&comando, "\"");
                }
            }

            //verifica se é um parametro válido
            if (strcasecmp(parametroNome, NRO_INSCRICAO) == 0 || strcasecmp(parametroNome, NOTA) == 0 || strcasecmp(parametroNome, DATA) == 0 || strcasecmp(parametroNome, CIDADE) == 0 || strcasecmp(parametroNome, NOME_ESCOLA) == 0) {
                //printf("ok");

                int RRN = 0;

                //voltao o ponteiro pro inicio do arquivo
                rewind(fileWb);

                while (!feof(fileWb)) {
                    char removido;
                    //int encadeamento;
                    int nroInscricao = 0;
                    double nota = -1;
                    char data[11] = "\0";
                    //data[10] = '\0';

                    char cidade[100] = "\0"; // = NULL;
                    char nomeEscola[100] = "\0"; // = NULL;

                    int excluir = 0;
                    int parar = 0;


                    int tamanhoCidade = 0;
                    int tamanhoEscola = 0;

                    //se conseguiu ler a linha
                    if (lerLinha(fileWb, RRN, &removido, &nroInscricao, &nota, data, &tamanhoCidade, cidade, &tamanhoEscola, nomeEscola)) {
                        if (removido == NAO_REMOVIDO) {


                            //verificar se o valor corresponde ao parametro
                            if (strcasecmp(parametroNome, NRO_INSCRICAO) == 0) {
                                int nroAux = atoi(parametroValor);

                                if (nroInscricao == nroAux) {
                                    excluir = 1;
                                    parar = 1;
                                }
                            }

                            if (strcasecmp(parametroNome, NOTA) == 0) {
                                double notaAux = strtod(parametroValor, NULL);

                                if (notaAux >= 0 && notaAux == nota) {
                                    excluir = 1;
                                }
                            }

                            if (strcasecmp(parametroNome, DATA) == 0) {
                                if (strcasecmp(parametroValor, data) == 0) {
                                    excluir = 1;
                                }
                            }

                            if (strcasecmp(parametroNome, CIDADE) == 0) {
                                if (strcasecmp(parametroValor, cidade) == 0) {
                                    excluir = 1;
                                }
                            }

                            if (strcasecmp(parametroNome, NOME_ESCOLA) == 0) {
                                if (strcasecmp(parametroValor, nomeEscola) == 0) {
                                    excluir = 1;
                                }
                            }

                        }
                    }


                    //faz a esclusao do resgistro
                    if (excluir) {
                        //remove o registro logicamente

                        //posicao do proximo registro
                        int pular = TAMANHO_PAGINA + RRN * TAMANHO_REGISTRO_DADO;

                        //posicao atual do ponteiro no arquivo
                        int posAtual = ftell(fileWb);

                        //ajusta o tamanho do salto tirando o valor atual do ponteiro do registro a ser obtido
                        pular -= posAtual;

                        //tenta pular pra posição
                        int seek = fseek(fileWb, pular, SEEK_CUR);

                        //escreve o registro como nulo
                        char status = REMOVIDO;
                        fwrite(&status, sizeof (char), 1, fileWb);

                        //escreve o encadeamento
                        fwrite(&topoPilha, sizeof (int), 1, fileWb);

                        //escreve @ nos campos restantes
                        int i;
                        //status 1 + encadeamento 4 = 5
                        for (i = 5; i < TAMANHO_REGISTRO_DADO; i++) {
                            char arr = '@';
                            fwrite(&arr, sizeof (char), 1, fileWb);
                        }

                        //atualiza o topo da pilha com o RRN atual
                        topoPilha = RRN;

                        //move o ponteiro pra posição de cabeçalho topo da lista
                        fseek(fileWb, posTopoPilha, SEEK_SET);
                        fwrite(&topoPilha, sizeof (int), 1, fileWb);



                        //verifica se deve parar
                        if (parar) {
                            break;
                        }
                    }



                    RRN++;
                }


            } else {
                erro = 1;
            }



        }



        fecharArquivoBinarioEscrita(fileWb);



    } else {
        erro = 1;
    }

    if (erro) {
        printf(MSG_ERRO);
    } else {
        binarioNaTela(nomeArquivo);
    }
}

/**
 * Inser um novo registro conforme valores informados
 * Entrada Modelo:
 
6 arquivoTrab1si.bin 5
78 408.02 "01/08/2016" "CAMPINA GRANDE" NULO
79 109.98 NULO NULO "ESCOLA DE ESTUDO PRIMARIO"
80 NULO NULO "Paradise City" NULO
81 NULO NULO NULO NULO
82 NULO "10/10/2010" "CID CID" "ESC ESC"

6 arquivoTrab1si.bin 4
11 50.10 "10/10/2010" "C D" "E N"
12 50.10 "10/10/2010" "C D" "E N" 
13 50.10 "10/10/2010" "C D" "E N" 
14 50.10 "10/10/2010" "C D" "E N" 

 * @param comando
 */
void opc6(char * comando) {

    char * nomeArquivo = strsep(&comando, " ");

    int erro = 0;

    int numeroIteracoes = 0;
    numeroIteracoes = atoi(strsep(&comando, "\0"));

    FILE * fileWb = abrirArquivoBinarioEscritra(nomeArquivo, MODO_EDICAO);

    if (fileWb) {

        //pega  a posicao do topo
        int posTopoPilha = ftell(fileWb);
        //pega o topo da pilha de removidos
        int topoPilha = -10;
        fread(&topoPilha, sizeof (int), 1, fileWb);





        int vez;
        //for para ler os comandos a serem executados
        for (vez = 0; vez < numeroIteracoes; vez++) {


            //posiciona o ponteiro no local correo para a nova insercao

            //insere no final
            if (topoPilha == -1) {
                //posiciona o ponteiro pro final do arquivo
                fseek(fileWb, 0, SEEK_END);
            } else {
                //pega a posicao do registro removido
                int posicao = topoPilha * TAMANHO_REGISTRO_DADO + TAMANHO_PAGINA;

                //soma 1 do byte de statys
                fseek(fileWb, posicao + 1, SEEK_SET);

                //pega o topo atual
                fread(&topoPilha, sizeof (int), 1, fileWb);

                //volta 5 bytes que foram andados e deixa o ponteiro na posição correta para inseção
                fseek(fileWb, -5, SEEK_CUR);

                //int pos = ftell(fileWb);
                //int a = 10;
            }

            //começa a escrever os dados no arquivo
            comando = lerComando();



            //grava o valor de removido
            char removido = NAO_REMOVIDO;
            fwrite(&removido, sizeof (char), 1, fileWb);

            //grava o encadeamento
            int encadeamento = -1;
            fwrite(&encadeamento, sizeof (int), 1, fileWb);


            //começa a ler os dados
            char * tmp = strsep(&comando, " ");
            //convert a string pra inteiro
            int nroInscricao = atoi(tmp);
            //grava no arquivo binario
            fwrite(&nroInscricao, sizeof (int), 1, fileWb);


            //pega  a nota
            tmp = strsep(&comando, " ");

            double nota = -1;


            if (strcasecmp(tmp, NULO) != 0) {
                //caso haja texto, converte o mesmo pra double
                nota = strtod(tmp, NULL);
            }

            //grava no arquivo binario
            fwrite(&nota, sizeof (nota), 1, fileWb);

            //separa o comando por " ou espaco vazio
            tmp = strsep(&comando, "\" ");
            //seta data nula padrao
            char data[10] = "\0@@@@@@@@@";


            //caso conseguiui ler campo data do arquivo copia para variavel

            if (strcasecmp(tmp, NULO) != 0) {
                //**caso venha com aspas
                if (strlen(tmp) < 10) {
                    tmp = strsep(&comando, "\"");
                    strsep(&comando, " \"");
                }
                strncpy(data, tmp, sizeof (data));
            }

            //grava a data no arquivo binario
            fwrite(&data, sizeof (data), 1, fileWb);

            //pega o tamanho dos campos fixo
            size_t totalBytes = 27;


            //tenta ler o campo cidade
            char * cidade = strsep(&comando, " \"");


            if (strcasecmp(cidade, NULO) != 0) {

                if (strlen(cidade) == 0) {
                    cidade = strsep(&comando, "\"");
                }
                //removeAspas(cidade);

                //add 1 para o \0
                int tamanhoCidade = strlen(cidade) + 1;

                //concatena com \0 no final d string
                cidade = strncat(cidade, "\0", tamanhoCidade);

                //add o char tagCampoCidade no tamanho do campo
                tamanhoCidade++;

                //salva o tamanho do campo
                fwrite(&tamanhoCidade, sizeof (int), 1, fileWb);

                //remove o char tagCampoCidade para salvar a string cidade
                tamanhoCidade--;

                //escreve a tag do campo
                char tagCampoCidade = TAG_CAMPO_CIDADE;
                fwrite(&tagCampoCidade, sizeof (char), 1, fileWb);

                //escreve a string cidade no arquivo
                fwrite(cidade, tamanhoCidade, 1, fileWb);

                // 5 = int (4) + tagCampoCIdade(1)
                totalBytes += 5 + tamanhoCidade;

                strsep(&comando, " ");
            }



            //tenta ler o nomeEscola
            char * nomeEscola = strsep(&comando, "\"");



            if (strcasecmp(nomeEscola, NULO) != 0) {
                //pega stering restante
                nomeEscola = strsep(&comando, "\"");


                //soma 1 do \0
                int tamanhoEscola = strlen(nomeEscola) + 1;

                //concatena com \0
                nomeEscola = strncat(nomeEscola, "\0", tamanhoEscola);

                //soma 1 do char tagCampoEscola
                tamanhoEscola++;

                //salva o tamanho do campo
                fwrite(&tamanhoEscola, sizeof (tamanhoEscola), 1, fileWb);

                //remove 1 do tagCampoEscola
                tamanhoEscola--;

                //salva a tag do campo
                char tagCampoEscola = TAG_CAMPO_ESCOLA;
                fwrite(&tagCampoEscola, sizeof (char), 1, fileWb);

                fwrite(nomeEscola, tamanhoEscola, 1, fileWb);

                //5 = int tamanho + char tag
                totalBytes += 5 + tamanhoEscola;
            }



            //for para setar @ nos bytes faltantes
            char arr = '@';
            int i;
            for (i = totalBytes; i < TAMANHO_REGISTRO_DADO; i++) {
                fwrite(&arr, 1, 1, fileWb);
            }


            //salva o topo atual dos escluidos
            fseek(fileWb, posTopoPilha, SEEK_SET);
            fwrite(&topoPilha, sizeof (int), 1, fileWb);
        }




        fecharArquivoBinarioEscrita(fileWb);
    } else {
        erro = 1;
    }



    if (erro) {
        printf(MSG_ERRO);
    } else {
        binarioNaTela(nomeArquivo);
    }
}

/**
 * Atualiza um campo de um registro conforme seu RRN
 * Entrada Modelo: 
 * 
7 arquivoTrab1si.bin 4
100 nota 20
6 nota 200.5
1 nroInscricao 10
5 data "07/07/2007"

7 arquivoTrab1si.bin 3
0 cidade NULO
5 cidade NULO
2 cidade NULO

7 arquivoTrab1si.bin 1
2 nomeEscola "Escola Nova"

2 arquivoTrab1si.bin
 * 
 * @param comando
 */
void opc7(char * comando) {

    char * nomeArquivo = strsep(&comando, " ");
    int erro = 0;
    int numeroIteracoes = 0;
    numeroIteracoes = atoi(strsep(&comando, "\0"));

    FILE * fileWb = abrirArquivoBinarioEscritra(nomeArquivo, MODO_EDICAO);

    if (fileWb) {
        int vez;
        //for para ler os comandos a serem executados
        for (vez = 0; vez < numeroIteracoes; vez++) {

            //voltao o ponteiro pro inicio do arquivo
            rewind(fileWb);

            //começa a escreve os dados no arquivo
            comando = lerComando();

            //pegando os parametros
            int RRN = atoi(strsep(&comando, " "));

            char * parametroNome = strsep(&comando, " ");

            char * parametroValor = strsep(&comando, "\"");

            //caso seja parametro com " faz um novo deslocamento
            if (parametroValor[0] == '\0') {
                parametroValor = strsep(&comando, "\"");
            }

            //campos para busca
            char removido;
            //int encadeamento;
            int nroInscricaoAtual = 0;
            double notaAtual = -1;
            char dataAtual[11] = "\0";
            //data[10] = '\0';

            char cidadeAtual[100] = "\0"; // = NULL;
            char nomeEscolaAtual[100] = "\0"; // = NULL;

            int tamanhoCidade = 0;
            int tamanhoEscola = 0;

            //se conseguiu ler a linha
            if (lerLinha(fileWb, RRN, &removido, &nroInscricaoAtual, &notaAtual, dataAtual, &tamanhoCidade, cidadeAtual, &tamanhoEscola, nomeEscolaAtual)) {

                //se o registro não esta removido logicamente
                if (removido == NAO_REMOVIDO) {

                    //inicio do arquivo
                    int inicioRegistro = TAMANHO_PAGINA + TAMANHO_REGISTRO_DADO * RRN;
                    //soma os bytes de status(1) e encadeamento(4)
                    inicioRegistro += 1 + 4;

                    //posicao atual
                    int posAtual = ftell(fileWb);

                    int salto = inicioRegistro - posAtual;

                    //verifica qual o parametro deve ser atualizado
                    if (strcasecmp(parametroNome, NRO_INSCRICAO) == 0) {
                        //volta o ponteiro para a posição do campo
                        fseek(fileWb, salto, SEEK_CUR);


                        int novoNroInscricao = atoi(parametroValor);
                        //escreve diretamente no arquivo
                        fwrite(&novoNroInscricao, sizeof (int), 1, fileWb);

                    } else if (strcasecmp(parametroNome, NOTA) == 0) {

                        //soma o campo nroinscricao
                        salto += 4;
                        //volta o ponteiro para a posição do campo
                        fseek(fileWb, salto, SEEK_CUR);

                        double novaNota = -1;
                        //se não for nulo
                        if (strcasecmp(parametroValor, NULO) != 0) {
                            novaNota = strtod(parametroValor, NULL);
                        }

                        fwrite(&novaNota, sizeof (double), 1, fileWb);

                    } else if (strcasecmp(parametroNome, DATA) == 0) {

                        //soma o campo nroinscricao + nota
                        salto += 4 + 8;
                        //volta o ponteiro para a posição do campo
                        fseek(fileWb, salto, SEEK_CUR);

                        char novaData[10] = "\0@@@@@@@@@";

                        if (strcasecmp(parametroValor, NULO) != 0) {
                            strncpy(novaData, parametroValor, sizeof (novaData));
                        }

                        //grava a data no arquivo binario
                        fwrite(&novaData, sizeof (novaData), 1, fileWb);


                    } else if (strcasecmp(parametroNome, CIDADE) == 0) {
                        //soma o campo nroinscricao + nota + data
                        salto += 4 + 8 + 10;
                        //volta o ponteiro para a posição do campo
                        fseek(fileWb, salto, SEEK_CUR);

                        //verifica se vai fazer deslocamento do campo cidade
                        int deslocaEscola = 0;

                        int tamanhoCidadeAtual = strlen(cidadeAtual);

                        //tota de bytes escritos no registro até o momento
                        int bytes = 27;

                        //for nulo e tiver algo escrito
                        if (strcasecmp(parametroValor, NULO) == 0) {

                            if (tamanhoCidadeAtual) {
                                //desloca os dados da escola pra frente
                                deslocaEscola = 1;
                            }
                        } else {

                            //salva os dados da cidade atual
                            //add 1 para o \0
                            int tamanhoCidadeNova = strlen(parametroValor) + 1;
                            if (tamanhoCidadeNova > 1) {

                                //concatena com \0 no final d string
                                char * cidadeNova = strncat(parametroValor, "\0", tamanhoCidadeNova);

                                //add o char tagCampoCidade no tamanho do campo
                                tamanhoCidadeNova++;

                                //salva o tamanho do campo
                                fwrite(&tamanhoCidadeNova, sizeof (tamanhoCidadeNova), 1, fileWb);

                                //remove o char tagCampoCidade para salvar a string cidade
                                tamanhoCidadeNova--;

                                //escreve a tag do campo
                                char tagCampoCidade = TAG_CAMPO_CIDADE;
                                fwrite(&tagCampoCidade, sizeof (char), 1, fileWb);

                                //escreve a string cidade no arquivo
                                fwrite(cidadeNova, tamanhoCidadeNova, 1, fileWb);

                                bytes += 5 + tamanhoCidadeNova;

                                //remove 1 do \0
                                if (tamanhoCidadeAtual != (tamanhoCidadeNova - 1)) {
                                    //se o tamanho for diferente desloca a cidade
                                    deslocaEscola = 1;
                                }
                            }
                        }

                        //faz o deslocamento do campo cidade
                        if (deslocaEscola) {
                            //verifica se existe nomeEscola
                            //soma 1 do \0
                            int tamanhoEscolaAtual = strlen(nomeEscolaAtual) + 1;
                            if (tamanhoEscolaAtual > 1) {

                                //grava os dados da escola pra frente do arquivo

                                //add o char tagCampoCidade no tamanho do campo
                                tamanhoEscolaAtual++;

                                //salva o tamanho do campo
                                fwrite(&tamanhoEscolaAtual, sizeof (tamanhoEscolaAtual), 1, fileWb);

                                //remove o char tagCampoCidade para salvar a string cidade
                                tamanhoEscolaAtual--;

                                //escreve a tag do campo
                                char tagCampoEscola = TAG_CAMPO_ESCOLA;
                                fwrite(&tagCampoEscola, sizeof (char), 1, fileWb);

                                //escreve a string cidade no arquivo
                                fwrite(nomeEscolaAtual, tamanhoEscolaAtual, 1, fileWb);

                                //soma os bytes deslocados
                                bytes += 5 + tamanhoEscolaAtual;
                            }

                            //completa com nulos
                            char arr = '@';
                            int j;

                            //completa com nulos caso 
                            for (j = bytes; j < TAMANHO_REGISTRO_DADO; j++) {
                                fwrite(&arr, 1, 1, fileWb);
                            }
                        }

                    } else if (strcasecmp(parametroNome, NOME_ESCOLA) == 0) {

                        //verifica se tem cidade atual
                        int tamanhoCidadeAtual = strlen(cidadeAtual);

                        //tota de bytes escritos no registro até o momento
                        int bytes = 27;

                        if (tamanhoCidadeAtual > 0) {
                            //soma tamanho(4) + tagCampo(1) + \0 (1)
                            tamanhoCidadeAtual += 6;

                            bytes += 5 + tamanhoCidadeAtual;
                        }

                        //soma o campo nroinscricao + nota + data
                        salto += 4 + 8 + 10 + tamanhoCidadeAtual;
                        //volta o ponteiro para a posição do campo
                        fseek(fileWb, salto, SEEK_CUR);


                        if (strcasecmp(parametroValor, NULO) != 0) {

                            //grava os dados da escola pra frente do arquivo

                            int tamanhoescolaNova = strlen(parametroValor) + 1;

                            //concatena com \0 no final d string
                            char * escolaNova = strncat(parametroValor, "\0", tamanhoescolaNova);

                            //add o char tagCampoCidade no tamanho do campo
                            tamanhoescolaNova++;

                            //salva o tamanho do campo
                            fwrite(&tamanhoescolaNova, sizeof (tamanhoescolaNova), 1, fileWb);

                            //remove o char tagCampoCidade para salvar a string cidade
                            tamanhoescolaNova--;

                            //escreve a tag do campo
                            char tagCampoEscola = TAG_CAMPO_ESCOLA;
                            fwrite(&tagCampoEscola, sizeof (char), 1, fileWb);

                            //escreve a string cidade no arquivo
                            fwrite(escolaNova, tamanhoescolaNova, 1, fileWb);

                            //soma os bytes deslocados
                            bytes += 5 + tamanhoescolaNova;
                        }

                        //completa com nulos
                        char arr = '@';
                        int j;

                        //completa com nulos caso 
                        for (j = bytes; j < TAMANHO_REGISTRO_DADO; j++) {
                            fwrite(&arr, 1, 1, fileWb);
                        }







                    }


                }
            }






        }

        //fecha o arquivo e seta o status como fechado
        fecharArquivoBinarioEscrita(fileWb);


    } else {
        erro = 1;
    }


    if (erro) {
        printf(MSG_ERRO);
    } else {
        binarioNaTela(nomeArquivo);
    }
}

/**
 * Realiza a ordenação interna dos dados
 * Entrada Modelo:
 
8 arquivoTrab1si.bin saida.bin

8 arquivoEntrada.bin arquivoSaida.bin
 
 * @param comando
 */
void opc8(char * comando) {
    char * nomeArquivoEntrada = strsep(&comando, " ");
    char * nomeArquivoSaida = strsep(&comando, " ");

    //cria a lista para ordenar
    LISTAARQ *lista = listaArqCriar();

    //faz a leitura do arquivo de entrada
    FILE * arquivoEntrada = abrirArquivoBinarioLeitura(nomeArquivoEntrada);


    if (arquivoEntrada) {

        int RRN = 0;

        while (!feof(arquivoEntrada)) {
            char removido;
            //int encadeamento;
            int nroInscricao = 0;
            double nota = -1;
            char data[11] = "\0";
            //data[10] = '\0';

            char cidade[100] = "\0"; // = NULL;
            char nomeEscola[100] = "\0"; // = NULL;

            int tamanhoCidade = 0;
            int tamanhoEscola = 0;

            //se conseguiu ler a linha
            if (lerLinha(arquivoEntrada, RRN, &removido, &nroInscricao, &nota, data, &tamanhoCidade, cidade, &tamanhoEscola, nomeEscola)) {
                //se o registro não esta removido logicamente
                if (removido == NAO_REMOVIDO) {
                    listaArqInserirInicio(lista, nroInscricao, nota, data, tamanhoCidade, cidade, tamanhoEscola, nomeEscola);
                }
            }

            RRN++;
        }

        //fecha o arquivo
        fclose(arquivoEntrada);

        //listaImprimir(lista);

        qsArqOrdernarLista(&lista->inicio);

        //listaImprimir(lista);

        //cria o novo arquivo
        FILE * arquivoSaida = fopen(nomeArquivoSaida, "wb");

        if (arquivoSaida) {
            //escreve o cabecalho
            escreverCabecalho(arquivoSaida);

            //escreve a lista ordenada no arquivo
            NOARQ * aux = lista->inicio;

            while (aux != NULL) {

                escreverNoEmArquivo(arquivoSaida, aux);

                aux = aux->proximo;
            }

            //fecha o arquivo binario
            fecharArquivoBinarioEscrita(arquivoSaida);

            //apaga a lista
            listaArqApagar(lista);

            binarioNaTela(nomeArquivoSaida);

        } else {
            printf(MSG_ERRO);
        }


    } else {
        printf(MSG_ERRO);
    }
}

/**
 * Realize a operação cosequencial de merging (união) de dois arquivos de dados,
 * considerando os valores do campo nroInscricao
 * 
 * Entrada Modelo:
 
9 arquivoEntrada1.bin arquivoEntrada2.bin arquivoSaida.bin
 
 * @param comando
 */
void opc9(char * comando) {

    char * nomeArq1 = strsep(&comando, " ");

    FILE * arq1 = abrirArquivoBinarioLeitura(nomeArq1);

    if (arq1) {

        char * nomeArq2 = strsep(&comando, " ");

        FILE * arq2 = abrirArquivoBinarioLeitura(nomeArq2);

        if (arq2) {

            char * nomeArquivoSaida = strsep(&comando, " ");

            FILE * arqSaida = fopen(nomeArquivoSaida, "wb");
            //escreve o cabeçalho
            escreverCabecalho(arqSaida);

            int RRN1 = 0, RRN2 = 0;


            //enquanto não chegar no fim dos dois arquivos
            while (!feof(arq1) && !feof(arq2)) {
                char removido1, removido2;
                //int encadeamento;
                int nroInscricao1 = 0, nroInscricao2 = 0;
                double nota1 = -1, nota2 = -1;
                char data1[11] = "\0", data2[11];
                //data[10] = '\0';

                char cidade1[100] = "\0", cidade2[100] = "\0"; // = NULL;
                char nomeEscola1[100] = "\0", nomeEscola2[100] = "\0"; // = NULL;

                int tamanhoCidade1 = 0, tamanhoCidade2 = 0;
                int tamanhoEscola1 = 0, tamanhoEscola2 = 0;

                if (lerLinha(arq1, RRN1, &removido1, &nroInscricao1, &nota1, data1, &tamanhoCidade1, cidade1, &tamanhoEscola1, nomeEscola1)) {




                    if (lerLinha(arq2, RRN2, &removido2, &nroInscricao2, &nota2, data2, &tamanhoCidade2, cidade2, &tamanhoEscola2, nomeEscola2)) {




                        if (removido1 == REMOVIDO) {
                            RRN1++;
                        }
                        if (removido2 == REMOVIDO) {
                            RRN2++;
                        }

                        if (removido1 == NAO_REMOVIDO && removido2 == NAO_REMOVIDO) {
                            if (nroInscricao1 < nroInscricao2) {

                                escreverDadosEmArquivo(arqSaida, nroInscricao1, nota1, data1, tamanhoCidade1, cidade1, tamanhoEscola1, nomeEscola1);

                                RRN1++;

                            } else if (nroInscricao1 == nroInscricao2) {

                                escreverDadosEmArquivo(arqSaida, nroInscricao1, nota1, data1, tamanhoCidade1, cidade1, tamanhoEscola1, nomeEscola1);

                                RRN1++;
                                RRN2++;

                            } else if (nroInscricao1 > 0 && nroInscricao2 > 0) {

                                escreverDadosEmArquivo(arqSaida, nroInscricao2, nota2, data2, tamanhoCidade2, cidade2, tamanhoEscola2, nomeEscola2);

                                RRN2++;

                            }
                        }
                    }
                }

            }

            //se ainda sobrou dados no arquivo1
            while (!feof(arq1)) {
                char removido;
                int nroInscricao = 0;
                double nota = -1;
                char data[11] = "\0";
                //data[10] = '\0';

                char cidade[100] = "\0"; // = NULL;
                char nomeEscola[100] = "\0"; // = NULL;

                int tamanhoCidade = 0;
                int tamanhoEscola = 0;

                if (lerLinha(arq1, RRN1, &removido, &nroInscricao, &nota, data, &tamanhoCidade, cidade, &tamanhoEscola, nomeEscola)) {
                    if (removido == NAO_REMOVIDO) {
                        escreverDadosEmArquivo(arqSaida, nroInscricao, nota, data, tamanhoCidade, cidade, tamanhoEscola, nomeEscola);
                    }
                }

                RRN1++;
            }

            //se ainda sobrou dados no arquivo 2
            while (!feof(arq2)) {
                char removido;
                int nroInscricao = 0;
                double nota = -1;
                char data[11] = "\0";
                //data[10] = '\0';

                char cidade[100] = "\0"; // = NULL;
                char nomeEscola[100] = "\0"; // = NULL;

                int tamanhoCidade = 0;
                int tamanhoEscola = 0;

                if (lerLinha(arq2, RRN2, &removido, &nroInscricao, &nota, data, &tamanhoCidade, cidade, &tamanhoEscola, nomeEscola)) {
                    if (removido == NAO_REMOVIDO) {
                        escreverDadosEmArquivo(arqSaida, nroInscricao, nota, data, tamanhoCidade, cidade, tamanhoEscola, nomeEscola);
                    }
                }

                RRN2++;
            }

            fclose(arq1);
            fclose(arq2);
            fecharArquivoBinarioEscrita(arqSaida);

            binarioNaTela(nomeArquivoSaida);
        } else {
            printf(MSG_ERRO);
        }

    } else {
        printf(MSG_ERRO);
    }

}

/**
 * Realize a operação cosequencial de matching (interesecção) de dois arquivos de dados,
 * considerando os valores do campo nroInscricao
 * 
 * Entrada Modelo:
  
10 arquivoEntrada1.bin arquivoEntrada2.bin arquivoSaida.bin
 
 * @param comando
 */
void opc10(char * comando) {
    char * nomeArq1 = strsep(&comando, " ");

    FILE * arq1 = abrirArquivoBinarioLeitura(nomeArq1);

    if (arq1) {

        char * nomeArq2 = strsep(&comando, " ");

        FILE * arq2 = abrirArquivoBinarioLeitura(nomeArq2);

        if (arq2) {

            char * nomeArquivoSaida = strsep(&comando, " ");

            FILE * arqSaida = fopen(nomeArquivoSaida, "wb");
            //escreve o cabeçalho
            escreverCabecalho(arqSaida);

            int RRN1 = 0, RRN2 = 0;


            //enquanto não chegar no fim dos dois arquivos
            while (!feof(arq1) && !feof(arq2)) {
                char removido1, removido2;
                //int encadeamento;
                int nroInscricao1 = 0, nroInscricao2 = 0;
                double nota1 = -1, nota2 = -1;
                char data1[11] = "\0", data2[11];
                //data[10] = '\0';

                char cidade1[100] = "\0", cidade2[100] = "\0"; // = NULL;
                char nomeEscola1[100] = "\0", nomeEscola2[100] = "\0"; // = NULL;

                int tamanhoCidade1 = 0, tamanhoCidade2 = 0;
                int tamanhoEscola1 = 0, tamanhoEscola2 = 0;

                if (lerLinha(arq1, RRN1, &removido1, &nroInscricao1, &nota1, data1, &tamanhoCidade1, cidade1, &tamanhoEscola1, nomeEscola1)) {




                    if (lerLinha(arq2, RRN2, &removido2, &nroInscricao2, &nota2, data2, &tamanhoCidade2, cidade2, &tamanhoEscola2, nomeEscola2)) {




                        if (removido1 == REMOVIDO) {
                            RRN1++;
                        }
                        if (removido2 == REMOVIDO) {
                            RRN2++;
                        }

                        if (removido1 == NAO_REMOVIDO && removido2 == NAO_REMOVIDO) {
                            if (nroInscricao1 < nroInscricao2) {
                                RRN1++;
                            } else if (nroInscricao1 == nroInscricao2) {

                                escreverDadosEmArquivo(arqSaida, nroInscricao1, nota1, data1, tamanhoCidade1, cidade1, tamanhoEscola1, nomeEscola1);

                                RRN1++;
                                RRN2++;

                            } else {
                                RRN2++;
                            }
                        }
                    }
                }
            }

            fclose(arq1);
            fclose(arq2);
            fecharArquivoBinarioEscrita(arqSaida);

            binarioNaTela(nomeArquivoSaida);
        } else {
            printf(MSG_ERRO);
        }

    } else {
        printf(MSG_ERRO);
    }
}

/**
 * Cria o arquivo de indice através de um arquivo de dados existente
 * Entrada Modelo:
  
11 arquivoEntrada.bin arquivoIndice.bin
 
 * @param comando
 */
void opc11(char * comando) {
    char * nomeArqEntrada = strsep(&comando, " ");
    char * nomeArquivoIndice = strsep(&comando, " ");



    LISTAINDICE * lista = listaIndCriar();

    
    //le o arquivo de dados e insere na lista ordenado
    
    

    if (escreverIndicie(nomeArquivoIndice, lista)) {

        listaIndApagar(lista);

        binarioNaTela(nomeArquivoIndice);
    } else {
        printf(MSG_ERRO);
    }
}

/*
 * Função Principal
 */
int main() {
    /*LISTAINDICE * lista = listaIndCriar();

    listaIndInserirOrdenado(lista, "Maria", 10);
    listaIndInserirOrdenado(lista, "Beatriz", 15);
    escreverIndicie("teste.bin", lista);

    exit(0);*/
    /*
        LISTAINDICE * lista = listaIndCriar();

        listaIndInserirOrdenado(lista, "Maria", 10);
        listaIndInserirOrdenado(lista, "Beatriz", 15);
        listaIndInserirOrdenado(lista, "Maria", 5);
        listaIndInserirOrdenado(lista, "Beatriz", 8);
        listaIndInserirOrdenado(lista, "Maria", 1);

        listaIndInserirFim(lista, "Ana", 10);
        listaIndInserirFim(lista, "Amanda", 10);

        listaIndInserirOrdenado(lista, "Amanda", 3);
        listaIndInserirFim(lista, "Barbara", 30);

        listaIndImprimir(lista);

        printf("\n");

        listaIndRemover(lista, 3);
        listaIndRemover(lista, 30);

        listaIndImprimir(lista);

        exit(0);
     */
    /*
        LISTA *lista = listaCriar();

        listarInserirInicio(lista, 1, -1, "a", 1, "a", 1, "a");
        listarInserirInicio(lista, 2, -1, "b", 2, "b", 2, "b");
        listarInserirInicio(lista, 3, -1, "", 0, "", 0, "");
        listarInserirInicio(lista, 4, -1, "", 0, "", 0, "");
        listarInserirInicio(lista, 5, -1, "", 0, "", 0, "");
        listarInserirInicio(lista, 6, -1, "", 0, "", 0, "");


        listarImprimir(lista);

        qsOrdernarLista(&lista->inicio);

        listarImprimir(lista);

        listarApagar(lista);

        return 0;
     */
    /*char a='@';
    printf("%02X ", a);
    exit(0);*/
    //comando a ser lido
    char * comando = calloc(100, sizeof (char));
    //strcpy(comando,"3 arquivoTrab1si.bin nomeEscola FRANCISCO RIBEIRO CARRI\0");

    //varicavel que guarda a opcao selecioanda
    int opc = 0;

    //leitura do comando a ser executado
    fgets(comando, 100, stdin);

    //pega o tamnho do comando lido
    size_t ln = strlen(comando) - 1;

    //elimina o \n caso houver para ajudar e evitar erros no processamento
    if (comando[ln] == '\n') {
        comando[ln] = '\0';

        if (comando[ln - 1] == '\r') {
            comando[ln - 1] = '\0';
        }
    }


    //verifica o inteiro digitado no comando
    opc = atoi(strsep(&comando, " "));


    switch (opc) {
        case 1:
        {
            opc1(comando);
            break;
        }
        case 2:
        {
            opc2(comando);
            break;
        }
        case 3:
        {
            opc3(comando);
            break;
        }
        case 4:
        {
            opc4(comando);
            break;
        }
        case 5:
        {
            opc5(comando);
            break;
        }
        case 6:
        {
            opc6(comando);
            break;
        }
        case 7:
        {
            opc7(comando);
            break;
        }
        case 8:
        {
            opc8(comando);
            break;
        }
        case 9:
        {
            opc9(comando);
            break;
        }
        case 10:
        {
            opc10(comando);
            break;
        }
        case 11:
        {
            opc11(comando);
        }
        case 99:
        {
            char * nomeArquivo = strsep(&comando, " ");
            binarioNaTela(nomeArquivo);

            break;
        }
        default:
        {
            printf("Opcao Invalida");
            break;
        }
    }

    //libera memoria do comando
    //free(comando);
    comando = NULL;

    return (0);
}
