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
#define FRAME_INTERVAL 100

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
    screenSetColor(GREEN, DARKGRAY);
    for (int x = MINX; x <= MAXX; x++) {
        screenGotoxy(x, GROUND_Y + 1);
        printf("_");
    }
}

void updatePlayer () {
    if (player.isJumping) {
        player.y -= player.jumpVelocity;
        player.jumpVelocity -= GRAVITY;

        if(player.y >= GROUND_Y) {
            player.y = GROUND_Y;
            player.isJumping = 0;
            player.jumpVelocity = 0;
        } 
    }
}

void updateObstacles () {
    for (int i = 0; i < 3; i++) {
        if(obstacles[i].active) {
            obstacles[i].x -= OBSTACLE_SPEED;

            if(obstacles[i].x < MINX) {
                obstacles[i].active = 0;
                score++;
            } 
        } else if (rand() % 100 < 5) {
            obstacles[i].x = MAXX - 1;
            obstacles[i].y = GROUND_Y;
            obstacles[i].active = 1;
        }
    }
}

int checkCollision () {
    for (int i = 0; i < 3; i++) {
        if (obstacles[i].active && obstacles[i].x == player.x && obstacles[i].y == player.y) {
            return 1;
        }
    }
    return 0;
}

int main () {
    screenInit(1);
    keyboardInit();
    timerInit(FRAME_INTERVAL);
    initGame();

    while (!gameOver) {
        if (keyhit()) {
            char ch = readch();
            if (ch == ' ' && !player.isJumping) {
                player.isJumping = 1;
                player.jumpVelocity = JUMP_HEIGHT;
            } else if (ch == 27) {
                break;
            } 
        }

        if (timerTimeOver()) {
            screenClear();
            updatePlayer();
            updateObstacles();
            drawGround();
            drawPlayer();

            for (int i = 0; i < 3; i++) {
                drawObstacle(&obstacles[i]);
                drawScore();
                screenUpdate();
            }

            if (checkCollision()) {
                gameOver = 1;
            }
        }
    }

    screenSetColor(RED, DARKGRAY);
    screenGotoxy(MAXX/2 - 5, GROUND_Y/2);
    printf("Game Over");
    screenUpdate();
    timerDestroy();
    keyboardDestroy();
    screenDestroy();

    return 0;
}





