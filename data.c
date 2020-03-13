#include "data.h"

//Funções que retornam valores já definidos
//Retorna qual o numero do jogador atual(1 ou 2)
int obter_jogador_atual(ESTADO *estado) {
    return estado->jogador_atual;
}

//Retorna o numero de jogadas efetuadas(Cada jogada tem o movimento dos dois jogadores)
int obter_numero_de_jogadas(ESTADO *estado) {
    return estado->num_jogadas;
}

//Retorna o estado atual da casa na coordenada c (Branca, Preta ou Vazia)
CASA obter_estado_casa(ESTADO *e, COORDENADA c) {
    return e->tab[c.linha][c.coluna];
}

//Retorna a ultima jogada ´
COORDENADA obter_ultima_jogada (ESTADO *e) {
    return e->ultima_jogada;
}

////////////////////////////////////////////////////////////////////////
// Funções que retornam novos valores 
// Recebe uma coluna e uma linha e retorna uma coordenada
COORDENADA setCoordenada(int line, int col) {
    COORDENADA c;
    c.coluna = col;
    c.linha = line;
    return c;
}

//Recebe as coordenadas do jogador1 e do jogador2 respetivamente
//Retorna uma jogada 
JOGADA setJogada(COORDENADA c1, COORDENADA c2) {
    JOGADA jog; 
    jog.jogador1.coluna = c1.coluna;
    jog.jogador1.linha = c1.linha;
    jog.jogador2.coluna = c2.coluna;
    jog.jogador2.linha = c2.linha;
    return jog;
}

//Recebe o estado atual e uma coordenada
//Altera a ultima_jogada do estado para a nova coordenada
void setUltimaJogada(ESTADO *e, COORDENADA c1) {
    e -> ultima_jogada = setCoordenada(c1.linha, c1.coluna);
}

//Recebe o estado atual
//Troca o jogador atual
    //Se o jogador atual for o 1 troca para o jogador 2
    //Caso contrario troca para o jogador 1
void swapJogador(ESTADO *e) {
    if(e->jogador_atual == 1) {
        e->jogador_atual = 2;
    }
    else e->jogador_atual = 1;
}

//Recebe:
    //O estado atual
    //Uma jogada
//Adiciona a jogada a lista de jogadas e incrementa o numero de jogadas
void addToJogadas(ESTADO *e, JOGADA j) {
    if(e->num_jogadas < 32) e->jogadas[e->num_jogadas++] = j;
}

//Recebe:  
    //O estado atual
    //1 coordenada do tabuleiro
    //1 casa que pode ser: {VAZIO, BRANCA, PRETA}
//Altera o tabuleiro na coordenada 'c' com o valor de 'casa'
void setPosicao(ESTADO *e, COORDENADA c, CASA casa) {
    e->tab[c.linha][c.coluna] = casa;
}

//Inicializa o estado do jogo
//A peça branca começa na coluna 4 da linha 4 e considera se essa a ultima jogada
//Todo o resto do tabuleiro é inicializado a VAZIO
//O tabuleiro é representado por um array bidimensional do tipo tab[linha][coluna]
ESTADO *inicializar_estado() {
    ESTADO *e = (ESTADO *) malloc(sizeof(ESTADO));
    COORDENADA pecaBranca = setCoordenada(4, 4);
    COORDENADA c;
    setUltimaJogada(e, pecaBranca);
    FORI(8) 
        FORJ(8) {
            c = setCoordenada(i, j);
            setPosicao(e, c, VAZIO);
        }
    setPosicao(e, pecaBranca, BRANCA);
    e -> jogador_atual = 1;
    e -> num_jogadas = 0;
    return e;
}