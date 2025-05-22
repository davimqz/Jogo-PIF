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
    screenSetColor(WHITE, DARKGRAY);
    screenGotoxy(23, 13);
    printf("Vidas: %d", lives);
}

void drawGround () {
    screenSetColor(GREEN, DARKGRAY);
    for (int x = MINX; x <= MAXX; x++) {
        screenGotoxy(x, GROUND_Y - 1);
        printf("_");
    }
}

typedef struct No {
    char nome[MAX_NOME];
    int pontos;
    struct No *next;
} No;

No *scoreList = NULL;

No *readScores(const char *arquivo) {
    FILE *file = fopen(arquivo, "r");
    if (!file) {
       return NULL; 
    } 

    No *head = NULL;
    char nome[MAX_NOME];
    int pontos;

    while (fscanf(file, " %[^-] - %d pontos\n", nome, &pontos) == 2) {
        No *novo = malloc(sizeof(No));
        strcpy(novo->nome, nome);
        novo->pontos = pontos;
        novo->next = head;
        head = novo;
    }

    fclose(file);
    return head;
}

void addPlayerScore(No **head, const char *nome, int pontos) {
    No *novo = malloc(sizeof(No));
    strcpy(novo->nome, nome);
    novo->pontos = pontos;
    novo->next = *head;
    *head = novo;
}

void bubbleSort(No **head) {
    if (!head || !*head){
        return;
    }

    int trocou;
    do {
        trocou = 0;
        No **index = head;
        while ((*index)->next) {
            No *primeiro = *index;
            No *segundo = primeiro->next;
            if (primeiro->pontos < segundo->pontos) {
                primeiro->next = segundo->next;
                segundo->next = primeiro;
                *index = segundo;
                trocou = 1;
            }
            index = &(*index)->next;
        }
    } while (trocou);
}

void saveListIntoFile(No *head, const char *arquivo) {
    FILE *file = fopen(arquivo, "w");
    if (!file){
        return;
    }

    while (head) {
        fprintf(file, "%s - %d pontos\n", head->nome, head->pontos);
        head = head->next;
    }

    fclose(file);
}

void saveNewScore(const char *arquivo, const char *nomeJogador, int pontos) {
    No *lista = readScores(arquivo);
    addPlayerScore(&lista, nomeJogador, pontos);
    bubbleSort(&lista);
    saveListIntoFile(lista, arquivo);

    while (lista) {
        No *temp = lista;
        lista = lista->next;
        free(temp);
    }
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

int showMenu(const char *arquivo) {
    FILE *file = fopen(arquivo, "r");

    if (file == NULL) {
        printf("Nao foi possivel abri o arquivo!\n");
        return 1;
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

void showTopScores() {
    printf("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
    showAscii(ARQ_TOPSCORES);
    printf("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
}

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

int main () {
    char jogador[MAX_NOME];
    int opcao;

    do {
        opcao = showMenu(ARQ_MENU);
        if (opcao == 2) {
            showTopScores();
            printf("\nPressione ENTER para voltar ao menu.");
            getchar();
        }
    } while (opcao != 1 && opcao != 3);

    if (opcao == 3) {
        return 0;
    }

    printf("Digite o seu nome: ");
    fgets(jogador, MAX_NOME, stdin);
    jogador[strcspn(jogador, "\n")] = 0;

    screenInit(1);
    keyboardInit();
    timerInit(FRAME_INTERVAL);
    initGame();

    while (lives > 0) {
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
            drawLives();
            for (int i = 0; i < 3; i++) {
                drawObstacle(&obstacles[i]);
            }
            drawScore();
            screenUpdate();

            if (checkCollision()) {
                lives--;
                screenUpdate();
                drawScore();
            }
        }
    }

    screenClear();
    showAscii(ARQ_DEATH);
    screenGotoxy(MAXX/2 - 5, GROUND_Y/2);
    printf("FIM DE JOGO - SCORE: %d", score);
    screenUpdate();

    saveNewScore(ARQ_TOPSCORES, jogador, score);

    sleep(3);
    timerDestroy();
    keyboardDestroy();
    screenDestroy();
    return 0;
}





