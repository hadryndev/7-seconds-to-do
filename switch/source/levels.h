#ifndef LEVELS_H
#define LEVELS_H

#define TILE_SIZE 16
#define LEVEL_COLS 40
#define LEVEL_ROWS 20

typedef struct {
    int x;
    int y;
    int w;
    int h;
} LevelRect;

void levelInit(void);
void levelSetStage(int stage);
int levelGetCols(void);
int levelIsSolid(int col, int row);
int levelCollides(float x, float y, float w, float h);

#endif
