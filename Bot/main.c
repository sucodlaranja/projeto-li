#include <stdio.h>
#include "Camada de dados.h"
#include "Listas.h"
#include <stdlib.h>
#include <string.h>
#include "logica pograma.h"

int main(int argc, char **argv) {
    COORDENADA coord;
    ESTADO *e = inicializar_estado();
    int coluna = 0, linha = 7, count;
    char h;
    char segundo[BUF_SIZE] = "segundo";
    strcpy(segundo,argv[2]);
    if (argc == 3) {
        FILE *file = fopen(strcat(argv[1],".txt"),"r");

        //tabuleiro
        while (linha != -1) {
            h = fgetc(file);
            if (h == '\n') coluna = 0, linha--;
            else if (h == EOF) break;
            else if (h == '1') e->tab[coluna][linha] = UM, coluna++;
            else if (h == '*') e->tab[coluna][linha] = BRANCA, coluna++;
            else if (h == '#') e->tab[coluna][linha] = PRETA, coluna++;
            else if (h == '.') e->tab[coluna][linha] = VAZIO, coluna++;
            else e->tab[coluna][linha] = DOIS, coluna++;
        }
        //movs
        int n=0,j1l=0,j2l=0,contador=0,jogadas=0;
        char line[BUF_SIZE],j1c,j2c;
        while (fgets(line,BUF_SIZE,file) != NULL) {
            if (sscanf(line,"%d: %c%d %c%d",&jogadas,&j1c,&j1l,&j2c,&j2l) == 5) {
                e->jogadas[n].jogador1.coluna = j1c - 'a';
                e->jogadas[n].jogador1.linha = j1l - 1;
                e->jogadas[n].jogador2.linha = j2l - 1;
                e->jogadas[n].jogador2.coluna = j2c - 'a';
                n++;
            }
            else if (sscanf(line,"%d: %c%d ",&jogadas,&j1c,&j1l) == 3) {
                e->jogadas[n].jogador1.coluna = j1c- 'a';
                e->jogadas[n].jogador1.linha = j1l- 1;
                contador++;
            }

        }
        e->num_jogadas = n;
        if (contador == 0) {
            e->ultima_jogada.coluna = 4;
            e->ultima_jogada.linha = 4;
        }
        else if (contador%2 == 1) {
            e->ultima_jogada.coluna = j1c - 'a';
            e->ultima_jogada.linha = j1l - 1;

        } else if (contador%2 == 0) {
            e->ultima_jogada.coluna = j2c - 'a';
            e->ultima_jogada.linha = j2l - 1;
        }
        count = contador;
        fclose(file);
        //heuristia
        LISTA L;
                L = criadordeposicoes(e);
                while (L != NULL && L->prox != NULL) {
                    if (obter_jogador_atual(count) == 1) {
                        if ((L->valor->coluna + L->valor->linha) < (L->prox->valor->coluna + L->prox->valor->linha)) {
                            L->prox->valor->linha = L->valor->linha;
                            L->prox->valor->coluna = L->valor->coluna;
                            L = proximo(L);
                        } else L = proximo(L);
                    } else {
                        if (abs((L->valor->coluna - 7) + (L->valor->linha - 7)) <
                            abs((L->prox->valor->coluna - 7) + (L->prox->valor->linha - 7))) {
                            L->prox->valor->linha = L->valor->linha;
                            L->prox->valor->coluna = L->valor->coluna;
                            L = proximo(L);
                        } else L = proximo(L);
                    }
                }


                coord.coluna = L->valor->coluna;
                coord.linha = L->valor->linha;

                atualizador(e, coord);



        //se o bot for o segundo jogador incrementar o numero de jogadas
        array(e, count, coord.linha, coord.coluna);
        if (obter_jogador_atual(count) == 2) {
            num_jogadas(e);
        }


        //gravar em jog02
        char tabuleiro[8][8];
        int c, l;

        for (l = 0; l < 8; l++) {
            for (c = 0; c < 8; c++) {
                if (e->tab[c][l] == DOIS) tabuleiro[c][l] = '2';
                else if (e->tab[c][l] == UM) tabuleiro[c][l] = '1';
                else if (e->tab[c][l] == BRANCA) tabuleiro[c][l] = '*';
                else if (e->tab[c][l] == PRETA) tabuleiro[c][l] = '#';
                else tabuleiro[c][l] = '.';
            }
        }
        FILE *file2 = fopen(strcat(segundo,".txt"), "w");


        //tabuleiro
        for (l = 7; l > -1; l--) {

            for (c = 0; c < 8; c++) {
                fprintf(file2, "%c", tabuleiro[c][l]);
            }
            fprintf(file2, "\n");
        }

        //linha de separacao
        fprintf(file2, "\n");

        //jogadas
        for(int k=0;k<e->num_jogadas;k++) {
            fprintf(file2,"%d: %c%d %c%d\n",k+1,e->jogadas[k].jogador1.coluna + 'a',e->jogadas[k].jogador1.linha + 1,e->jogadas[k].jogador2.coluna +'a',e->jogadas[k].jogador2.linha + 1);
        }
        if(e->jogadas[e->num_jogadas].jogador1.linha !=-1 && e->jogadas[e->num_jogadas].jogador1.coluna !=-1) {
            fprintf(file2,"%d: %c%d\n",e->num_jogadas+1,e->jogadas[e->num_jogadas].jogador1.coluna + 'a',e->jogadas[e->num_jogadas].jogador1.linha + 1);
        }

        fclose(file2);

        return 0;
    }
}