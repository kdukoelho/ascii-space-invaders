#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>

#define SIZEY 22
#define SIZEX 40

typedef struct{
    char player, laser;
    int laserStatus, 
    score, defeat;
} Player;

Player player = {'A', '^', 1, 0, 0};

typedef struct{
    int total_enemies, current_enemies, drop, speed, is_ready, has_enemies;
    char enemy, shielded, laser, 
    direction;

} Enemy;

Enemy enemy = {0, 0, 0, 0, 0, 1, 'Y', 'O', '!', 'l'};

char world[SIZEY][SIZEX];
char explosion = 'X';

void display_welcome(){
    // Limpa o terminal
    printf("\033[2J\033[H");

    // Inicializa a tela do ncurses
    initscr();
    
    // Obtem as dimensões da tela
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    // Exibe as mensagens com um pequeno delay entre elas
    mvprintw((maxY / 2) - 4 , (maxX / 2) - (21/2), "Bem vindo, guerreiro!");
    refresh(); sleep(1);
    mvprintw((maxY / 2 + 1) - 4, (maxX / 2) - (39/2), "Seu sistema operacional esta em perigo.");
    refresh(); sleep(2);
    mvprintw((maxY / 2 + 2) - 4, (maxX / 2) - (75/2), "Hackers invadiram o seu computador e enviaram uma horda de PROMPT INVADERS!");
    refresh(); sleep(2);
    mvprintw((maxY / 2 + 3) - 4, (maxX / 2) - (49/2), "Derrote-os e evite o desligamento de sua maquina!");
    refresh(); sleep(1);
    mvprintw((maxY / 2 + 6) - 4, (maxX / 2) - (37/2), "Aperte qualquer tecla para continuar.");
    refresh();

    // Aguarda a entrada do usuario
    getch();

    // Limpa a tela
    clear();
}

void init_world(){
    // Atribui as posições de cada elemento no mundo.
    for (int x = 0; x < SIZEX; x++){
        for (int y = 0; y < SIZEY; y++){
            if ((y + 1) % 2 == 0 && y < 7 && x > 4 && x < SIZEX - 5 && x % 2 == 0){
                world[y][x] = enemy.enemy;
                enemy.total_enemies++;
            } else if ((y + 1) % 2 == 0 && y >= 7 && y < 9 && x > 4 && x < SIZEX - 5 && x % 2 == 0){
                world[y][x] = enemy.shielded;
                enemy.total_enemies += 2;
            } else {
                world[y][x] = ' ';
            }
        }
    }
    world[SIZEY - 1][SIZEX / 2] = player.player;
}

void display_world(){
    clear();

    // Obtem as dimensões da tela
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    
    // Exibe o contador de SCORE.
    mvprintw(0, (maxX - 8) / 2, "SCORE: %d\n", player.score);
    // Define as bordas do mapa.
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
    // Movimenta o player de acordo a entrada do jogador.
    for (int x = 0; x < SIZEX; x++){
        if (world[SIZEY - 1][x + 1] == player.player && keyPress == 'a'){
            world[SIZEY - 1][x] = player.player;
            world[SIZEY - 1][x + 1] = ' ';
            break;
        }
        if (world[SIZEY - 1][x - 1] == player.player && keyPress == 'd') {
            world[SIZEY - 1][x] = player.player;
            world[SIZEY - 1][x - 1] = ' ';
            break;
        }
    }
}

void player_laser(char keyPress){
    // Verifica se a tecla 'm' foi pressionada e se o laser está pronto.
    if (keyPress == 'm' && player.laserStatus > 2){
        for (int x = 0; x < SIZEX; x++){ 
            if (world[SIZEY-1][x] == player.player) {
                world[SIZEY-2][x] = player.laser; // Coloca o caracter correspondente ao laser imediatamente acima do player.
                player.laserStatus = 0; // Define o status do laser como "não-pronto".
            }
        }
    }

    // Movimenta o laser do jogador pelo mundo.
    for (int x = 0; x < SIZEX; x++){
        if (world[1][x] == player.laser){
            world[1][x] = ' ';
        }
        for (int y = 1; y < SIZEY; y++){
            // Move o laser do jogador caso não haja colisão iminente com o laser inimigo.
            if (world[y][x] == player.laser && world[y-1][x] != enemy.laser){
                world[y][x] = ' ';
                world[y - 1][x] = player.laser;
            }
        }
    }

}

void check_enemy_direction(){
    // Percore o eixo y, verificando se o inimigo chegou nos limite do mundo.
    for (int y = 0; y < SIZEY; y++){
        if (world[y][0] == enemy.enemy){
            enemy.direction = 'r';
            enemy.drop = 1;
            break;
        }
        if (world[y][SIZEX - 1] == enemy.enemy){
            enemy.direction = 'l';
            enemy.drop = 1;
            break;
        }
    }
}

void update_enemies(int i){
    if (i % enemy.speed == 0){

        // Verifica a direção do inimigo.
        if (enemy.direction == 'l'){

            // Percore a matriz do mundo do jogo da direita para a esquerda.
            for (int x = 0; x < SIZEX - 1; x++){
                for (int y = 0; y < SIZEY; y++){
                    if (enemy.drop && world[y - 1][x + 1] == enemy.enemy || world[y - 1][x + 1] == enemy.shielded){
                        world[y][x] = world[y - 1][x + 1]; // Move o inimigo para baixo.
                        world[y - 1][x + 1] = ' '; // Limpa a posição anterior.
                    } 
                    else if (!enemy.drop && (world[y][x + 1] == enemy.enemy || world[y][x + 1] == enemy.shielded)){
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
                    if (enemy.drop && world[y - 1][x - 1] == enemy.enemy || world[y - 1][x - 1] == enemy.shielded){ 
                        world[y][x] = world[y - 1][x - 1]; // Move o inimigo para baixo.
                        world[y - 1][x - 1] = ' '; // Limpa a posição anterior.
                    } 
                    else if (!enemy.drop && (world[y][x - 1] == enemy.enemy || world[y][x - 1] == enemy.shielded)){
                        world[y][x] = world[y][x - 1]; // Move o inimigo para a direita.
                        world[y][x - 1] = ' '; // Limpa a posição anterior.
                    }
                }
            }
        }

        // Verifica se algum inimigo chegou ao final do mundo.
        for (int x = 0; x < SIZEX; x++){
            if (world[SIZEY - 1][x] == enemy.enemy || world[SIZEY - 1][x] == enemy.shielded){
                player.defeat = 1; // Define a derrota como verdadeira.
                break;
            }
        }
    }
}

int enemies_laser(int i){
    for (int x = 0; x < SIZEX; x++){
        for (int y = 0; y < SIZEY; y++){

            // Verifica se as condições de disparo do laser de inimigos foram atingidas.
            if ((i % 5 == 0) && (world[y][x] == enemy.shielded | world[y][x] == enemy.enemy) 
            && (rand() % 15) > 13 && world[y+1][x] != player.laser){
                // Verifica se há inimigos abaixo da posição atual para decidir se pode disparar.
                for (int yi = y + 1; yi < SIZEY; yi++){
                    if (world[yi][x] == enemy.enemy || world[yi][x] == enemy.shielded){
                        enemy.is_ready = 0;
                        break;
                    }
                    enemy.is_ready = 1;
                }

                // Se nenhum inimigo estiver abaixo as condições de disparo são atendidas.
                if (enemy.is_ready){
                    world[y + 1][x] = enemy.laser;
                }
            }
        }
        for (int y = SIZEY - 2; y >= 0; y--){
            if (i % 2 == 0){
                if(world[y][x] == enemy.laser && (world[y + 1][x] != enemy.enemy && world[y + 1][x] != enemy.shielded)){
                world[y + 1][x] = enemy.laser;
                world[y][x] = ' ';
                }
                else if (world[y][x] == enemy.laser){
                    world[y][x] = ' ';
                }
            }
        }
    }
}

int are_enemies_alive(){
    for (int y = 0; y < SIZEY; y++){
        for (int x = 0; x < SIZEX; x++){
            if(world[y][x] == enemy.enemy || world[y][x] == enemy.shielded){
                return 1;
            }
        }
    }
    return 0;
}

void update_board(int i){
    check_enemy_direction();
    update_enemies(i);
    
    // Verifica se há inimigos vivos.
    enemy.has_enemies = are_enemies_alive();

    // Verifica a morte de um jogador.
    int has_player = 0;
    for (int x = 0; x < SIZEX; x++){
        if (world[SIZEY - 1][x] == player.player){
                has_player = 1;
                break;
            } else{
                has_player = 0;
            }
    }

    if (!has_player){
        player.defeat = 1;
    }


    for (int x = 0; x < SIZEX; x++){
        for (int y = 0; y < SIZEY; y++){
            
            // Verifica se o laser do jogador atingiu um inimigo.
            if (world[y][x] == player.laser && world[y - 1][x] == enemy.enemy){
                world[y][x] = ' '; // Remove o laser da antiga posição.
                world[y - 1][x] = explosion; // Inimigo morre. 
                enemy.current_enemies--;           
                if (x > SIZEX/1){
                    player.score += 50;
                } else if (x > SIZEX/2){
                    player.score += 30;
                } else if (x > SIZEX/3){
                    player.score += 60;
                } else if (x > SIZEX/4){
                    player.score += 130;
                }
            }

            // Verifica se o laser do jogador atingiu um inimigo com escudo.
            else if (world[y][x] == player.laser && world[y - 1][x] == enemy.shielded){
                world[y][x] = ' '; 
                world[y - 1][x] = enemy.enemy; // Inimigo com escudo vira um inimigo simples.
                enemy.current_enemies--;
                player.score += 50;
            }
            
            // Verifica se o laser do inimigo atingiu o laser jogador.
            else if (world[y][x] == player.laser && world[y - 1][x] == enemy.laser){
                world[y][x] = ' '; // Remove o laser do jogador.
            }

            // Remove as explosões.
            else if (world[y][x] == explosion){
                world[y][x] = ' ';
            }

            else if (world[SIZEY - 1][x] == enemy.laser && world[SIZEY - 1][x] != player.player){
                world[SIZEY - 1][x] = ' ';
            }

            // Verifica se o laser inimigo antigiu o jogador.
            else if ((i + 1) % 2 == 0 && world[y][x] == enemy.laser && world[y + 1][x] == player.player){
                world[y + 1][x] = explosion;
                world[y][x] = ' '; 
                player.defeat = 1;
                break;
            }

            if (enemy.current_enemies <= 0){
                break;
            }
        }
    }
}

void finalize_game(){
    clear(); refresh();

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    
    // Exibe mensagem final;
    if (!player.defeat) {
        mvprintw((maxY / 2 + 1) - 4, (maxX / 2) - (24/2), "Bom trabalho, guerreiro!");
        refresh(); sleep(1);
        mvprintw((maxY / 2 + 2) - 4, (maxX / 2) - (40/2), "A horda de PROMPT INVADERS foi derrotada!");
        refresh(); sleep(2);
        mvprintw((maxY / 2 + 3) - 4, (maxX / 2) - (39/2), "Sua maquina agora, esta fora de perigo.");
        refresh(); sleep(5);
    } else{
        mvprintw((maxY / 2 + 1) - 4, (maxX / 2) - (10/2), "PATETICO!");
        refresh(); sleep(1);
        mvprintw((maxY / 2 + 2) - 4, (maxX / 2) - (72/2), "Os PROMPT INVADERS venceram, sua maquina sera desligada imediatamente!");
        refresh(); sleep(5);
    }
    
    endwin();
}