#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include "fogefoge.h"
#include "mapa.h"
#include "ui.h"

MAPA m;
POSICAO heroi;
int tempilula = 0;

int main() {
    //inicializa a semente do gerador de numeros aleatorios (movimentos aleatorios fantasmas)
    srand(time(0));
    //le o arquivo do mapa e carrega seus dados na struct m
    lemapa(&m);
    //procura a posicao inicial do heroi e armazena na sctruct heroi
    encontranomapa(&m, &heroi, HEROI);

    do {
        printf("Tem pilula: %s\n", (tempilula ? "SIM" : "NAO"));

        //imprime o estado atual do mapa    
        imprimemapa(&m);

        //captura a tecla inserida 
        char comando;
            scanf(" %c", &comando);
        
        //move o heroi se o movimento for valido
        move(comando);

        //se o comando for b, explode a pilula
        if(comando == BOMBA && tempilula) explodepilula();

        fantasmas();
    } while (!acabou());

    imprimemapa(&m);
    liberamapa(&m);

    return 0;
}

//verifica se a entrada de direcao e valida 
int entradavalida(char direcao) {
    return direcao == 'a' ||
        direcao == 'w' ||
        direcao == 's' ||
        direcao == 'd';
}

//move o heroi
void move(char direcao) {
    //verifica se a tecla pressionada e valida
    if(entradavalida(direcao) == 0) 
    return;

    //calcula a nova posicao de acordo com a entrada
    int proximox = heroi.x;
    int proximoy = heroi.y;

    switch(direcao) {
        case CIMA:
            proximox--;
            break;
        case BAIXO:    
            proximox++;
            break;
        case ESQUERDA:
            proximoy--;
            break;
        case DIREITA:
            proximoy++;
            break;
    }

    //verifica se a nova posicao e valida
    if(!posicaovalida(&m, HEROI, proximox, proximoy))
    return;
    //verifica se a nova posicao e uma pilula
    if(tipodepersonagem(&m, PILULA, proximox, proximoy)) 
    tempilula = 1;

    //move o heroi para a nova posicao
    andanomapa(&m, heroi.x, heroi.y, proximox, proximoy);
    heroi.x = proximox;
    heroi.y = proximoy;
}  

//executa a explosao para as 4 direcoes
void explodepilula() {
    explodepilula2(heroi.x, heroi.y, 0, 1, 3);
    explodepilula2(heroi.x, heroi.y, 0, -1, 3);
    explodepilula2(heroi.x, heroi.y, 1, 0, 3);
    explodepilula2(heroi.x, heroi.y, -1, 0, 3);
    tempilula = 0;
}

//explode a pilula
void explodepilula2(int x, int y, int somax, int somay, int qtd) {
    //para se rodar o numero maximo de vezes
    if(qtd == 0) return;

    int novox = x + somax;
    int novoy = y + somay;

    //para se tiver parede
    if(m.matriz[novox][novoy] == PAREDE_VERTICAL || m.matriz[novox][novoy] == PAREDE_HORIZONTAL) return;

    //substitui o espaco por vazio
    m.matriz[novox][novoy] = VAZIO;
    //executa de novo (loop)
    explodepilula2(novox, novoy, somax, somay, qtd - 1);
}

//controla a movimentacao dos fantasmas
void fantasmas() {
    MAPA copia;

    //cria uma copia do mapa para o algoritmo de movimentacao
    copiamapa(&copia, &m);
    //varre a matriz
    for(int i = 0; i < m.linhas; i++) {
        for(int j = 0; j < m.colunas; j++) {
            //quando encontra um fantasma, move ele
            if(copia.matriz[i][j] == FANTASMA) {

                int xdestino;
                int ydestino;
                
                //gera uma posicao aleatoria que esteja vazia
                int encontrou = direcaofantasma(i, j,
                            &xdestino, &ydestino);
                
                //se a posicao for valida, move o fantasma
                if(encontrou) {
                    andanomapa(&m, i, j, xdestino, ydestino);
                }
            }
        }
    }
    //libera a memoria alocada pela copia do mapa
    liberamapa(&copia);
}

//randomiza a movimentacao do fantasma
int direcaofantasma(int xatual, int yatual,
            int* xdestino, int* ydestino) {
    //lista de movimentos possiveis
    int opcoes[4][2] = {
        { xatual, yatual + 1},
        { xatual + 1, yatual},
        { xatual, yatual -1},
        { xatual - 1, yatual}
    };

    //gera um numero aleatorio de 1 a 4 correspondente as opcoes
    for(int i = 0; i < 10; i++) {
        int posicao = rand() % 4;

    //verifiica se a opcao gerada é valida
    //se encontrar uma posicao valida, atualiza as posicoes
    if(posicaovalida(&m, FANTASMA, opcoes[posicao][0], opcoes[posicao][1])) {
        *xdestino = opcoes[posicao][0];
        *ydestino = opcoes[posicao][1];
        return 1;
    }
    }
    return 0;
}   

//se o heroi nao esta no mapa, o jogo acaba
int acabou() {
    POSICAO pos;
    int heroinomapa = encontranomapa(&m,&pos, HEROI);
    return !heroinomapa;
}