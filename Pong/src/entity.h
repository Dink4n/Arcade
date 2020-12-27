#ifndef ENTITY_H
#define ENTITY_H

#include "types.h"

typedef struct Entity
{
    v2 pos;
    v2 size;
    v2 vel;
    u16 score;
} Entity;

void BallInit(Entity* ball, i32 posX, i32 posY);
void BallUpdate(Entity* ball);
void BallRender(Entity* ball);
void BallReset(Entity* ball);

void PaddleInit(Entity* paddle);
void PaddleUpdate(Entity* paddle);
void PaddleRender(Entity* paddle);

#endif
