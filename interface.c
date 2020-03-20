#include "interface.h"
#define BUF_SIZE 1024
void mostra_casa(CASA c) {
    switch(c) {
    case BRANCA:
        putchar('*');
        break;
    case PRETA:
        putchar('#');
        break;
    default:
        putchar('.');
    }
}

void mostrar_tabuleiro(ESTADO *e) {
    COORDENADA c;
    
    REVERSE_FORI(8) { //Percorrer todas as linhas de forma inversa
        printf("%d ", i + 1);
        FORJ(8) { //Percorrer todas as colunas de uma linha
            if(i == 7 && j == 7) putchar('2');
            else if(i == 0 && j == 0) putchar('1');
            else {
                c = setCoordenada(i, j); 
                CASA c1 = getCasa(e, c);
                mostra_casa(c1);
            }
        }
        putchar('\n');
    }
    printf("  abcdefgh\n");
}

int interpretador(ESTADO *e) {
    char linha[BUF_SIZE];
    char col[2], lin[2];
    char filename[BUF_SIZE];
    FILE *fp;
    printf("Bem vindo ao Rastros!\nInstruções: Q - Sair; gr - Gravar,");
    mostrar_tabuleiro(e);
    for(int i = 0; endGame(e); i++) {
        printf("(#%d Jogada %d Player %d)> ", i, getNumJogadas(e), getjogador(e));
        if(fgets(linha, BUF_SIZE, stdin) == NULL) return 0; //le o comando e retorna 0 caso nao seja escrito nada
        if(strlen(linha) == 3 && sscanf(linha, "%[a-h]%[1-8]", col, lin) == 2) { //If que executa a jogada
            COORDENADA coord = {*col - 'a', *lin - '1'};
            if(jogar(e, coord)) mostrar_tabuleiro(e);
        } else if(strcmp(linha, "Q\n") == 0) { //Comando para sair do jogo
            printf("Saindo do jogo...\n");
            return 0;
        } else if(sscanf(linha, "gr %s", filename) == 1) { //Comando que guarda o tabuleiro atual (cria um "savefile")
            fp = fopen(filename, "w");
            fprintf(fp, "Placeholder be like 💩💩💩💩💩💩💩💩💩💩💩💩💩\n");
            fclose(fp);
        } else if(strcmp(linha, "ler\n") == 0) { //Comando que le um "savefile"
            printf("Faz de conta que le\n");
        }
    }
    return 1;
}