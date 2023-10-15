#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libs/raylib.h"

#define SHAPESIZE 5
#define MAX_BULLETS 100
#define LINECOLOR CLITERAL(Color){ 0, 0, 255, 80}
#define ARRAY_LEN(a) sizeof((a))/sizeof((a)[0])


typedef struct {
    int items[SHAPESIZE][SHAPESIZE];
    Vector2 pos;
    bool active;
} Shape;

typedef struct {
    int age;
    Vector2 pos;
    Vector2 speed;
    Color color;
    bool active;
} Bullet;

typedef struct {
    Shape enemies[12];
    Vector2 pos;
    Vector2 speed;
    float flipTime;
    float currentTime;
} EnemyWave;

Shape enemyTemplate = {
    .items = {
        {3,0,3,0,3},
        {3,3,1,3,3},
        {3,2,1,2,3},
        {3,3,3,3,3},
        {0,0,3,0,0}

    }
};

Shape player = {
    .items = {
        {0,0,0,0,0},
        {0,0,2,0,0},
        {2,2,1,2,2},
        {2,2,2,2,2},
        {2,0,2,0,2}
    },
    .pos = {0}
};

EnemyWave wave = {0};
Bullet bullets[MAX_BULLETS];
int score = 0;
float dt;
int screenWidth = 800;
int screenHeight = 600;
float speedX = 1;
float speedY = 1;
float speedFactor = 0.01;
float boxFactor = 0.01;

int cellWidth = 0; //calculated every tick
double gametime = 0;

void StartNewEnemyWave() {
    memset(&wave, 0, sizeof(wave));

    wave.pos.x = screenWidth * 0.25;
    wave.pos.y = 10;
    wave.speed.x = 300*speedFactor;
    wave.speed.y = 0;
    wave.flipTime = 10;
    wave.currentTime = 0;
    for(int i = 0; i < ARRAY_LEN(wave.enemies); i++) {
        Shape* enemy = &wave.enemies[i];
        enemy->active = true;
        memcpy(&enemy->items, &enemyTemplate.items, sizeof(enemy->items));
        enemy->pos.x = (i%4)*SHAPESIZE*cellWidth*2.1;
        enemy->pos.y = (i/4)*SHAPESIZE*cellWidth*2.1;
    }
}

void DrawShape(Shape shape, Vector2 offset) {

    for (int x = 0; x < SHAPESIZE; x++) {
        for (int y = 0; y < SHAPESIZE; y++) {
            Color c = BLANK;
            switch (shape.items[y][x])
            {
                case 1: c = RED; break;
                case 2: c = BLUE; break;
                case 3: c = GREEN; break;
            }
            DrawRectangle(offset.x + shape.pos.x + x*cellWidth, offset.y + shape.pos.y + y*cellWidth, cellWidth, cellWidth, c);
        }
    }
}

Vector2 AddVector2(Vector2 a, Vector2 b) {
    return CLITERAL(Vector2) {
        a.x + b.x,
        a.y + b.y
    };
}

bool SpawnBullet(Vector2 pos, Vector2 speed, Color color) {
    for (int i = 0; i < MAX_BULLETS; i++){
        if (!bullets[i].active) {
            bullets[i].active = true;
            bullets[i].color = color;
            bullets[i].speed = speed;//
            bullets[i].pos = pos;
            //offset to center top of player
            bullets[i].pos.x += 2*cellWidth;
            bullets[i].pos.y += 1*cellWidth;
            return true;
        }
    }
    return false;
}

int main(void) {
    Color gridColor = CLITERAL(Color){ 180, 41, 55, 255 };

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Game 1");
    SetTargetFPS(60);
    screenWidth = GetRenderWidth();
    screenHeight = GetRenderHeight();
    cellWidth = screenWidth * boxFactor;
    player.pos.y = screenHeight - SHAPESIZE*cellWidth;
    player.active = true;

    while(!WindowShouldClose()) {
        dt = GetFrameTime();
        gametime += dt;
        screenWidth = GetRenderWidth();
        screenHeight = GetRenderHeight();
        cellWidth = screenWidth * boxFactor;
        speedX = screenWidth * speedFactor;
        speedY = screenHeight * speedFactor;
        wave.currentTime += dt;

        if (IsKeyPressed(KEY_Q)) {
            StartNewEnemyWave();
        }
        if (IsKeyPressed(KEY_R)) {
            player.pos.x = screenWidth*0.05;
            player.active = true;
            score = 0;
        }

        // Player movement
        if (IsKeyDown(KEY_A)) {
            player.pos.x -= speedX;
            if (player.pos.x < 0) player.pos.x = 0;
        }
        if (IsKeyDown(KEY_D)) {
            player.pos.x += speedX;
            if (player.pos.x  > screenWidth - SHAPESIZE*cellWidth) player.pos.x = screenWidth - SHAPESIZE*cellWidth;
        }
        if (IsKeyDown(KEY_W)) {
            player.pos.y -= speedY;
            if (player.pos.y < 0) player.pos.y = 0;
        }
        if (IsKeyDown(KEY_S)) {
            player.pos.y += speedY;
            if (player.pos.y  > screenHeight - SHAPESIZE*cellWidth) player.pos.y = screenHeight - SHAPESIZE*cellWidth;
        }

        // Bullets
        for (int i = 0; i < MAX_BULLETS; i++){
            if (bullets[i].active) {
                bullets[i].pos.x += bullets[i].speed.x;
                bullets[i].pos.y += bullets[i].speed.y;

                if (bullets[i].speed.y < 0) {
                    //player bullet check with enemies
                    for(int e = 0; e < ARRAY_LEN(wave.enemies); e++) {
                        if (wave.enemies[e].active) {
                            if (CheckCollisionRecs(CLITERAL(Rectangle){
                                        bullets[i].pos.x,
                                        bullets[i].pos.y,
                                        cellWidth,
                                        cellWidth
                                        },
                                        CLITERAL(Rectangle){
                                        wave.pos.x + wave.enemies[e].pos.x,
                                        wave.pos.y + wave.enemies[e].pos.y,
                                        SHAPESIZE*cellWidth,
                                        SHAPESIZE*cellWidth,
                                        })){
                                wave.enemies[e].active = false;
                                bullets[i].active = false;
                                score += 10;
                            }
                        }
                    }

                } else if (bullets[i].speed.y > 0) {
                    //enemy bullets check with player
                    if (CheckCollisionRecs(CLITERAL(Rectangle){
                                bullets[i].pos.x,
                                bullets[i].pos.y,
                                cellWidth,
                                cellWidth
                                },
                                CLITERAL(Rectangle){
                                player.pos.x,
                                player.pos.y,
                                SHAPESIZE*cellWidth,
                                SHAPESIZE*cellWidth,
                                })){
                        player.active = false;
                        bullets[i].active = false;
                    }
                }

                if (bullets[i].pos.y < 0 || bullets[i].pos.y > screenHeight) {
                    bullets[i].active = false;
                }
            }
        }

        bool firedBullet = true;
        if (player.active && IsKeyPressed (KEY_SPACE)) {
            firedBullet = SpawnBullet(player.pos, CLITERAL(Vector2){ 0, -speedY}, BLUE);
        }

        // Enemies
        bool enemiesPresent = false;
        for(int i = 0; i < ARRAY_LEN(wave.enemies); i++) {
            if (wave.enemies[i].active) {
                enemiesPresent = true;
                //fire bullet sometimes
                if (GetRandomValue(0,100) == 8) {
                    SpawnBullet(AddVector2(wave.pos, wave.enemies[i].pos), CLITERAL(Vector2){ 0, speedY}, RED);
                }
            }
        }

        if (enemiesPresent) {
            wave.pos.x += wave.speed.x;
            wave.pos.y += wave.speed.y;
            if (wave.pos.x < screenWidth*0.2 || wave.pos.x > screenWidth*0.4) {
                wave.speed.x = -wave.speed.x;
            }
        }

        // RENDERING
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText(TextFormat("SCORE %3i", score), 20, 20, 40, LIGHTGRAY);

        for(int i = 0; i < ARRAY_LEN(wave.enemies); i++) {
            if (wave.enemies[i].active) {
                DrawShape(wave.enemies[i], wave.pos);
            }
        }

        if (player.active) {
            DrawShape(player, CLITERAL(Vector2){0,0});
            if (!enemiesPresent) {
                DrawText("PRESS 'Q' TO START", 50, 100, 50, GREEN);
            }
        } else {
            DrawText("YOU DIED,\nPRESS 'R' TO RESTART", 50, 100, 50, RED);
        }

        // Draw bullets
        for (int i = 0; i < MAX_BULLETS; i++){
            if (bullets[i].active) {
                DrawRectangle(bullets[i].pos.x,bullets[i].pos.y,cellWidth, cellWidth, bullets[i].color);
            }
        }

        if (!firedBullet) {
            TraceLog(LOG_ERROR, "FAILED TO FIRE BULLET");
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
