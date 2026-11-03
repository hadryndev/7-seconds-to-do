
#include "physics.h"

// Define a velocidade horizontal do jogador
void playerMove(Player* player, int direction)
{
    float speed = player->hasKey
        ? PLAYER_SPEED_KEY
        : PLAYER_SPEED;

    player->vx = direction * speed;
}

// Aplica a gravidade enquanto o jogador estiver no ar
void playerApplyGravity(Player* player, float dt)
{
    if (!player->onGround)
    {
        player->vy += GRAVITY * dt;
    }
}

// Aplica o impulso inicial do salto
void playerJump(Player* player)
{
    if (player->onGround || player->coyoteTimer > 0.0f)
    {
        // Aplica o impulso vertical
        player->vy = JUMP_VELOCITY;

        // O jogador deixa de estar apoiado
        player->onGround = 0;

        // Consome a tolerancia para impedir salto duplo
        player->coyoteTimer = 0.0f;
    }
}

void playerUpdateCoyoteTime(Player* player, float dt)
{
    if (player->onGround)
    {
        // Renova a tolerancia enquanto estiver no chao
        player->coyoteTimer = COYOTE_TIME;
    }
    else if (player->coyoteTimer > 0.0f)
    {
        // Reduz a tolerancia enquanto estiver no ar
        player->coyoteTimer -= dt;

        if (player->coyoteTimer < 0.0f)
        {
            player->coyoteTimer = 0.0f;
        }
    }
}

void playerUpdateJumpBuffer(Player* player, float dt)
{
    if (player->jumpBufferTimer > 0.0f)
    {
        player->jumpBufferTimer -= dt;

        if (player->jumpBufferTimer < 0.0f)
        {
            player->jumpBufferTimer = 0.0f;
        }
    }
}

void playerRequestJump(Player* player)
{
    // Guarda o comando de salto por 0,10 segundo
    player->jumpBufferTimer = JUMP_BUFFER;
}
