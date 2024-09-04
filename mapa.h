#ifndef _MAPA_H_
#define _MAPA_H_

#define HEROI '@'
#define FANTASMA '#'
#define VAZIO '.'
#define PILULA '*'
#define PAREDE_VERTICAL '|'
#define PAREDE_HORIZONTAL '-'



struct mapa {
    char** matriz;
    int linhas;
    int colunas;
};
typedef struct mapa MAPA;

struct posicao{
    int x;
    int y;
};
typedef struct posicao POSICAO;

void lemapa();
void alocamapa();
void liberamapa();
int encontranomapa(MAPA* m, POSICAO* p, char c);
int posicaovalida(MAPA* m, char personagem, int x, int y);
void andanomapa(MAPA* m, int xorigem, int yorigem,
                int xdestino, int ydestino);
void copiamapa(MAPA* destino, MAPA* origem);
int tipodepersonagem(MAPA* m, char personagem,
                    int x, int y);

#endif