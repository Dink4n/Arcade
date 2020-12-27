#include "entity.h"

void
BallInit(Entity* ball, i32 posX, i32 posY)
{
    ball->pos   = (v2){ posX, posY };
    ball->size  = (v2){ 20.0f, 20.0f };
    ball->vel.x = GetRandomValue(0, 2) ? 400 : -400;
    ball->vel.y = GetRandomValue(-200, 200) * 1.5f;
    ball->score = 0;
}

void
BallUpdate(Entity* ball)
{
}

void
BallRender(Entity* ball)
{
}

void
BallReset(Entity* ball)
{
    ball->pos   = (v2){ posX, posY };
    ball->vel.x = GetRandomValue(0, 2) ? 400 : -400;
    ball->vel.y = GetRandomValue(-200, 200) * 1.5f;
}

void
PaddleInit(Entity* paddle)
{
}

void
PaddleUpdate(Entity* paddle)
{
}

void
PaddleRender(Entity* paddle)
{
}
