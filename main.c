#include <stdio.h>
#include "invaders.h"

int main(void){
    srand(time(NULL));
    
    display_welcome();
    init_world();
    
    char keyPress;
    int i = 0;
    enemy.current_enemies = enemy.total_enemies;
    while (!player.defeat){
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
    
    finalize_game();

    return 0;
}