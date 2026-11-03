
#include "game.h"

// Inicializa os dados principais do jogo
void gameInit(Game* game)
{
    game->state = GAME_MENU;
    game->currentLevel = 0;
    game->remainingTime = 7.0f;
}
