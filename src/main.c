#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include <unistd.h>


#define GROUND_Y 20
#define JUMP_HEIGHT 2
#define GRAVITY 1
#define OBSTACLE_SPEED 2
#define FRAME_INTERVAL 100

#define ARQ_WIN "src/files/win.txt"
#define ARQ_DEATH "src/files/death.txt"
#define ARQ_MENU "src/files/menu.txt"
#define MAX_NOME 100
#define ARQ_TOPSCORES "src/files/topscores.txt"
#define ARQ_SCORES "src/files/score.txt"

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

int score;
int gameOver;
int lives;

void initGame () {
    srand(time(NULL));
    player.x = 10;
    player.y = GROUND_Y - 1;
    player.isJumping = 0;
    player.jumpVelocity = 0;

    for (int i = 0; i < 3; i++) {
        obstacles[i].active = 0;
    }
    score = 0;
    gameOver = 0;
    lives = 3;
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


void drawLives() {
    screnSetColor(WHITE, DARKGRAY);
    screenGotoxy("Vidas: %d", lives);
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
            player.y = GROUND_Y - 1;
            player.isJumping = 0;
            player.jumpVelocity = 0;
        } 
    }
}

int showMenu(const char *arquivo) {
    FILE *file = fopen(arquivo, "r");

    if (file == NULL) {
        printf("Nao foi possivel abri o arquivo!\n");
        return;
    }

    char linha[256];

    while (fgets(linha, sizeof(linha), file)) {
        printf("%s", linha);
    }

    fclose(file);

    printf("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");

    printf("\n[1] - Start Game\n");
    printf("\n[2] - Top Scores\n");
    printf("\n[3] - Exit\n");

    printf("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");

    int opcao;

    printf("Escolha uma opcao: ");

    scanf("%d", &opcao);

    getchar();

    return opcao;

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
            obstacles[i].y = GROUND_Y - 1;
            obstacles[i].active = 1;
        }
    }
}

int checkCollision () {
    for (int i = 0; i < 3; i++) {
        if (obstacles[i].active && 
            abs(obstacles[i].x - player.x) <= 1 &&
            abs(obstacles[i].y - player.y) <= 1) {
                return 1;
            }
    }
    return 0;
}

void showAscii (const char *arquivo) {
    FILE *file = fopen(arquivo, "r");

    if (file == NULL) {
        printf("Arquivo nao encontrado!\n");
        return;
    }

    char linha[256];

    while (fgets(linha, sizeof(linha), file)) {
        printf("%s", linha);
    }

    fclose(file);
}

void showTopScores() {
    printf("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
    showAscii(ARQ_TOPSCORES);
    printf("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
    showAscii(ARQ_SCORES);
    printf("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
}

typedef struct No {
    char nome[MAX_NOME];
    int pontos;
    struct No *next;
} No;

No *scoreList = NULL;

void addScore (No **list, const char *nome, int pontos) {
    No *novo = malloc(sizeof(No));
    strcpy(novo->nome, nome);
    novo->pontos = pontos;
    novo->next = *list;
    *list = novo;
}

void saveScore (No *list, const char *arquivo) {
    FILE *file = fopen(arquivo, "a");

    while (list) {
        fprintf("%s - %d pontos\n", list->nome, list->pontos);
        list = list -> next;
    }
    fclose(file);
}

int main () {
    char jogador[MAX_NOME];

    printf("\nDigite o nome: ");

    fgets(jogador, MAX_NOME, stdin);

    



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
            }
            drawScore();
            screenUpdate();

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





