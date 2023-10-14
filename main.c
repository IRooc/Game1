#include <stdio.h>
#include "libs/raylib.h"

#define SHAPESIZE 5
#define MAX_BULLETS 10
#define LINECOLOR CLITERAL(Color){ 0, 0, 255, 80}

static float dt;
int screenWidth = 800;
int screenHeight = 600;
float speedX = 1;
float speedY = 1;
float speedFactor = 0.01;
float boxFactor = 0.01;

int cellWidth = 0; //calculated every tick

typedef struct {
    int items[SHAPESIZE][SHAPESIZE];
    Vector2 pos;
} Shape;

typedef struct {
    int age;
    Vector2 pos;
    Vector2 speed;
    Color color;
    bool active;
} Bullet;

Shape s1 = {
    .items = {
        {0,0,0,0,0},
        {0,0,3,0,0},
        {2,2,1,2,2},
        {2,2,2,2,2},
        {2,0,2,0,2}
    },
    .pos = {0}
};
Bullet bullets[MAX_BULLETS];

void DrawPlayer() {

    for (int x = 0; x < SHAPESIZE; x++) {
        for (int y = 0; y < SHAPESIZE; y++) {
            Color c = BLACK;
            switch (s1.items[y][x])
            {
                case 1: c = RED; break;
                case 2: c = BLUE; break;
                case 3: c = GREEN; break;
            }
            DrawRectangle(s1.pos.x+ x*cellWidth, s1.pos.y + y*cellWidth, cellWidth, cellWidth, c);

        }
    }
}
double gametime = 0;

int main(void) {
    Color gridColor = CLITERAL(Color){ 180, 41, 55, 255 };

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Game 1");
    SetTargetFPS(60);
    screenWidth = GetRenderWidth();
    screenHeight = GetRenderHeight();
    cellWidth = screenWidth * boxFactor;
    s1.pos.y = screenHeight - SHAPESIZE*cellWidth;

    while(!WindowShouldClose()) {
        dt = GetFrameTime();
        gametime += dt;
        screenWidth = GetRenderWidth();
        screenHeight = GetRenderHeight();
        cellWidth = screenWidth * boxFactor;
        speedX = screenWidth * speedFactor;
        speedY = screenHeight * speedFactor;

        if (IsKeyDown(KEY_A)) {
            s1.pos.x -= speedX;
            if (s1.pos.x < 0) s1.pos.x = 0;
        }
        if (IsKeyDown(KEY_D)) {
            s1.pos.x += speedX;
            if (s1.pos.x  > screenWidth - SHAPESIZE*cellWidth) s1.pos.x = screenWidth - SHAPESIZE*cellWidth;
        }
        if (IsKeyDown(KEY_W)) {
            s1.pos.y -= speedY;
            if (s1.pos.y < 0) s1.pos.y = 0;
        }
        if (IsKeyDown(KEY_S)) {
            s1.pos.y += speedY;
            if (s1.pos.y  > screenHeight - SHAPESIZE*cellWidth) s1.pos.y = screenHeight - SHAPESIZE*cellWidth;
        }

        for (int i = 0; i < MAX_BULLETS; i++){
            if (bullets[i].active) {
                bullets[i].pos.x += bullets[i].speed.x;
                bullets[i].pos.y += bullets[i].speed.y;
                if (bullets[i].pos.y < 0) {
                    bullets[i].active = false;
                }
            }
        }
        if (IsKeyPressed (KEY_SPACE)) {
            for (int i = 0; i < MAX_BULLETS; i++){
                if (!bullets[i].active) {
                    bullets[i].age = 0;
                    bullets[i].active = true;
                    bullets[i].color = GREEN;
                    bullets[i].speed = CLITERAL(Vector2){ 0, -speedY};
                    bullets[i].pos = s1.pos;
                    bullets[i].pos.x += 2*cellWidth;
                    bullets[i].pos.y += 1*cellWidth;
                    break;
                }
            }
        }


        BeginDrawing();
        ClearBackground(BLACK);

        DrawPlayer();

        for (int i = 0; i < MAX_BULLETS; i++){
            if (bullets[i].active) {
                DrawRectangle(bullets[i].pos.x,bullets[i].pos.y,cellWidth, cellWidth, bullets[i].color);
            }
        }

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
