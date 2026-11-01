#include <stdio.h>
#include <raylib.h>
#include <time.h>
#include <stdlib.h>

#define GRID_SIZE 25
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define MAX_SCORE 256

int widthByGrid  = SCREEN_WIDTH / GRID_SIZE;
int heightByGrid = SCREEN_HEIGHT / GRID_SIZE;

Vector2 head;
Vector2 dir;
Vector2 food;
Vector2 segments[MAX_SCORE - 1];

int score = 0;
char scoreStr[32];
bool isRunning = true;

const Color bg         = { 20,  30,  50, 255};
const Color snakeHead  = {190, 145, 105, 255};
const Color snakeWhite = {190, 195, 205, 255};
const Color snakeGrey  = {145, 150, 165, 255};
const Color foodColor  = {160, 190, 140, 255};
const Color textDark   = { 75,  85, 105, 255};
const Color textOrange = {210, 135, 110, 255};
const Color textGreen  = {150, 200, 195, 255};
const Color textBlue   = { 90, 130, 170, 255};

void initGame();
void quitGame();
void draw();
Vector2 spawnFood();
bool collide(Vector2 a, Vector2 b);
bool collideSegment(Vector2 self);
void processInput();
void update();
void drawGameOver();

int main(int argc, char* argv[]) {
    initGame();

    while(!WindowShouldClose()) {
		processInput();

		if (isRunning) {
			update();
		}

        draw();
    }

	printf("\nquit in main\n\n");

    quitGame();

    return 0;
}

void initGame() {
	time_t t;
	srand(time(&t));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");

	SetTargetFPS(10);

	sprintf(scoreStr, "SCORE : %d", score);

	head.x = 0;
	head.y = 0;
	dir.x  = 1;
	dir.y  = 0;

	food = spawnFood();

	isRunning = true;
}

Vector2 spawnFood() {
	Vector2 snack = {1 + rand() % (GRID_SIZE - 2), 1 + rand() % (GRID_SIZE - 2)};

	while(collide(head, snack) || collideSegment(snack)) {
		snack.x = 1 + rand() % (GRID_SIZE - 2);
		snack.y = 1 + rand() % (GRID_SIZE - 2);
	}

	return snack;
}

bool collide(Vector2 a, Vector2 b) {
	return (a.x == b.x && a.y == b.y);
}

bool collideSegment(Vector2 self) {
	for(int i=0; i < score; i++) {
		if(collide(self, segments[i])) return true;
	}

	return false;
}

void processInput() {
	if (IsKeyPressed(KEY_ESCAPE)) {
		printf("\nescape pressed\n\n");
		isRunning = false;

		quitGame();
	}

	if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
		if(dir.y == 1) return;
		dir.x =  0;
		dir.y = -1;
	}
	else if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
		if(dir.x == 1) return;
		dir.x = -1;
		dir.y =  0;
	}
	else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
		if(dir.y == -1) return;
		dir.x =  0;
		dir.y =  1;
	}
	else if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
		if(dir.x == -1) return;
		dir.x =  1;
		dir.y =  0;
	}
}

void update() {
	for(int i=score; i>0; i--) {
		segments[i] = segments[i - 1];
	}

	segments[0] = head;

	head.x += dir.x;
	head.y += dir.y;

	if(collideSegment(head) || head.x < 0 || head.y < 0 || head.x >= GRID_SIZE || head.y >= GRID_SIZE) {
		isRunning = false;
	}

	if(collide(head, food)) {
		if(score < MAX_SCORE) {
			score++;
			sprintf(scoreStr, "SCORE : %d", score);
			food = spawnFood();
		} else {
			sprintf(scoreStr, "YOU WIN!!!");
		}
	}
}

void quitGame() {
	printf("quitGame called...\n");
    CloseWindow();
}

void draw() {
    BeginDrawing();

    ClearBackground(bg);

	DrawRectangle(food.x * widthByGrid, food.y * heightByGrid, widthByGrid, heightByGrid, foodColor);

	DrawRectangle(head.x * widthByGrid, head.y * heightByGrid, widthByGrid, heightByGrid, snakeHead);

	for(int i=0; i<score; i++) {
		if(i % 2 == 0){
			DrawRectangle(segments[i].x * widthByGrid, segments[i].y * heightByGrid, widthByGrid, heightByGrid, snakeGrey);
		} else {
			DrawRectangle(segments[i].x * widthByGrid, segments[i].y * heightByGrid, widthByGrid, heightByGrid, snakeWhite);
		}
	}

	DrawText(scoreStr, 10, 10, 30, textGreen);

	if(!isRunning) {
		drawGameOver();
	}

    EndDrawing();
}

void drawGameOver() {
	char gameOverMessage[32];
	sprintf(gameOverMessage, "GAME OVER!\nFinal Score : %d", score);
	DrawText(gameOverMessage, 80, 300, 80, textOrange);
}
