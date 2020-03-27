#include "controller.h"
#include <string.h>

/* Função deverá receber o estado atual e uma coordenada e  modificaro estado ao jogar na casa correta
se a jogada for válida. 
A função devolve verdadeiro (valor diferente de zero) se for possível jogar e falso (zero) caso
não seja possível.*/
//
// Função que verifica se uma jogada é válida, isto é, se a proxima jogada a ser realizada é vizinha da posição anterior
Boolean isCasaVizinha(ESTADO *e, COORDENADA c) {
    COORDENADA posAnt = getUltimaJogada(e);
    Boolean r = False;
    if (getLinha(posAnt) - 1 <= getLinha(c)  && getColuna(posAnt) - 1 <= getColuna(c) &&
        getLinha(posAnt) + 1 >= getLinha(c)  && getColuna(posAnt) + 1 >= getColuna(c))
        r = True;
    return r;
}

// Função que ve se não é possivel jogar mais e se a peça branca se encontra presa
Boolean isRodeado(ESTADO *e, COORDENADA c) {
    Boolean r = True;
    for(int i = getLinha(c) - 1; i <= getLinha(c) + 1; i++)
        for(int j = getColuna(c) - 1; j <= getColuna(c) + 1; j++) {
            if(i >= 0 && i < 8 && j >= 0 && j < 8) {
                COORDENADA c = setCoordenada(i, j);
                if(getCasa(e, c) == VAZIO){
                    r = False;
                    break;
                } 
            }
        }
    
    return r;
}

// Função que determina o fim do jogo
Boolean isTerminado(ESTADO *e) {
    Boolean r = False;
    COORDENADA posAnt = getUltimaJogada(e);
    if ((getLinha(posAnt) == 7 && getColuna(posAnt) == 7) ||
        (getLinha(posAnt) == 0 && getColuna(posAnt) == 0) ||
        (isRodeado(e, posAnt))) 
        r = True;
    return r;
}

int jogar(ESTADO *e, COORDENADA c) {
    COORDENADA lstJogada = getUltimaJogada (e);
    if ( (getCasa(e,c) == VAZIO || getCasa(e,c) == DOIS || getCasa(e,c) == UM)
         && c.coluna < 8 && c.coluna >= 0 && c.linha < 8 && c.linha >= 0
         && isCasaVizinha(e, c)) {
        setCasa(e, lstJogada, PRETA);
        if(getjogador(e) == 1) {
            addToJogadas(e, setJogada(c, createNullCoord()));
            setUltimaJogada (e, c);
            swapJogador(e);
        } else {
            JOGADA j = setJogada(lstJogada, c);
            editJogadas(e, j, getNumJogadas(e) - 1);
            setUltimaJogada (e, c); 
            swapJogador(e);
        }
        setCasa(e, c, BRANCA);
        return 1;
    } else {
        printf("Jogada Inválida.\n");
        return 0;
    }
}

char coordToChar(ESTADO *e, int linha, int coluna) {
    COORDENADA c = setCoordenada(linha, coluna); 
    CASA c1 = getCasa(e, c);
    return c1;
}

int winner(ESTADO *e) {
    int player = 0;
    COORDENADA posAnt = getUltimaJogada(e);
    if(getColuna(posAnt) == 0 && getLinha(posAnt) == 0) player = 1;
    if(getColuna(posAnt) == 7 && getLinha(posAnt) == 7) player = 2;
    if(isRodeado(e, posAnt)) {
        swapJogador(e);
        player = getjogador(e);
    }
    return player;
}

ERROS gravar(ESTADO *e, char *filename) {
    FILE *fp = fopen(filename, "w");
    JOGADA j;
    if(fp == NULL) return ERRO_ABRIR_FICHEIRO;
    //Ciclo que guarda o tabuleiro 
    REVERSE_FORI(8) {
        FORJ(8) fputc(getCasa(e, setCoordenada(i, j)), fp);
        fputc('\n', fp);
    }
    fputc('\n', fp);
    //Guarda as jogadas no ficheiro
    FORI(getNumJogadas(e)) {
        j = getJogada(e, i);
        int linJ1, linJ2;
        char colJ1, colJ2;
        linJ1 = getLinha(getCoordenada(j, 1)) + 1;
        colJ1 = getColuna(getCoordenada(j, 1)) + 'a';
        if (!isNullCoord(getCoordenada(j, 2))) {
            linJ2 = getLinha(getCoordenada(j, 2)) + 1;
            colJ2 = getColuna(getCoordenada(j, 2)) + 'a';
            fprintf(fp, "Jog%d: %c%d %c%d\n", getNumJogadas(e), colJ1, linJ1, colJ2, linJ2);
        }
        else fprintf(fp, "Jog%d: %c%d\n", getNumJogadas(e), colJ1, linJ1);
    }
    fclose(fp);
    return OK;
}

ERROS ler(ESTADO *e,  char *filename) {
    FILE *fp = fopen(filename, "r");
    char buffer[64];
    if(fp == NULL) return ERRO_ABRIR_FICHEIRO;
    e = inicializar_estado(); //Reinicializa o estado para assegurar que le o ficheiro sem problemas
    //Passar o tabuleiro no ficheiro para o estado
    REVERSE_FORI(8){
        fgets(buffer, 64, fp);
        FORJ(8) setCasa(e, setCoordenada(i, j), buffer[j]);
    }
    fgetc(fp); //Consumir o new line 
    //Passar as jogadas para o estado
    char *jog1, *jog2;
    int jogada;
    COORDENADA c1, c2;
    JOGADA j;
    while(fgets(buffer, 64, fp)) {
        //Partir a linha de jogadas em pedaços 
        int scanned = sscanf(buffer, "Jog%d: %s %s\n", &jogada, jog1, jog2);
        c1 = setCoordenada(jog1[1], jog1[0] - 'a');
        if(scanned == 3) c2 = setCoordenada(jog2[1], jog2[0] - 'a');
        else c2 = createNullCoord();
        j = setJogada(c1, c2);
        addToJogadas(e, j);    
    }

    //Alterar a ultima jogada para refletir o 'reloading' da lista de jogadas
    if (isNullCoord(c2)) setUltimaJogada(e, c1);
    else setUltimaJogada(e, c2);
    
    fclose(fp);
    return OK;
}





