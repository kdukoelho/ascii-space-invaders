#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>

#define SIZEY 22
#define SIZEX 40

char world[SIZEY][SIZEX];
char player = 'A', playerLaser = '^';
char enemy = 'Y', enemyShielded = 'O', enemyLaser = '!', enemyDirection;
char explosion = 'X';

int score = 0, defeat = 0;
int laserReady = 1;
int totalEnemies = 0, drop = 0;

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
                totalEnemies++;
            } else if ((y + 1) % 2 == 0 && y >= 7 && y < 9 && x > 4 && x < SIZEX - 5 && x % 2 == 0){
                world[y][x] = enemyShielded;
                totalEnemies = totalEnemies + 2;
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

void move_player(char keyPress){
    for (int x = 0; x < SIZEX; x++){
        if (world[SIZEY - 1][x + 1] == player && keyPress == 'a'){
            world[SIZEY - 1][x] = player;
            world[SIZEY - 1][x + 1] = ' ';
            break;
        }
        if (world[SIZEY - 1][x - 1] == player && keyPress == 'd') {
            world[SIZEY - 1][x] = player;
            world[SIZEY - 1][x - 1] = ' ';
            break;
        }
    }
}

void check_enemy_direction(){
    // Percore o eixo y, verificando se o inimigo chegou nos limite do mundo.
    for (int y = 0; y < SIZEY; y++){
        if (world[y][0] == enemy){
            enemyDirection = 'r';
            drop = 1;
            break;
        }
        if (world[y][SIZEX - 1] == enemy){
            enemyDirection = 'l';
            drop = 1;
            break;
        }
    }
}

void update_enemies(int i, int enemySpeed){
    if (i % enemySpeed == 0){

        // Verifica a direção do inimigo.
        if (enemyDirection == 'l'){

            // Percore a matriz do mundo do jogo da direita para a esquerda.
            for (int x = 0; x < SIZEX - 1; x++){
                for (int y = 0; y < SIZEY; y++){
                    if (drop && world[y - 1][x + 1] == enemy || world[y - 1][x + 1] == enemyShielded){
                        world[y][x] = world[y - 1][x + 1]; // Move o inimigo para baixo.
                        world[y - 1][x + 1] = ' '; // Limpa a posição anterior.
                    } 
                    else if (!drop && (world[y][x + 1] == enemy || world[y][x + 1] == enemyShielded)){
                        world[y][x] = world[y][x + 1]; // Move o inimigo para baixo.
                        world[y][x + 1] = ' '; // Limpa a posição anterior.
                    }
                }
            }
        }
        else{

            // Percore a matriz do mundo do jogo da esquerda para a direita.
            for (int x = SIZEX - 1; x > 0; x--){
                for (int y = 0; y < SIZEY; y++){
            
                    // Verifica se deve deixar o inimigo cair.
                    if (drop && world[y - 1][x - 1] == enemy || world[y - 1][x - 1] == enemyShielded){ 
                        world[y][x] = world[y - 1][x - 1]; // Move o inimigo para baixo.
                        world[y - 1][x - 1] = ' '; // Limpa a posição anterior.
                    } 
                    else if (!drop && (world[y][x - 1] == enemy || world[y][x - 1] == enemyShielded)){
                        world[y][x] = world[y][x - 1]; // Move o inimigo para a direita.
                        world[y][x - 1] = ' '; // Limpa a posição anterior.
                    }
                }
            }
        }

        // Verifica se algum inimigo chegou ao final do mundo.
        for (int x = 0; x < SIZEX; x++){
            if (world[SIZEY - 1][x] == enemy || world[SIZEY - 1][x] == enemyShielded){
                defeat = 1; // Define a derrota como verdadeira.
                break;
            }
        }
    }
}

int main(void){
    srand(time(NULL));
    
    display_welcome();
    init_world();
    
    char keyPress;
    int i = 0;
    int currentEnemies = totalEnemies;
    while (!defeat){
        int enemySpeed = 1 + 10 * currentEnemies / totalEnemies;
        drop = 0;
        display_world();
        check_enemy_direction();
        update_enemies(i, enemySpeed);
        keyPress = getch();
        move_player(keyPress);
        i++;
        usleep(50000);
    }
    
    endwin();   
    return 0;
}