#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "mapa.h"

void lemapa(MAPA* m) {
    //abre o arquivo do mapa
    FILE* f;
    f = fopen("mapa.txt", "r");
    if(f == 0) {
        printf("Erro na leitura do mapa\n");
        exit(1);
    }

    //escaneia o numero de linhas e colunas
    fscanf(f, "%d %d", &(m->linhas), &(m->colunas));

    //aloca a memoria dinamicamente
    alocamapa(m);
    
    //armazena linha por linha na variavel mapa
    for(int i = 0; i < m->linhas; i++) {
        fscanf(f, "%s", m->matriz[i]);
    }
    
    fclose(f);
}

//aloca a memoria de acordo com o tamanho do mapa
void alocamapa(MAPA* m) {
    m->matriz = malloc(sizeof(char*) * m->linhas);
    for(int i = 0; i < m->linhas; i++) {
        m->matriz[i] = malloc(sizeof(char) * (m->colunas + 1));
    }
}

 //cria uma copia do mapa
void copiamapa(MAPA* destino, MAPA* origem) {
    destino->linhas = origem->linhas;
    destino->colunas = origem->colunas;

    alocamapa(destino);
    for(int i = 0; i < origem->linhas; i++) {
        //copia o conteudo linha por linha
        strcpy(destino->matriz[i], origem->matriz[i]);
        }
}

//acha a posicao do objeto no mapa
int encontranomapa(MAPA* m, POSICAO* p, char c) {
    for(int i = 0; i < m->linhas; i++) {
        for(int j = 0; j < m->colunas; j++) {
            if(m->matriz[i][j] == c)  {
                p->x = i;
                p->y = j;
                return 1;
            }
        }
    }
    return 0;
}

//verifica se a nova posicao corresponde a algum personagem
int tipodepersonagem(MAPA* m, char personagem,
                    int x, int y) {
    return m->matriz[x][y] == personagem;
}

//verifica se a posicao e valida 
int posicaovalida(MAPA* m, char personagem, int x, int y) {
    //verifica se a posição está dentro dos limites do mapa
    if (x < 0 || x >= m->linhas || y < 0 || y >= m->colunas) {
        return 0;
    }

    if (personagem == FANTASMA) {
        //fantasma pode se mover para posições vazias ou onde está o herói
        return (m->matriz[x][y] == VAZIO || tipodepersonagem(m, HEROI, x, y));
    } else if (personagem == HEROI) {
        //herói pode se mover para posicoes vazias ou para piilulas
        return (m->matriz[x][y] == VAZIO || tipodepersonagem(m, PILULA, x, y));
    }
    //caso não seja nenhum dos personagens reconhecidos
    return 0;
}

//move o personagem
void andanomapa(MAPA* m, int xorigem, int yorigem,
                int xdestino, int ydestino) {
    //armazena o tipo de personagem
    char personagem = m->matriz[xorigem][yorigem];
    //muda o destino para o personagem
    m->matriz[xdestino][ydestino] = personagem;
    //muda a origem para vazio
    m->matriz[xorigem][yorigem] = VAZIO;
}

//libera a memoria que foi alocada pelo mapa
void liberamapa(MAPA* m) {
    for(int i = 0; i < m->linhas; i++) {
        free(m->matriz[i]);
    } 
    free(m->matriz);
}
