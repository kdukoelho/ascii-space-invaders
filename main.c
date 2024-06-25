#include <stdio.h>
#include <ncurses.h>
#include "invaders.h"

int main(void){
    srand(time(NULL));
    
    display_welcome();
    init_world();
    
    char keyPress;
    int i = 0;
    enemy.current_enemies = enemy.total_enemies;
    while (!defeat){
        enemy.speed = 1 + 10 * enemy.current_enemies / enemy.total_enemies;
        enemy.drop = 0;
        player.laserStatus++;

        display_world();
        update_board(i);
        enemies_laser(i);
        
        keyPress = getch();
        move_player(keyPress);
        player_laser(keyPress);
        
        if (player.score >= 3375){
            break;
        }

        i++;
        usleep(50000);
    }

clear(); refresh();

int maxY, maxX;
getmaxyx(stdscr, maxY, maxX);

    if (!defeat) {
        mvprintw((maxY / 2 + 1) - 4, (maxX / 2) - (24/2), "Bom trabalho, guerreiro!");
        refresh(); sleep(1);
        mvprintw((maxY / 2 + 2) - 4, (maxX / 2) - (40/2), "A orda de PROMPT INVADERS foi derrotada!");
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
    return 0;
}