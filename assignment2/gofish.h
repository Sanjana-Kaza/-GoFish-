#ifndef GOFISH_H
#define GOFISH_H
#include <stdlib.h>
#include "player.h"
#include "deck.h"

struct player *current_player;
struct player *next_player;

void gamestart();
int gameend();
int gameplay();
void printhand(struct player *target);
const char *printrank(char rank);
void printbook(struct player *target);

#endif