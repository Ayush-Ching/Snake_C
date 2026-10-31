#include <raylib.h>

#define GRID_SIZE 25
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_SCORE 256

Vector2 head;
Vector2 dir;
Vector2 food;
Vector2 segments[MAX_SCORE - 1];

int score = 0;
char scoreStr[32];
bool isRunning = true;

const Color bg         = {220, 230, 250, 255};
const Color snakeRed   = {190, 100, 105, 255};
const Color snakeBlack = { 45,  50,  65, 255};
const Color foodColor  = {160, 190, 140, 255};
const Color textDark   = { 75,  85, 105, 255};
const Color textOrange = {210, 135, 110, 255};
const Color textGreen  = {140, 190, 185, 255};
const Color textBlue   = { 90, 130, 170, 255};

void initGame();
void quitGame();
void draw();

int main(int argc, char* argv[]) {
    initGame();

    while(!WindowShouldClose()) {
        draw();
    }

    quitGame();

    return 0;
}

void initGame() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");
}

void quitGame() {
    CloseWindow();
}

void draw() {
    BeginDrawing();

    ClearBackground(bg);

    DrawText("Hello, snake", 240, 270, 60, textGreen);

    EndDrawing();
}
