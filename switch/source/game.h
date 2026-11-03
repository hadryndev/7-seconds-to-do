
#ifndef GAME_H
#define GAME_H

typedef enum {
    GAME_MENU,
    GAME_PRESENTATION,
    GAME_CHALLENGE,
    GAME_VICTORY,
    GAME_DEFEAT
} GameState;

typedef struct {
    GameState state;
    int currentLevel;
    float remainingTime;
} Game;

void gameInit(Game* game);

#endif
