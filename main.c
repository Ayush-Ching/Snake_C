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

char scoreStr[32];
int  score       = 0;
bool isRunning   = true;
bool gameStarted = false;
int  fps         = 10;

const Color bg         = { 20,  30,  50, 255};
const Color wallColor  = {220, 230, 250, 255};
const Color snakeHead  = {225, 165, 120, 255};
const Color snakeLight = {120, 165, 225, 255};
const Color snakeDark  = { 45,  70, 125, 255};
const Color foodColor  = {165, 225, 120, 255};
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
void restartGame();
void startScreen();

int main(int argc, char* argv[]) {
	fps = argc > 1 ? atoi(argv[1]) : 10;

    initGame();

    while(!WindowShouldClose()) {
		processInput();

		if (!gameStarted) {
			startScreen();
		} else if (isRunning) {
			update();
		}

        if(gameStarted) draw();
    }

    quitGame();

    return 0;
}

void initGame() {
	time_t t;
	srand(time(&t));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");

	SetTargetFPS(fps);

	sprintf(scoreStr, "SCORE : %d", score);

	head.x = 1;
	head.y = 13;
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
	if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && (dir.y == 0)) {
		dir.x =  0;
		dir.y = -1;
	}
	else if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && (dir.x == 0)) {
		dir.x = -1;
		dir.y =  0;
	}
	else if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && (dir.y == 0)) {
		dir.x =  0;
		dir.y =  1;
	}
	else if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && (dir.x == 0)) {
		dir.x =  1;
		dir.y =  0;
	}

	if(!isRunning && IsKeyPressed(KEY_SPACE)) {
		restartGame();
	}

	if(IsKeyPressed(KEY_ENTER)) {
		gameStarted = true;
	}

	if (IsKeyPressed(KEY_ESCAPE)) {
		isRunning = false;

		quitGame();
	}
}

void update() {
	for(int i=score; i>0; i--) {
		segments[i] = segments[i - 1];
	}

	segments[0] = head;

	head.x += dir.x;
	head.y += dir.y;

	if(collideSegment(head) || head.x < 1 || head.y < 1 || head.x >= GRID_SIZE - 1 || head.y >= GRID_SIZE - 1) {
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
    CloseWindow();
}

void draw() {
    BeginDrawing();

    ClearBackground(bg);

	for(int i=0; i<GRID_SIZE; i++) {
		for(int j=0; j<GRID_SIZE; j++) {
			if(i == 0 || i == GRID_SIZE - 1 || j == 0 || j == GRID_SIZE - 1) {
				DrawRectangle(i * widthByGrid, j * heightByGrid, widthByGrid, heightByGrid, wallColor);
			}
		}
	}

	DrawRectangle(food.x * widthByGrid, food.y * heightByGrid, widthByGrid, heightByGrid, foodColor);

	for(int i=0; i<score; i++) {
		if(i % 2 == 0){
			DrawRectangle(segments[i].x * widthByGrid, segments[i].y * heightByGrid, widthByGrid, heightByGrid, snakeDark);
		} else {
			DrawRectangle(segments[i].x * widthByGrid, segments[i].y * heightByGrid, widthByGrid, heightByGrid, snakeLight);
		}
	}

	DrawRectangle(head.x * widthByGrid, head.y * heightByGrid, widthByGrid, heightByGrid, snakeHead);

	DrawText(scoreStr, 5, 5, 30, textBlue);

	if(!isRunning) {
		drawGameOver();
	}

    EndDrawing();
}

void drawGameOver() {
	char gameOverMessage[32];
	sprintf(gameOverMessage, " GAME OVER!\nFinal Score : %d", score);
	int mesWidth = MeasureText(gameOverMessage, 80);
	int mesMid = (SCREEN_WIDTH - mesWidth) / 2;
	DrawText(gameOverMessage, mesMid, 300, 80, textOrange);

	char* restartText = "Press Space to Try Again";
	int resWidth = MeasureText(restartText, 30);
	int resMid = (SCREEN_WIDTH - resWidth) / 2;
	DrawText(restartText, resMid, 470, 30, textBlue);

	char* quitText = "Press Escape to Quit";
	int quitWidth = MeasureText(quitText, 30);
	int quitMid = (SCREEN_WIDTH - quitWidth) / 2;
	DrawText(quitText, quitMid, 520, 30, textBlue);
}

void restartGame() {
	head.x    = 1;
	head.y    = 13;
	dir.x     = 1;
	dir.y     = 0;
	score     = 0;
	food      = spawnFood();
	isRunning = true;

	sprintf(scoreStr, "SCORE : %d", score);
}

void startScreen() {
	BeginDrawing();
	
    ClearBackground(bg);

	char* titleText = "___SNAKE___";
	int titleWidth = MeasureText(titleText, 120);
	int titleMid = (SCREEN_WIDTH - titleWidth) / 2;
	DrawText(titleText, titleMid, 120, 120, textGreen);

	char* startText = "Press ENTER to start";
	int startWidth = MeasureText(startText, 50);
	int startMid = (SCREEN_WIDTH - startWidth) / 2;
	DrawText(startText, startMid, 300, 50, textBlue);

	char* quitText = "Press Escape to Quit";
	int quitWidth = MeasureText(quitText, 40);
	int quitMid = (SCREEN_WIDTH - quitWidth) / 2;
	DrawText(quitText, quitMid, 360, 40, textOrange);

	char* helpText = "Use WASD or arrows to move";
	int helpWidth = MeasureText(helpText, 30);
	int helpMid = (SCREEN_WIDTH - helpWidth) / 2;
	DrawText(helpText, helpMid, 420, 30, textBlue);

	EndDrawing();
}
