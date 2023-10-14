#include <stdio.h>
#include "libs/raylib.h"

#define SPEED 10
#define SHAPESIZE 5
#define NROFCELLSX 40
#define LINECOLOR CLITERAL(Color){ 0, 0, 255, 80}

static float dt;
int screenWidth = 800;
int screenHeight = 600;

int cellWidth = 0; //calculated every tick

typedef struct {
    int items[SHAPESIZE][SHAPESIZE];
    Vector2 gridPos;
    Vector2 pos;
} Shape;

Shape s1 = {
    .items = {
        {0,0,0,0,0},
        {0,0,3,0,0},
        {2,2,1,2,2},
        {2,2,2,2,2},
        {2,0,2,0,2}
    },
    .gridPos = { .x = 10, .y = 25 },
    .pos = {0}
};

void DrawPlayer() {
    //update gridpos from pos
    s1.gridPos.x = (int)(s1.pos.x / cellWidth);

    for (int x = 0; x < SHAPESIZE; x++) {
        for (int y = 0; y < SHAPESIZE; y++) {
            Color c = BLACK;
            switch (s1.items[y][x])
            {
                case 1: c = RED; break;
                case 2: c = BLUE; break;
                case 3: c = GREEN; break;
            }
            DrawRectangle(s1.pos.x+ x*cellWidth, s1.gridPos.y*cellWidth + y*cellWidth, cellWidth, cellWidth, c);

        }
    }
}
double gametime = 0;

int main(void) {
    Color gridColor = CLITERAL(Color){ 180, 41, 55, 255 };

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Game 1");
    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        dt = GetFrameTime();
        gametime += dt;
        screenWidth = GetRenderWidth();
        screenHeight = GetRenderHeight();
        cellWidth = screenWidth / NROFCELLSX;
        s1.gridPos.y = (screenHeight / cellWidth) - SHAPESIZE;

        if (IsKeyDown(KEY_A)) {
            s1.pos.x -= SPEED;
            if (s1.pos.x < 0) s1.pos.x = 0;
        }
        if (IsKeyDown(KEY_D)) {
            s1.pos.x += SPEED;
            if (s1.pos.x  > screenWidth - SHAPESIZE*cellWidth) s1.pos.x = screenWidth - SHAPESIZE*cellWidth;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawPlayer();

        for (int i = 0; i < screenWidth+cellWidth/cellWidth; i++) {
            DrawLine(i*cellWidth, 0, i*cellWidth, screenHeight, LINECOLOR);
        }
        for (int i = 0; i < screenHeight+cellWidth/cellWidth; i++) {
            DrawLine(0, i*cellWidth, screenWidth, i*cellWidth, LINECOLOR);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
