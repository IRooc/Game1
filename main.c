#include <stdio.h>
#include "libs/raylib.h"

#define NROFCELLS  30

static float dt;
int screenWidth = 800;
int screenHeight = 600;
int cellWidth = 40;
int halfCellWidth = 10;
int gridOffset = 20;

Vector2 mpos = {0};

typedef struct {
    int fields[NROFCELLS][NROFCELLS];
} Board;

Board board = {0};

typedef struct {
    int items[4][4];
} Shape;

Shape s1 = {
    .items = {
        {0,0,0,0},
        {0,0,0,2},
        {2,2,1,2},
        {0,0,0,0}
    }
};

Shape RotateShape(Shape s) {
    int n = 4;
    // Transpose the s.items
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            int temp = s.items[i][j];
            s.items[i][j] = s.items[j][i];
            s.items[j][i] = temp;
        }
    }
    // Reverse the columns
    for (int i = 0; i < n; i++) {
        for (int j = 0, k = n - 1; j < k; j++, k--) {
            int temp = s.items[j][i];
            s.items[j][i] = s.items[k][i];
            s.items[k][i] = temp;
        }
    }
    return s;
}

void DrawShape(Shape s, int gridX, int gridY, bool picked) {
    int offsetX = 0;
    int offsetY = 0;
    for(int x = 0; x < 4; x++) {
        for(int y = 0; y < 4; y++) {
            if (s.items[y][x] == 1) {
                offsetX = x;
                offsetY = y;
                break;
            }
        }
        if (offsetX || offsetY) break;
    }

	Color cellColor = CLITERAL(Color){255,200,100,255};
    if (picked) {
        TraceLog(LOG_INFO, "picked");
    }

    for(int x = 0; x < 4; x++) {
        for(int y = 0; y < 4; y++) {
            int xpos = gridX*cellWidth + x*cellWidth - offsetX*cellWidth + gridOffset;
            int ypos = gridY*cellWidth + y*cellWidth - offsetY*cellWidth + gridOffset;
            if (s.items[y][x]) {
                if (picked && (gridX + x - offsetX < NROFCELLS) && (gridY + y - offsetY < NROFCELLS)) {
                    board.fields[gridX + x - offsetX][gridY + y - offsetY] = 1;
                }
                DrawRectangle(xpos, ypos, cellWidth, cellWidth, cellColor);
            }
        }
    }
}

int main(void) {
    Color gridColor = CLITERAL(Color){ 180, 41, 55, 255 };

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Game 1");
    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        dt = GetFrameTime();
        screenWidth = GetRenderWidth();
        screenHeight = GetRenderHeight();
        cellWidth = screenWidth / 40;
        mpos = GetMousePosition();

        if (IsKeyReleased(KEY_R)) {
            s1 = RotateShape(s1);
        }

        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawText("Game1", 200, 10, 10, WHITE);
        for(int x = 0; x < NROFCELLS; x++) {
            for(int y = 0; y < NROFCELLS; y ++) {
                int xpos = x*cellWidth + gridOffset;
                int ypos = y*cellWidth + gridOffset;

				if (CheckCollisionPointRec(mpos, CLITERAL(Rectangle){xpos, ypos, cellWidth, cellWidth})) {
                    DrawShape(s1, x, y, IsMouseButtonReleased(MOUSE_LEFT_BUTTON));
                    break;
				}
            }
        }
        for(int x = 0; x < NROFCELLS; x++) {
            for(int y = 0; y < NROFCELLS; y ++) {
                int xpos = x*cellWidth + gridOffset;
                int ypos = y*cellWidth + gridOffset;

                switch (board.fields[x][y]) {
                    case 1:
        	            DrawRectangle(xpos, ypos, cellWidth, cellWidth, gridColor);
                        break;

                    default:
        	            DrawRectangleLines(xpos, ypos, cellWidth, cellWidth, gridColor);
                        break;
                }


            }
        }
        //DrawShape(s1, mpos);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
