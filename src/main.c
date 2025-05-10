#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

typedef struct {
    int x, y;
    int isJumping;
    int jumpVelocity;
} Player;

typedef struct {
    int x, y;
    int acitive;
} Obstacle;

