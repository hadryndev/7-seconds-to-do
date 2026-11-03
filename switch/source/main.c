
#include <switch.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include "game.h"
#include "physics.h"
#include "levels.h"

#define FB_WIDTH 1280
#define FB_HEIGHT 720
#define SCALE 2
#define OFFSET_Y 40

#define WHITE  RGBA8_MAXALPHA(255, 255, 255)
#define BLACK  RGBA8_MAXALPHA(0, 0, 0)
#define YELLOW RGBA8_MAXALPHA(255, 212, 0)
#define RED    RGBA8_MAXALPHA(229, 57, 53)
#define GREEN  RGBA8_MAXALPHA(32, 180, 85)
#define GRAY   RGBA8_MAXALPHA(146, 146, 146)

#define DOOR_X (currentStage == 0 ? 16 : (currentStage == 1 ? 608 : 2340))
#define DOOR_Y (currentStage == 2 ? 256 : 16)
#define DOOR_W (currentStage == 2 ? 19 : 16)
#define DOOR_H 32

#define KEY_X (currentStage == 2 ? 1810 : 16)
#define KEY_Y (currentStage == 0 ? 192 : (currentStage == 1 ? 128 : 256))
#define KEY_W 16
#define KEY_H 16

#define TRAIL_COUNT 32
#define TRAIL_INTERVAL 0.035f
#define TRAIL_LIFETIME 0.16f

typedef enum {
    PHASE_PRESENTATION,
    PHASE_CHALLENGE,
    PHASE_COMPLETE
} Phase;

typedef struct {
    float x;
    float y;
    float life;
    int direction;
} Trail;

static int currentStage = 0; // 0 Zigzagging, 1 Parkour, 2 Gauntlet
static float cameraX = 0.0f;

static Trail trails[TRAIL_COUNT];
static int trailCount = 0;
static float trailClock = 0.0f;

static const unsigned char digitFont[11][7] = {
    {14,17,19,21,25,17,14}, // 0
    {4,12,4,4,4,4,14},       // 1
    {14,17,1,2,4,8,31},     // 2
    {30,1,1,14,1,1,30},     // 3
    {2,6,10,18,31,2,2},    // 4
    {31,16,16,30,1,1,30},   // 5
    {14,16,16,30,17,17,14}, // 6
    {31,1,2,4,8,8,8},       // 7
    {14,17,17,14,17,17,14}, // 8
    {14,17,17,15,1,1,14},   // 9
    {0,4,4,0,4,4,0}         // :
};

static void drawRect(u32* pixels, u32 stride,
                     int x, int y, int w, int h, u32 color)
{
    int left = (x - (int)cameraX) * SCALE;
    int top = y * SCALE + OFFSET_Y;
    int right = (x + w - (int)cameraX) * SCALE;
    int bottom = (y + h) * SCALE + OFFSET_Y;

    for (int py = top; py < bottom; py++)
    {
        if (py < 0 || py >= FB_HEIGHT)
            continue;

        for (int px = left; px < right; px++)
        {
            if (px < 0 || px >= FB_WIDTH)
                continue;

            pixels[py * stride + px] = color;
        }
    }
}

static double getTimeSeconds(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (double)ts.tv_sec +
           (double)ts.tv_nsec / 1000000000.0;
}

static bool overlaps(float ax, float ay, float aw, float ah,
                     float bx, float by, float bw, float bh)
{
    return ax < bx + bw &&
           ax + aw > bx &&
           ay < by + bh &&
           ay + ah > by;
}

static bool touches(float ax, float ay, float aw, float ah,
                    float bx, float by, float bw, float bh)
{
    const float e = 0.001f;

    return ax <= bx + bw + e &&
           ax + aw >= bx - e &&
           ay <= by + bh + e &&
           ay + ah >= by - e;
}

static void clearTrails(void)
{
    trailCount = 0;
    trailClock = 0.0f;
}

static void resetPlayer(Player* player)
{
    *player = (Player){0};
    player->x = currentStage == 2 ? 48.0f : 16.0f;
    player->y = currentStage == 0 ? 272.0f : (currentStage == 1 ? 288.0f : 272.0f);
    cameraX = 0.0f;
    player->onGround = 1;
    clearTrails();
}

static void moveHorizontal(Player* player, float dt)
{
    float distance = player->vx * dt;

    while (distance != 0.0f)
    {
        float step = distance;

        if (step > 1.0f) step = 1.0f;
        if (step < -1.0f) step = -1.0f;

        float nextX = player->x + step;

        if (levelCollides(nextX, player->y,
                          PLAYER_SIZE, PLAYER_SIZE))
        {
            player->vx = 0.0f;
            break;
        }

        player->x = nextX;
        distance -= step;

        if (fabsf(distance) < 0.00001f)
            break;
    }
}

static void moveVertical(Player* player, float dt)
{
    float distance = player->vy * dt;
    player->onGround = 0;

    while (distance != 0.0f)
    {
        float step = distance;

        if (step > 1.0f) step = 1.0f;
        if (step < -1.0f) step = -1.0f;

        float nextY = player->y + step;

        if (levelCollides(player->x, nextY,
                          PLAYER_SIZE, PLAYER_SIZE))
        {
            if (step > 0.0f)
                player->onGround = 1;

            player->vy = 0.0f;
            break;
        }

        player->y = nextY;
        distance -= step;

        if (fabsf(distance) < 0.00001f)
            break;
    }

    if (player->vy >= 0.0f &&
        levelCollides(player->x, player->y + 0.1f,
                      PLAYER_SIZE, PLAYER_SIZE))
    {
        player->onGround = 1;
        player->vy = 0.0f;
    }
}

static void drawKey(u32* pixels, u32 stride)
{
    drawRect(pixels, stride, KEY_X + 2, KEY_Y + 2, 8, 8, RED);
    drawRect(pixels, stride, KEY_X + 4, KEY_Y + 4, 4, 4, WHITE);
    drawRect(pixels, stride, KEY_X + 8, KEY_Y + 7, 3, 7, RED);
    drawRect(pixels, stride, KEY_X + 10, KEY_Y + 11, 4, 3, RED);
}

// Fonte pixelada para o cronometro
static void drawDigit(u32* pixels, u32 stride,
                      int x, int y, int character,
                      int pixelSize, u32 color)
{
    if (character < 0 || character > 10)
        return;

    for (int row = 0; row < 7; row++)
    {
        for (int col = 0; col < 5; col++)
        {
            if (digitFont[character][row] & (1 << (4 - col)))
            {
                drawRect(
                    pixels, stride,
                    x + col * pixelSize,
                    y + row * pixelSize,
                    pixelSize, pixelSize,
                    color
                );
            }
        }
    }
}

// Formato 07:00 ate 00:00
static void drawTimer(u32* pixels, u32 stride,
                      float remaining, double now)
{
    if (remaining < 0.0f) remaining = 0.0f;
    if (remaining > 7.0f) remaining = 7.0f;

    // Exibicao em centesimos, sem alterar o prazo real
    int centiseconds = (int)ceilf(remaining * 100.0f - 0.0001f);

    if (centiseconds < 0) centiseconds = 0;
    if (centiseconds > 700) centiseconds = 700;

    int seconds = centiseconds / 100;
    int hundredths = centiseconds % 100;

    bool danger = remaining <= 4.0f;

    // Pisca 8 vezes por segundo na zona critica
bool visible = !danger || (((long long)(now * 16.0)) % 2 == 0);

    if (!visible)
        return;

    u32 color = danger ? RED : BLACK;

    int digits[5] = {
        seconds / 10,
        seconds % 10,
        10,
        hundredths / 10,
        hundredths % 10
    };

    int x = 532;
    const int y = 8;
    const int pixelSize = 3;
    const int advance = 18;

    for (int i = 0; i < 5; i++)
    {
        drawDigit(pixels, stride,
                  x + i * advance, y,
                  digits[i], pixelSize, color);
    }
}

static void updateTrails(const Player* player,
                         bool turbo, float dt)
{
    // Envelhece os rastros existentes
    for (int i = 0; i < trailCount;)
    {
        trails[i].life -= dt;

        if (trails[i].life <= 0.0f)
        {
            for (int j = i; j < trailCount - 1; j++)
                trails[j] = trails[j + 1];

            trailCount--;
        }
        else
        {
            i++;
        }
    }

    if (!turbo || fabsf(player->vx) < 0.01f)
    {
        trailClock = 0.0f;
        return;
    }

    int direction = player->vx > 0.0f ? 1 : -1;

    trailClock += dt;

    while (trailClock >= TRAIL_INTERVAL)
    {
        trailClock -= TRAIL_INTERVAL;

        if (trailCount >= TRAIL_COUNT)
        {
            for (int j = 0; j < TRAIL_COUNT - 1; j++)
                trails[j] = trails[j + 1];

            trailCount--;
        }

        trails[trailCount++] = (Trail){
            .x = player->x - direction * 7.0f,
            .y = player->y + 3.0f,
            .life = TRAIL_LIFETIME,
            .direction = direction
        };
    }
}


static u32 blendColor(u32 foreground, u32 background, float alpha)
{
    if (alpha < 0.0f) alpha = 0.0f;
    if (alpha > 1.0f) alpha = 1.0f;

    u8 fr = (u8)(foreground & 0xFF);
    u8 fg = (u8)((foreground >> 8) & 0xFF);
    u8 fb = (u8)((foreground >> 16) & 0xFF);

    u8 br = (u8)(background & 0xFF);
    u8 bg = (u8)((background >> 8) & 0xFF);
    u8 bb = (u8)((background >> 16) & 0xFF);

    u8 r = (u8)(fr * alpha + br * (1.0f - alpha));
    u8 g = (u8)(fg * alpha + bg * (1.0f - alpha));
    u8 b = (u8)(fb * alpha + bb * (1.0f - alpha));

    return RGBA8_MAXALPHA(r, g, b);
}

static void drawTrails(u32* pixels, u32 stride)
{
    for (int i = 0; i < trailCount; i++)
    {
        Trail* t = &trails[i];

        float alpha = t->life * 2.5f;

        if (alpha > 0.55f)
            alpha = 0.55f;

        u32 trailGray = blendColor(GRAY, WHITE, alpha);
        u32 trailYellow = blendColor(YELLOW, WHITE, alpha);

        drawRect(
            pixels, stride,
            (int)t->x, (int)t->y,
            12, 10, trailGray
        );

        drawRect(
            pixels, stride,
            (int)t->x - t->direction * 5,
            (int)t->y + 4,
            5, 4, trailYellow
        );
    }
}


// Gauntlet: plataformas, inimigos e respawn baseados na Alpha web v0.0.4_7.
#define G_COUNT_FIXED 12
#define G_COUNT_GRAVITY 6
#define G_COUNT_MOVING 4
#define G_COUNT_ENEMIES 5
#define G_W 2400.0f
#define G_H 320.0f
#define G_BLUE RGBA8_MAXALPHA(50,139,221)
#define G_RED RGBA8_MAXALPHA(223,70,70)
#define G_PURPLE RGBA8_MAXALPHA(152,96,215)
#define G_MOVEBLUE RGBA8_MAXALPHA(67,136,231)
#define G_GRAVITY RGBA8_MAXALPHA(36,169,106)
#define G_GRAVITY_ACTIVE RGBA8_MAXALPHA(21,139,88)
typedef struct {float x,y,w,h,oldX,oldY,dx,dy;int active;} GRect;
typedef struct {GRect b;float min,max,speed;int dir,axis;} GMoving;
typedef struct {GRect b;float vy;int triggered;} GGravity;
typedef struct {GRect b;float min,max,speed;int dir,blue,alive;} GEnemy;
static GRect gFixed[G_COUNT_FIXED] = {
 {0,288,256,32},{710,288,455,32},{1400,288,495,32},{2125,288,275,32},
 {0,0,2400,16},{0,16,16,272},{2384,16,16,272},
 {791,226,88,16},{956,215,88,16},{1492,225,90,16},
 {1640,207,80,16},{1770,230,90,16}
};
static const GMoving gMovingInitial[G_COUNT_MOVING] = {
 {{1175,250,68,16},172,250,47,-1,1},
 {{1285,210,68,16},1270,1335,54,1,0},
 {{1905,241,78,16},1905,2034,63,1,0},
 {{2050,240,67,16},190,249,49,-1,1}
};
static const GRect gGravityInitial[G_COUNT_GRAVITY] = {
 {260,259,65,16},{335,232,65,16},{410,206,65,16},
 {485,218,65,16},{560,239,65,16},{635,258,65,16}
};
static const GEnemy gEnemyInitial[G_COUNT_ENEMIES] = {
 {{835,272,16,16},752,918,46,1,1,1},
 {{1072,272,16,16},1045,1124,54,-1,0,1},
 {{1505,272,16,16},1440,1605,51,1,1,1},
 {{1810,272,16,16},1733,1867,60,-1,0,1},
 {{2160,272,16,16},2144,2270,53,1,1,1}
};
static GMoving gMoving[G_COUNT_MOVING];
static GGravity gGravity[G_COUNT_GRAVITY];
static GEnemy gEnemies[G_COUNT_ENEMIES];
static GRect* gRide = NULL;
static float gCameraLook = .35f;
static int gFacing = 1;
static float gCoyote = 0.0f;
static float gJumpBuffer = 0.0f;
static float gVisualClock = 0.0f;
static float gClamp(float n,float a,float b){return n<a?a:(n>b?b:n);}
static int gOverlapX(float x,float w,const GRect* b){return x<b->x+b->w&&x+w>b->x;}
static int gCollision(float x,float y,float w,float h,const GRect* b){
 return x<b->x+b->w&&x+w>b->x&&y<b->y+b->h&&y+h>b->y;
}
static void gResetObjects(int resetMoving){
 if(resetMoving){for(int i=0;i<G_COUNT_MOVING;i++)gMoving[i]=gMovingInitial[i];}
 for(int i=0;i<G_COUNT_GRAVITY;i++){
  gGravity[i]=(GGravity){0};gGravity[i].b=gGravityInitial[i];
 }
 for(int i=0;i<G_COUNT_ENEMIES;i++)gEnemies[i]=gEnemyInitial[i];
}
static void gSpawn(Player* p){
 *p=(Player){0};p->x=48;p->y=272;p->onGround=1;
 p->coyoteTimer=COYOTE_TIME;gCoyote=COYOTE_TIME;gJumpBuffer=0;
 gRide=NULL;cameraX=0;gCameraLook=.35f;gFacing=1;gVisualClock=0;
 clearTrails();
}
static void gRespawn(Player* p,Phase phase,bool* doorOpen,bool* timerRunning,
                      float* remaining,double* deadline){
 gSpawn(p);gResetObjects(0);
 *doorOpen=(phase==PHASE_PRESENTATION);*timerRunning=false;
 *remaining=7.0f;*deadline=0.0;
}
static void gUpdateWorld(float dt){
 for(int i=0;i<G_COUNT_MOVING;i++){
  GMoving* m=&gMoving[i];GRect* b=&m->b;
  b->oldX=b->x;b->oldY=b->y;
  float* value=m->axis?&b->y:&b->x;
  float n=*value+m->dir*m->speed*dt;
  if(n>m->max){n=m->max-(n-m->max);m->dir=-1;}
  if(n<m->min){n=m->min+(m->min-n);m->dir=1;}
  *value=n;b->dx=b->x-b->oldX;b->dy=b->y-b->oldY;
 }
 for(int i=0;i<G_COUNT_GRAVITY;i++){
  GGravity* g=&gGravity[i];GRect* b=&g->b;
  b->oldX=b->x;b->oldY=b->y;
  if(g->triggered){g->vy=gClamp(g->vy+670*dt,0,320);b->y+=g->vy*dt;}
  b->dx=0;b->dy=b->y-b->oldY;
 }
 for(int i=0;i<G_COUNT_ENEMIES;i++){
  GEnemy* e=&gEnemies[i];if(!e->alive)continue;
  float n=e->b.x+e->dir*e->speed*dt;
  if(n>e->max){n=e->max-(n-e->max);e->dir=-1;}
  if(n<e->min){n=e->min+(e->min-n);e->dir=1;}
  e->b.x=n;
 }
}
static GRect* gGetPlatform(int index){
 if(index<G_COUNT_FIXED)return &gFixed[index];
 index-=G_COUNT_FIXED;
 if(index<G_COUNT_MOVING)return &gMoving[index].b;
 index-=G_COUNT_MOVING;
 return &gGravity[index].b;
}
static void gUpdate(Player* p,Phase phase,bool* doorOpen,bool* timerRunning,
                    float* remaining,double* deadline,float dt,double now,
                    int direction,bool jumpPressed){
 gVisualClock+=dt;gUpdateWorld(dt);
 if(*timerRunning){
  double left=*deadline-now;
  if(left<=0){*timerRunning=false;*doorOpen=false;*remaining=7;*deadline=0;p->hasKey=0;clearTrails();}
  else *remaining=(float)left;
 }
 if(gRide){p->x+=gRide->dx;p->y+=gRide->dy;}
 if(jumpPressed)gJumpBuffer=JUMP_BUFFER;
 gJumpBuffer=gClamp(gJumpBuffer-dt,0,JUMP_BUFFER);
 if(p->onGround)gCoyote=COYOTE_TIME;
 else gCoyote=gClamp(gCoyote-dt,0,COYOTE_TIME);
 if(gJumpBuffer>0&&(p->onGround||gCoyote>0)){
  p->vy=JUMP_VELOCITY;p->onGround=0;gCoyote=0;gJumpBuffer=0;
 }
 if(!p->onGround)gRide=NULL;
 p->vx=direction*(*timerRunning?PLAYER_SPEED_KEY:PLAYER_SPEED);
 if(direction)gFacing=direction;
 updateTrails(p,*timerRunning,dt);
 float previousX=p->x,previousY=p->y;
 p->x+=p->vx*dt;
 for(int i=0;i<G_COUNT_FIXED+G_COUNT_MOVING+G_COUNT_GRAVITY;i++){
  GRect* b=gGetPlatform(i);
  if(i>=G_COUNT_FIXED+G_COUNT_MOVING&&b->y>G_H+16)continue;
  float bx=(i>=G_COUNT_FIXED)?b->oldX:b->x;
  float by=(i>=G_COUNT_FIXED)?b->oldY:b->y;
  if(!(previousY<by+b->h-.01f&&previousY+PLAYER_SIZE>by+.01f))continue;
  if(!gOverlapX(p->x,PLAYER_SIZE,b))continue;
  if(previousX+PLAYER_SIZE<=bx+.01f&&p->x+PLAYER_SIZE>b->x)p->x=b->x-PLAYER_SIZE;
  else if(previousX>=bx+b->w-.01f&&p->x<b->x+b->w)p->x=b->x+b->w;
 }
 p->x=gClamp(p->x,16,G_W-32);
 float oldY=p->y,oldBottom=p->y+PLAYER_SIZE;
 p->vy=gClamp(p->vy+GRAVITY*dt,-1000,550);
 p->y+=p->vy*dt;p->onGround=0;gRide=NULL;
 for(int i=0;i<G_COUNT_FIXED+G_COUNT_MOVING+G_COUNT_GRAVITY;i++){
  GRect* b=gGetPlatform(i);
  if(i>=G_COUNT_FIXED+G_COUNT_MOVING&&b->y>G_H+16)continue;
  if(!gOverlapX(p->x,PLAYER_SIZE,b))continue;
  float by=i>=G_COUNT_FIXED?b->oldY:b->y;
  int above=oldBottom<=by+1.5f;
  int below=oldY>=by+b->h-1.5f;
  float relative=(p->y-oldY)-b->dy;
  int land=0;
  if(above&&p->y+PLAYER_SIZE>=b->y-.001f&&relative>=-.001f)land=1;
  else if(below&&p->y<=b->y+b->h+.001f&&relative<=.001f){
   p->y=b->y+b->h;if(p->vy<0)p->vy=0;
  }else if(gCollision(p->x,p->y,PLAYER_SIZE,PLAYER_SIZE,b)){
   float topDepth=p->y+PLAYER_SIZE-b->y;
   float bottomDepth=b->y+b->h-p->y;
   if(topDepth<=bottomDepth&&oldBottom<=by+8)land=1;
   else {p->y=b->y+b->h;if(p->vy<0)p->vy=0;}
  }
  if(land){
   p->y=b->y-PLAYER_SIZE;p->vy=0;p->onGround=1;
   if(i>=G_COUNT_FIXED)gRide=b;
   if(i>=G_COUNT_FIXED+G_COUNT_MOVING){
    GGravity* g=&gGravity[i-G_COUNT_FIXED-G_COUNT_MOVING];
    if(!g->triggered){g->triggered=1;g->vy=0;}
   }
  }
 }
 if(p->onGround){
  gCoyote=COYOTE_TIME;
  if(gJumpBuffer>0){p->vy=JUMP_VELOCITY;p->onGround=0;gCoyote=0;gJumpBuffer=0;gRide=NULL;}
 }
 for(int i=0;i<G_COUNT_ENEMIES;i++){
  GEnemy* e=&gEnemies[i];if(!e->alive)continue;
  if(!gCollision(p->x,p->y,PLAYER_SIZE,PLAYER_SIZE,&e->b))continue;
  if(e->blue&&p->vy>=0&&oldBottom<=e->b.y+4){
   e->alive=0;p->y=e->b.y-PLAYER_SIZE;p->vy=-275;
   p->onGround=0;gCoyote=0;gRide=NULL;
  }else{gRespawn(p,phase,doorOpen,timerRunning,remaining,deadline);return;}
 }
 if(p->y>G_H+24){gRespawn(p,phase,doorOpen,timerRunning,remaining,deadline);return;}
 if(phase==PHASE_CHALLENGE&&!*doorOpen&&
    overlaps(p->x,p->y,PLAYER_SIZE,PLAYER_SIZE,KEY_X,KEY_Y,KEY_W,KEY_H)){
  *doorOpen=true;*timerRunning=true;p->hasKey=1;*remaining=7;
  *deadline=getTimeSeconds()+7.0;clearTrails();
 }
 if(*timerRunning){
  double left=*deadline-getTimeSeconds();
  if(left<=0){*timerRunning=false;*doorOpen=false;*remaining=7;*deadline=0;p->hasKey=0;clearTrails();}
  else *remaining=(float)left;
 }
 float targetLook=gFacing>0?.35f:.65f;
 gCameraLook+=(targetLook-gCameraLook)*(dt*5.0f/(1.0f+dt*5.0f));
 float target=gClamp(p->x+PLAYER_SIZE/2-640*gCameraLook,0,G_W-640);
 cameraX+=(target-cameraX)*(dt*12.0f/(1.0f+dt*12.0f));
 cameraX=gClamp(cameraX,0,G_W-640);
}
static void gDraw(u32* pixels,u32 stride){
 for(int i=0;i<G_COUNT_GRAVITY;i++){
  GGravity* g=&gGravity[i];GRect* b=&g->b;
  if(b->y>G_H+16)continue;
  drawRect(pixels,stride,(int)b->x,(int)b->y,(int)b->w,(int)b->h,
           g->triggered?G_GRAVITY_ACTIVE:G_GRAVITY);
 }
 for(int i=0;i<G_COUNT_MOVING;i++){
  GMoving* m=&gMoving[i];GRect* b=&m->b;
  drawRect(pixels,stride,(int)b->x,(int)b->y,(int)b->w,(int)b->h,
           m->axis?G_PURPLE:G_MOVEBLUE);
 }
 for(int i=0;i<G_COUNT_ENEMIES;i++){
  GEnemy* e=&gEnemies[i];if(!e->alive)continue;
  int x=(int)e->b.x,y=(int)e->b.y;
  drawRect(pixels,stride,x,y,16,16,e->blue?G_BLUE:G_RED);
  drawRect(pixels,stride,x+3,y+5,3,4,WHITE);
  drawRect(pixels,stride,x+10,y+5,3,4,WHITE);
 }
}


// Interface nativa: desenhada em pixels de tela para nao seguir a camera.
typedef enum {
 UI_MENU, UI_SETTINGS, UI_CREDITS, UI_CHANGELOG,
 UI_PLAY, UI_PRESENTATION_DONE, UI_STAGE_DONE, UI_FINAL
} UIState;

typedef struct {char letter; unsigned char rows[7];} Glyph;
static const Glyph uiGlyphs[] = {
    {'A', {0x0E,0x11,0x11,0x1F,0x11,0x11,0x11}},
    {'B', {0x1E,0x11,0x11,0x1E,0x11,0x11,0x1E}},
    {'C', {0x0F,0x10,0x10,0x10,0x10,0x10,0x0F}},
    {'D', {0x1E,0x11,0x11,0x11,0x11,0x11,0x1E}},
    {'E', {0x1F,0x10,0x10,0x1E,0x10,0x10,0x1F}},
    {'F', {0x1F,0x10,0x10,0x1E,0x10,0x10,0x10}},
    {'G', {0x0F,0x10,0x10,0x17,0x11,0x11,0x0F}},
    {'H', {0x11,0x11,0x11,0x1F,0x11,0x11,0x11}},
    {'I', {0x1F,0x04,0x04,0x04,0x04,0x04,0x1F}},
    {'J', {0x07,0x02,0x02,0x02,0x12,0x12,0x0C}},
    {'K', {0x11,0x12,0x14,0x18,0x14,0x12,0x11}},
    {'L', {0x10,0x10,0x10,0x10,0x10,0x10,0x1F}},
    {'M', {0x11,0x1B,0x15,0x15,0x11,0x11,0x11}},
    {'N', {0x11,0x19,0x15,0x13,0x11,0x11,0x11}},
    {'O', {0x0E,0x11,0x11,0x11,0x11,0x11,0x0E}},
    {'P', {0x1E,0x11,0x11,0x1E,0x10,0x10,0x10}},
    {'Q', {0x0E,0x11,0x11,0x11,0x15,0x12,0x0D}},
    {'R', {0x1E,0x11,0x11,0x1E,0x14,0x12,0x11}},
    {'S', {0x0F,0x10,0x10,0x0E,0x01,0x01,0x1E}},
    {'T', {0x1F,0x04,0x04,0x04,0x04,0x04,0x04}},
    {'U', {0x11,0x11,0x11,0x11,0x11,0x11,0x0E}},
    {'V', {0x11,0x11,0x11,0x11,0x11,0x0A,0x04}},
    {'W', {0x11,0x11,0x11,0x15,0x15,0x15,0x0A}},
    {'X', {0x11,0x11,0x0A,0x04,0x0A,0x11,0x11}},
    {'Y', {0x11,0x11,0x0A,0x04,0x04,0x04,0x04}},
    {'Z', {0x1F,0x01,0x02,0x04,0x08,0x10,0x1F}},
    {'0', {0x0E,0x11,0x13,0x15,0x19,0x11,0x0E}},
    {'1', {0x04,0x0C,0x04,0x04,0x04,0x04,0x0E}},
    {'2', {0x0E,0x11,0x01,0x02,0x04,0x08,0x1F}},
    {'3', {0x1E,0x01,0x01,0x0E,0x01,0x01,0x1E}},
    {'4', {0x02,0x06,0x0A,0x12,0x1F,0x02,0x02}},
    {'5', {0x1F,0x10,0x10,0x1E,0x01,0x01,0x1E}},
    {'6', {0x0F,0x10,0x10,0x1E,0x11,0x11,0x0E}},
    {'7', {0x1F,0x01,0x02,0x04,0x08,0x08,0x08}},
    {'8', {0x0E,0x11,0x11,0x0E,0x11,0x11,0x0E}},
    {'9', {0x0E,0x11,0x11,0x0F,0x01,0x01,0x1E}},
    {'!', {0x04,0x04,0x04,0x04,0x04,0x00,0x04}},
    {'?', {0x0E,0x11,0x01,0x02,0x04,0x00,0x04}},
    {'.', {0x00,0x00,0x00,0x00,0x00,0x04,0x04}},
    {':', {0x00,0x04,0x04,0x00,0x04,0x04,0x00}},
    {'-', {0x00,0x00,0x00,0x1F,0x00,0x00,0x00}},
    {'/', {0x01,0x01,0x02,0x04,0x08,0x10,0x10}},
    {'+', {0x00,0x04,0x04,0x1F,0x04,0x04,0x00}},
    {'>', {0x10,0x08,0x04,0x02,0x04,0x08,0x10}},
    {'(', {0x02,0x04,0x08,0x08,0x08,0x04,0x02}},
    {')', {0x08,0x04,0x02,0x02,0x02,0x04,0x08}},
    {' ', {0x00,0x00,0x00,0x00,0x00,0x00,0x00}}
};
static const char* uiStageNames[] = {"ZIGZAGGING", "PARKOUR", "THE GAUNTLET"};

static void screenRect(u32* pixels, u32 stride, int x,int y,int w,int h,u32 color){
 int x0=x<0?0:x, y0=y<0?0:y;
 int x1=x+w>FB_WIDTH?FB_WIDTH:x+w;
 int y1=y+h>FB_HEIGHT?FB_HEIGHT:y+h;
 for(int py=y0;py<y1;py++) for(int px=x0;px<x1;px++)pixels[py*stride+px]=color;
}
static const unsigned char* uiGlyph(char ch){
 for(unsigned int i=0;i<sizeof(uiGlyphs)/sizeof(uiGlyphs[0]);i++)
  if(uiGlyphs[i].letter==ch)return uiGlyphs[i].rows;
 return uiGlyphs[sizeof(uiGlyphs)/sizeof(uiGlyphs[0])-1].rows;
}
static int uiTextWidth(const char* t,int scale){return (int)strlen(t)*6*scale;}
static void uiText(u32* pixels,u32 stride,int x,int y,const char* text,int scale,u32 color){
 for(const char* p=text;*p;p++){
  const unsigned char* rows=uiGlyph(*p);
  for(int r=0;r<7;r++)for(int c=0;c<5;c++)
   if(rows[r]&(1<<(4-c)))screenRect(pixels,stride,x+c*scale,y+r*scale,scale,scale,color);
  x+=6*scale;
 }
}
static void uiCentered(u32* pixels,u32 stride,int y,const char* t,int scale,u32 color){
 uiText(pixels,stride,(FB_WIDTH-uiTextWidth(t,scale))/2,y,t,scale,color);
}
static void uiBorder(u32* pixels,u32 stride,int x,int y,int w,int h,u32 color){
 screenRect(pixels,stride,x,y,w,4,color);screenRect(pixels,stride,x,y+h-4,w,4,color);
 screenRect(pixels,stride,x,y,4,h,color);screenRect(pixels,stride,x+w-4,y,4,h,color);
}
static void uiButton(u32* pixels,u32 stride,int x,int y,int w,const char* title,bool selected){
 screenRect(pixels,stride,x+8,y+8,w,58,YELLOW);
 screenRect(pixels,stride,x,y,w,58,selected?YELLOW:WHITE);
 uiBorder(pixels,stride,x,y,w,58,BLACK);
 uiText(pixels,stride,x+(w-uiTextWidth(title,3))/2,y+19,title,3,BLACK);
}
static void uiPanel(u32* pixels,u32 stride,UIState ui,int selection,bool showHelp){
 screenRect(pixels,stride,0,0,FB_WIDTH,FB_HEIGHT,WHITE);
 if(ui==UI_MENU){
  // Paleta, tipografia quadrada e assinatura visual baseadas na versao web.
  uiCentered(pixels,stride,105,"7 SECONDS",8,BLACK);
  uiCentered(pixels,stride,180,"TO DO!",9,GREEN);
  uiCentered(pixels,stride,279,"ALPHA V0.0.4_7 - A GAME BY HADRYN",2,GRAY);
  const char* choices[]={"INICIAR","CONFIGURACOES","CREDITOS","CHANGELOG"};
  for(int i=0;i<4;i++)uiButton(pixels,stride,448,340+i*76,384,choices[i],selection==i);
  uiCentered(pixels,stride,679,"D-PAD: SELECIONAR   A: CONFIRMAR",2,GRAY);
 }else if(ui==UI_SETTINGS){
  uiCentered(pixels,stride,130,"CONFIGURACOES",6,BLACK);
  uiButton(pixels,stride,320,320,640,showHelp?"INSTRUCOES: SIM":"INSTRUCOES: NAO",true);
  uiCentered(pixels,stride,435,"A: ALTERAR   B: VOLTAR",3,BLACK);
  uiCentered(pixels,stride,490,"OUTRAS OPCOES VISUAIS: WEB",2,GRAY);
 }else if(ui==UI_CREDITS){
  uiCentered(pixels,stride,112,"CREDITOS",7,BLACK);
  uiCentered(pixels,stride,250,"7 SECONDS TO DO!",4,BLACK);
  uiCentered(pixels,stride,322,"MEMORIZE O CAMINHO. VOCE TEM 7 SEGUNDOS!",2,BLACK);
  uiCentered(pixels,stride,397,"CRIACAO, CONCEITO E DIRECAO: HADRYN",2,BLACK);
  uiCentered(pixels,stride,441,"PROTOTIPO: HADRYN COM ASSISTENCIA DO CHATGPT",2,BLACK);
  uiCentered(pixels,stride,571,"B: VOLTAR",3,GRAY);
 }else if(ui==UI_CHANGELOG){
  uiCentered(pixels,stride,105,"CHANGELOG",7,BLACK);
  uiCentered(pixels,stride,234,"ALPHA V0.0.4_7 - MOVING PLATFORMS & ENEMIES",2,BLACK);
  uiCentered(pixels,stride,289,"THE GAUNTLET: FASE LONGA E CAMERA DINAMICA",2,BLACK);
  uiCentered(pixels,stride,329,"PLATAFORMAS MOVEIS, GRAVIDADE E INIMIGOS",2,BLACK);
  uiCentered(pixels,stride,389,"ALPHA V0.0.4_5 - REFINAMENTO DA FISICA",2,BLACK);
  uiCentered(pixels,stride,429,"COYOTE TIME, JUMP BUFFER E COLISOES",2,BLACK);
  uiCentered(pixels,stride,571,"B: VOLTAR",3,GRAY);
 }else{
  uiCentered(pixels,stride,170,ui==UI_FINAL?"TODAS AS FASES CONCLUIDAS!":
       (ui==UI_PRESENTATION_DONE?"APRESENTACAO CONCLUIDA!":"FASE CONCLUIDA!"),5,BLACK);
  if(ui==UI_FINAL){
   uiCentered(pixels,stride,305,"VOCE CONCLUIU AS TRES FASES,",3,BLACK);
   uiCentered(pixels,stride,345,"INCLUINDO THE GAUNTLET. PARABENS!",3,BLACK);
  }else{
   uiCentered(pixels,stride,310,uiStageNames[currentStage],4,GREEN);
   uiCentered(pixels,stride,380,ui==UI_PRESENTATION_DONE?"PREPARE-SE PARA O DESAFIO":"PROXIMA FASE",2,BLACK);
  }
  uiButton(pixels,stride,430,495,420,ui==UI_FINAL?"VOLTAR AO MENU":"CONTINUAR",true);
  uiCentered(pixels,stride,640,"A: CONFIRMAR   B: MENU",2,GRAY);
 }
}
static void uiHud(u32* pixels,u32 stride,Phase phase,bool timerRunning,float remaining,bool showHelp,double now){
 char line[96];
 if(timerRunning){
  int tenths=(int)(remaining*10.0f+0.999f);if(tenths<0)tenths=0;
  if(tenths>70)tenths=70;
  snprintf(line,sizeof(line),"%d.%d SEGUNDOS",tenths/10,tenths%10);
 }else{
  snprintf(line,sizeof(line),"FASE %d - %s - %s",currentStage+1,uiStageNames[currentStage],
           phase==PHASE_PRESENTATION?"APRESENTACAO":"DESAFIO - PEGUE A CHAVE");
 }
 int scale=2;int width=uiTextWidth(line,scale)+22;
 screenRect(pixels,stride,20,12,width,24,WHITE);
 uiBorder(pixels,stride,20,12,width,24,timerRunning&&remaining<=3.0f?RED:BLACK);
 if(!timerRunning || remaining>3.0f || (((long long)(now*16.0))%2==0))
  uiText(pixels,stride,30,17,line,scale,timerRunning&&remaining<=3.0f?RED:BLACK);
 if(showHelp){
  uiText(pixels,stride,20,690,"D-PAD: MOVER  B: PULAR  X: REINICIAR  +: MENU",2,GRAY);
 }
}

int main(int argc, char* argv[])
{
    Game game;
    gameInit(&game);
    levelInit();
    levelSetStage(currentStage);

    Player player;
    resetPlayer(&player);
    gResetObjects(1);

    Phase phase = PHASE_PRESENTATION;

    bool doorOpen = true;
    bool timerRunning = false;

    double deadline = 0.0;
    float remainingTime = 7.0f;

    NWindow* window = nwindowGetDefault();

    Framebuffer fb;
    Result rc = framebufferCreate(
        &fb, window, FB_WIDTH, FB_HEIGHT,
        PIXEL_FORMAT_RGBA_8888, 2
    );

    if (R_FAILED(rc))
        return 1;

    framebufferMakeLinear(&fb);

    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    PadState pad;
    padInitializeDefault(&pad);

    UIState ui = UI_MENU;
    int menuSelection = 0;
    bool showHelp = true;
    double previousTime = getTimeSeconds();

    while (appletMainLoop())
    {
        double now = getTimeSeconds();
        float dt = (float)(now - previousTime);
        previousTime = now;

        if (dt < 0.0f) dt = 0.0f;
        if (dt > 0.05f) dt = 0.05f;

        padUpdate(&pad);

        u64 buttons = padGetButtons(&pad);
        u64 buttonsDown = padGetButtonsDown(&pad);

        if (buttonsDown & HidNpadButton_Plus) {
            ui = UI_MENU;
            menuSelection = 0;
        }
        if (ui != UI_PLAY) {
            if (ui == UI_MENU) {
                if (buttonsDown & (HidNpadButton_Up | HidNpadButton_StickLUp))
                    menuSelection = (menuSelection + 3) % 4;
                if (buttonsDown & (HidNpadButton_Down | HidNpadButton_StickLDown))
                    menuSelection = (menuSelection + 1) % 4;
                if (buttonsDown & HidNpadButton_A) {
                    if (menuSelection == 0) {
                        currentStage=0;levelSetStage(0);phase=PHASE_PRESENTATION;
                        resetPlayer(&player);gResetObjects(1);
                        timerRunning=false;remainingTime=7;deadline=0;doorOpen=true;
                        ui=UI_PLAY;
                    } else if (menuSelection==1)ui=UI_SETTINGS;
                    else if (menuSelection==2)ui=UI_CREDITS;
                    else ui=UI_CHANGELOG;
                }
            }else if(ui==UI_SETTINGS){
                if(buttonsDown & HidNpadButton_A)showHelp=!showHelp;
                if(buttonsDown & HidNpadButton_B)ui=UI_MENU;
            }else if(ui==UI_CREDITS || ui==UI_CHANGELOG){
                if(buttonsDown & (HidNpadButton_B | HidNpadButton_A))ui=UI_MENU;
            }else if(ui==UI_PRESENTATION_DONE){
                if(buttonsDown & HidNpadButton_A){
                    phase=PHASE_CHALLENGE;resetPlayer(&player);
                    if(currentStage==2){gSpawn(&player);gResetObjects(1);}
                    timerRunning=false;remainingTime=7;deadline=0;doorOpen=false;
                    ui=UI_PLAY;
                }
                if(buttonsDown & HidNpadButton_B)ui=UI_MENU;
            }else if(ui==UI_STAGE_DONE){
                if(buttonsDown & HidNpadButton_A){
                    currentStage++;levelSetStage(currentStage);
                    phase=PHASE_PRESENTATION;resetPlayer(&player);
                    if(currentStage==2){gSpawn(&player);gResetObjects(1);}
                    timerRunning=false;remainingTime=7;deadline=0;doorOpen=true;
                    ui=UI_PLAY;
                }
                if(buttonsDown & HidNpadButton_B)ui=UI_MENU;
            }else if(ui==UI_FINAL){
                if(buttonsDown & (HidNpadButton_A | HidNpadButton_B))ui=UI_MENU;
            }
        }
        if (ui == UI_PLAY) {


        // X reinicia a etapa atual
        if (buttonsDown & HidNpadButton_X)
        {
            resetPlayer(&player);
            if(currentStage==2){gSpawn(&player);gResetObjects(1);}
            timerRunning = false;
            remainingTime = 7.0f;
            deadline = 0.0;
            doorOpen = (phase == PHASE_PRESENTATION);
        }

        if (timerRunning && currentStage != 2)
        {
            double secondsLeft = deadline - getTimeSeconds();

            if (secondsLeft <= 0.0)
            {
                timerRunning = false;
                doorOpen = false;
                remainingTime = 7.0f;
                deadline = 0.0;
                player.hasKey = 0;
                clearTrails();
            }
            else
            {
                remainingTime = (float)secondsLeft;
            }
        }

        if (phase != PHASE_COMPLETE && currentStage == 2)
        {
            int direction = 0;
            if(buttons & (HidNpadButton_Left | HidNpadButton_StickLLeft)) direction--;
            if(buttons & (HidNpadButton_Right | HidNpadButton_StickLRight)) direction++;
            gUpdate(&player,phase,&doorOpen,&timerRunning,&remainingTime,&deadline,
                    dt,now,direction,(buttonsDown & HidNpadButton_B)!=0);
            if(doorOpen && touches(player.x,player.y,PLAYER_SIZE,PLAYER_SIZE,
                                    DOOR_X,DOOR_Y,DOOR_W,DOOR_H)) {
                if(phase==PHASE_PRESENTATION){
                    phase=PHASE_CHALLENGE;
                    resetPlayer(&player);
                    gSpawn(&player);
                    gResetObjects(1);
                    doorOpen=false;
                    timerRunning=false;
                    remainingTime=7.0f;
                    deadline=0.0;
                    ui=UI_PLAY;
                    clearTrails();
                }else if(phase==PHASE_CHALLENGE && timerRunning &&
                         getTimeSeconds()<=deadline){
                    ui=UI_FINAL;phase=PHASE_COMPLETE;
                    timerRunning=false;clearTrails();player.vx=0;player.vy=0;
                }
            }
        }
        else if (phase != PHASE_COMPLETE)
        {
            int direction = 0;

            if (buttons & (HidNpadButton_Left |
                           HidNpadButton_StickLLeft))
                direction = -1;

            if (buttons & (HidNpadButton_Right |
                           HidNpadButton_StickLRight))
                direction = 1;

            playerMove(&player, direction);

            if (buttonsDown & HidNpadButton_B)
                playerRequestJump(&player);

            if (player.jumpBufferTimer > 0.0f &&
                (player.onGround || player.coyoteTimer > 0.0f))
            {
                playerJump(&player);
                player.jumpBufferTimer = 0.0f;
            }

            playerApplyGravity(&player, dt);

            moveHorizontal(&player, dt);
            moveVertical(&player, dt);

            playerUpdateCoyoteTime(&player, dt);

            if (player.onGround &&
                player.jumpBufferTimer > 0.0f)
            {
                playerJump(&player);
                player.jumpBufferTimer = 0.0f;
            }

            playerUpdateJumpBuffer(&player, dt);

            // Coleta da Red Key
            if (phase == PHASE_CHALLENGE &&
                !doorOpen &&
                overlaps(player.x, player.y,
                         PLAYER_SIZE, PLAYER_SIZE,
                         KEY_X, KEY_Y, KEY_W, KEY_H))
            {
                doorOpen = true;
                timerRunning = true;
                player.hasKey = 1;
                remainingTime = 7.0f;
                deadline = getTimeSeconds() + 7.0;
                clearTrails();
            }

            // Expiracao rigorosa
            if (timerRunning &&
                getTimeSeconds() > deadline)
            {
                timerRunning = false;
                doorOpen = false;
                remainingTime = 7.0f;
                deadline = 0.0;
                player.hasKey = 0;
                clearTrails();
            }

            updateTrails(&player, timerRunning, dt);

            // Contato com a porta
            if (doorOpen &&
                touches(player.x, player.y,
                        PLAYER_SIZE, PLAYER_SIZE,
                        DOOR_X, DOOR_Y, DOOR_W, DOOR_H))
            {
                if (phase == PHASE_PRESENTATION)
                {
                    phase=PHASE_CHALLENGE;
                    resetPlayer(&player);
                    doorOpen=false;
                    timerRunning=false;
                    remainingTime=7.0f;
                    deadline=0.0;
                    clearTrails();
                }
                else if (phase == PHASE_CHALLENGE &&
                         timerRunning &&
                         getTimeSeconds() <= deadline)
                {
                    timerRunning = false;
                    clearTrails();
                    if (currentStage < 2) {
                        currentStage++;
                        levelSetStage(currentStage);
                        phase=PHASE_PRESENTATION;
                        resetPlayer(&player);
                        if(currentStage==2){gSpawn(&player);gResetObjects(1);}
                        doorOpen=true;
                        remainingTime=7.0f;
                        deadline=0.0;
                        ui=UI_PLAY;
                    } else {
                        ui=UI_FINAL;
                        phase=PHASE_COMPLETE;
                    }
                    player.vx = 0.0f;
                    player.vy = 0.0f;
                }
            }
        }

        } // UI_PLAY

        // Camera acompanha a posicao horizontal na fase longa.
        if (currentStage != 2) cameraX = 0.0f;

        u32 strideBytes;
        u32* pixels = (u32*)framebufferBegin(
            &fb, &strideBytes
        );

        u32 stride = strideBytes / sizeof(u32);

        // Fundo branco
        for (int y = 0; y < FB_HEIGHT; y++)
        {
            for (int x = 0; x < FB_WIDTH; x++)
            {
                pixels[y * stride + x] = WHITE;
            }
        }

        // As duas primeiras fases usam o mapa de tiles original.
        // The Gauntlet possui retangulos com coordenadas e dimensoes
        // nao alinhadas ao grid de 16 px; desenhar tiles aqui ampliava
        // visualmente o ground sem ampliar as hitboxes reais.
        if (currentStage == 2)
        {
            for (int i = 0; i < G_COUNT_FIXED; i++)
            {
                const GRect* b = &gFixed[i];
                drawRect(pixels, stride,
                         (int)b->x, (int)b->y,
                         (int)b->w, (int)b->h, YELLOW);
            }
        }
        else
        {
            for (int row = 0; row < LEVEL_ROWS; row++)
            {
                for (int col = 0; col < levelGetCols(); col++)
                {
                    if (levelIsSolid(col, row))
                    {
                        drawRect(pixels, stride,
                                 col * TILE_SIZE, row * TILE_SIZE,
                                 TILE_SIZE, TILE_SIZE, YELLOW);
                    }
                }
            }
        }

        if(currentStage==2)gDraw(pixels,stride);

        // Red Key
        if (phase == PHASE_CHALLENGE && !doorOpen)
            drawKey(pixels, stride);

        // Porta
        drawRect(
            pixels, stride,
            DOOR_X, DOOR_Y,
            DOOR_W, DOOR_H,
            doorOpen ? GREEN : RED
        );

        // Rastro de velocidade
        drawTrails(pixels, stride);

        // Jogador
        drawRect(
            pixels, stride,
            (int)player.x,
            (int)player.y,
            PLAYER_SIZE,
            PLAYER_SIZE,
            BLACK
        );

        // HUD fixa na tela, sem sobrepor as plataformas.
        if (ui == UI_PLAY) uiHud(pixels,stride,phase,timerRunning,remainingTime,showHelp,now);
        else uiPanel(pixels,stride,ui,menuSelection,showHelp);

        framebufferEnd(&fb);
    }

    framebufferClose(&fb);

    return 0;
}
