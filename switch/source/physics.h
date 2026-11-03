
#ifndef PHYSICS_H
#define PHYSICS_H

// Valores originais da Alpha v0.0.4_7
#define PLAYER_SIZE 16
#define PLAYER_SPEED 175.0f
#define PLAYER_SPEED_KEY 230.0f
#define JUMP_VELOCITY -404.0f
#define GRAVITY 850.0f

#define COYOTE_TIME 0.10f
#define JUMP_BUFFER 0.10f

typedef struct {
    float x;
    float y;
    float vx;
    float vy;

    float coyoteTimer;
    float jumpBufferTimer;

    int onGround;
    int hasKey;
} Player;

void playerMove(Player* player, int direction);
void playerApplyGravity(Player* player, float dt);
void playerJump(Player* player);
void playerUpdateCoyoteTime(Player* player, float dt);
void playerUpdateJumpBuffer(Player* player, float dt);
void playerRequestJump(Player* player);
#endif