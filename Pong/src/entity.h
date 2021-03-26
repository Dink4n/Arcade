#ifndef ENTITY_H
#define ENTITY_H

#include "types.h"
#include "constants.h"

typedef struct Entity
{
    v2 pos;
    v2 size;
    v2 vel;
    u16 score;
} Entity;

void BallInit(Entity* ball);
void BallUpdate(Entity* ball, f64 delta);
void BallRender(Entity* ball);
void BallReset(Entity* ball);

void PaddleInit(Entity* paddle, v2 pos, v2 size);
void PaddleUpdate(Entity* paddle, f64 delta);
void PaddleRender(Entity* paddle);

#endif
