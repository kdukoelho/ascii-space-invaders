#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#define SIZEY 23
#define SIZEX 40

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

int main(void){
    display_welcome();
    return 0;
}