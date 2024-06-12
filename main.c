#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#define SIZEY 22
#define SIZEX 40

char world[SIZEY][SIZEX];
char player = 'A', playerLaser = '^';
char enemy = 'Y', enemyShielded = 'O', enemyLaser = '!', explosion = 'X';

int score = 0, defeat = 0, laserReady = 1;
void display_welcome(){
    // Limpa o terminal
    printf("\033[2J\033[H");

    // Inicializa a tela do ncurses
    initscr();
    
    // Obtem as dimensões da tela
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    // Exibe as mensagens com um pequeno delay entre elas
    mvprintw((maxY / 2) - 4 , (maxX / 2) - (20/2), "Bem vindo, guerreiro!");
    refresh(); sleep(1);
    mvprintw((maxY / 2 + 1) - 4, (maxX / 2) - (40/2), "Seu sistema operacional esta em perigo.");
    refresh(); sleep(2);
    mvprintw((maxY / 2 + 2) - 4, (maxX / 2) - (74/2), "Hackers invadiram o seu computador e enviaram uma orda de PROMPT INVADERS!");
    refresh(); sleep(2);
    mvprintw((maxY / 2 + 3) - 4, (maxX / 2) - (50/2), "Derrote-os e evite o desligamento de sua maquina!");
    refresh(); sleep(1);
    mvprintw((maxY / 2 + 6) - 4, (maxX / 2) - (38/2), "Aperte qualquer tecla para continuar.");
    refresh();

    // Aguarda a entrada do usuario
    getch();

    // Limpa a tela
    clear();
    endwin();
}

void init_world(){
    for (int x = 0; x < SIZEX; x++){
        for (int y = 0; y < SIZEY; y++){
            if ((y + 1) % 2 == 0 && y < 7 && x > 4 && x < SIZEX - 5 && x % 2 == 0){
                world[y][x] = enemy;
            } else if ((y + 1) % 2 == 0 && y >= 7 && y < 9 && x > 4 && x < SIZEX - 5 && x % 2 == 0){
                world[y][x] = enemyShielded;
            } else {
                world[y][x] = ' ';
            }
        }
    }
    world[SIZEY - 1][SIZEX / 2] = player;
}

void display_world(){
    clear();

    // Obtem as dimensões da tela
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    mvprintw(0, (maxX - 8) / 2, "SCORE: %d\n", score);
    for (int y = 0; y < SIZEY; y++) {
        mvprintw(y + 1, (maxX - SIZEX) / 2, "|");
        for (int x = 0; x < SIZEX; x++){
            printw("%c", world[y][x]);
        }
        printw("|\n");
    }
    refresh();
}

int main(void){
    display_welcome();
    init_world();
    display_world();

    getch();
    endwin();
    return 0;
}