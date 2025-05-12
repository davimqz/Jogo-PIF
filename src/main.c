#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define GROUND_Y 20
#define JUMP_HEIGHT 5
#define GRAVITY 1
#define OBSTACLE_SPEED 2

typedef struct {
    int x, y;
    int isJumping;
    int jumpVelocity;
} Player;

typedef struct {
    int x, y;
    int active;
} Obstacle;

Player player = {10, GROUND_Y, 0, 0};
Obstacle obstacles[3] = {0};

int score = 0;
int gameOver = 0;

void initGame () {
    srand(time(NULL));
    player.x = 10;
    player.y = GROUND_Y;
    player.isJumping = 0;
    player.jumpVelocity = 0;

    for (int i = 0; i < 3; i++) {
        obstacles[i].active = 0;
    }
    score = 0;
    gameOver = 0;
}

void drawPlayer () {
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(player.x, player.y);
    printf("🦖");
}

void drawObstacle (Obstacle *obstacle) {
    if (obstacle->active) {
        screenSetColor(RED, DARKGRAY);
        screenGotoxy(obstacle->x, obstacle->y);
        printf("🌵");
    }
}

void drawScore () {
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(MAXX - 20, MINY + 1);
    printf("Score: %d", score);

}

void drawGround () {
    screnSetColor(GREEN, DARKGRAY);
    for (int x = MINX; x <= MAXX; x++) {
        screenGotoxy(x, GROUND_Y + 1);
        printf("_");
    }
}


int main () {
    screenInit(1);
    initGame();
}





