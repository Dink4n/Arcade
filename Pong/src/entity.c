#include "entity.h"

void
BallInit(Entity* ball)
{
    ball->size  = (v2){ 20.0f, 20.0f };
    ball->pos   = (v2){
        (SCREEN_WIDTH/2.0f)  - ball->size.x/2.0f,
        (SCREEN_HEIGHT/2.0f) - ball->size.y/2.0f
    };
    ball->vel.x = GetRandomValue(0, 2) ? 400 : -400;
    ball->vel.y = GetRandomValue(-200, 200) * 1.5f;
    ball->score = 0;
}

bool
BallCollides(Entity* ball, Entity* paddle)
{
    f32 ballTop = ball->pos.y;
    f32 ballBot = ball->pos.y + ball->size.y;
    f32 ballLeft = ball->pos.x;
    f32 ballRight = ball->pos.x + ball->size.x;

    f32 paddleTop = paddle->pos.y;
    f32 paddleBot = paddle->pos.y + paddle->size.y;
    f32 paddleLeft = paddle->pos.x;
    f32 paddleRight = paddle->pos.x + paddle->size.x;

    if (ballLeft >= paddleRight)
        return false;
    if (ballRight <= paddleLeft)
        return false;
    if (ballTop >= paddleBot)
        return false;
    if (ballBot <= paddleTop)
        return false;

    return true;
}

void
BallUpdate(Entity* ball, f64 delta)
{
    ball->pos.x += ball->vel.x * delta;
    ball->pos.y += ball->vel.y * delta;
}

void
BallRender(Entity* ball)
{
    DrawRectangleV(ball->pos, ball->size, WHITE);
}

void
BallReset(Entity* ball)
{
    ball->pos   = (v2){
        (SCREEN_WIDTH/2.0f)  - ball->size.x/2.0f,
        (SCREEN_HEIGHT/2.0f) - ball->size.y/2.0f
    };
    ball->vel.x = GetRandomValue(0, 2) ? 400 : -400;
    ball->vel.y = GetRandomValue(-200, 200) * 1.5f;
}

void
PaddleInit(Entity* paddle, v2 pos, v2 size)
{
    paddle->size  = size;
    paddle->pos   = pos;
    paddle->vel.y = 0.0f;
    paddle->score = 0;
}

void
PaddleUpdate(Entity* paddle, f64 delta)
{
    if (paddle->vel.y < 0)
        paddle->pos.y = fmax(
                0,
                paddle->pos.y + paddle->vel.y * delta);
    else
        paddle->pos.y = fmin(
                SCREEN_HEIGHT - paddle->size.y,
                paddle->pos.y + paddle->vel.y * delta);
}

void
PaddleRender(Entity* paddle)
{
    DrawRectangleV(paddle->pos, paddle->size, WHITE);
}
