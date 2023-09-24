#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int numRandom(int min, int max){
    srand(time(NULL));

    int random = rand() % (max - min + 1) + min;

    return random;
}