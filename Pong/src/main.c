#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include "./utils.h"

//------------------------------------------------------------------------------
// Types and structure definitions
//------------------------------------------------------------------------------
typedef enum GameState
{
    start,
    play
} GameState;

typedef struct Paddle
{
    v2 pos;
    v2 size;
    f32 vel;
    i32 score;
} Paddle;

typedef struct Ball
{
    v2 pos;
    v2 size;
    v2 vel;
} Ball;

//------------------------------------------------------------------------------
// Global variable declarations
//------------------------------------------------------------------------------
internal const int g_ScreenWidth = 1270;
internal const int g_ScreenHeight = 720;

internal GameState gameState;
internal Paddle leftPlayer = { 0 };
internal Paddle rightPlayer = { 0 };
internal Ball ball = { 0 };

internal Font font;

//------------------------------------------------------------------------------
// Game Functions declarations
//------------------------------------------------------------------------------
internal void GameInit(void);
internal void GameUpdate(f64 delta);
internal void GameDraw(void);
internal void GameClose(void);

internal b8 IsColliding(Paddle* paddle, Ball* ball);

//------------------------------------------------------------------------------
// Main Entry Point
//------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //------------------------------------------------------------------------------
    InitWindow(g_ScreenWidth, g_ScreenHeight, "Game");
    GameInit();
    SetTargetFPS(60);

    //------------------------------------------------------------------------------
    // Game Loop
    //------------------------------------------------------------------------------
    while (!WindowShouldClose())
    {
        // Update
        //------------------------------------------------------------------------------
        f64 deltaTime = GetFrameTime();
        GameUpdate(deltaTime);
        //------------------------------------------------------------------------------

        // Draw
        //------------------------------------------------------------------------------
        BeginDrawing();

        GameDraw();

        EndDrawing();
        //------------------------------------------------------------------------------
    }

    // De-Initialization
    //------------------------------------------------------------------------------
    GameClose();
    CloseWindow();
    //------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------
// Game Functions Definitions
//------------------------------------------------------------------------------

// Initialize game objects
internal void
GameInit(void)
{
    // Initialize the players
    leftPlayer.pos.x = 50;
    leftPlayer.pos.y = 34.0f;
    leftPlayer.size = (v2){ 20.0f, 100.0f };
    leftPlayer.vel = 500.0f;
    leftPlayer.score = 0;

    rightPlayer.pos.x = (g_ScreenWidth - rightPlayer.size.x) - 50;
    rightPlayer.pos.y = (g_ScreenHeight - 100.0f) - 34.0f;
    rightPlayer.size = (v2){ 20.0f, 100.0f };
    rightPlayer.vel = 500.0f;
    leftPlayer.score = 0;

    // Initialize the ball
    ball.size = (v2){ 20.0f, 20.0f };
    ball.pos = (v2){
        (g_ScreenWidth/2.0f)  - ball.size.x/2.0f,
        (g_ScreenHeight/2.0f) - ball.size.y/2.0f
    };

    ball.vel.x = GetRandomValue(0, 2) ? 400.0f : -400.0f;
    ball.vel.y = GetRandomValue(-100.0f, 100.0f);

    // Initialize the fonts
    font = LoadFont("assets/font.ttf");
}

internal void
GameUpdate(f64 delta)
{
    // Handle game states
    if (IsKeyPressed(KEY_ENTER))
    {
        if (gameState == start)
        {
            gameState = play;
        }
        else
        {
            gameState = start;

            // Reset the ball
            ball.size = (v2){ 20.0f, 20.0f };
            ball.pos = (v2){
                (g_ScreenWidth/2.0f) - ball.size.x,
                (g_ScreenHeight/2.0f) - ball.size.y};

            ball.vel.x = GetRandomValue(0, 2) ? 400.0f : -400.0f;
            ball.vel.y = GetRandomValue(-100.0f, 100.0f) * 1.5f;
        }
    }

    if (gameState == play)
    {
        ball.pos.x += ball.vel.x * delta;
        ball.pos.y += ball.vel.y * delta;
    }

    // Handle player movement
    if (IsKeyDown(KEY_W))
        leftPlayer.pos.y = fmax(
                    0,
                    leftPlayer.pos.y + -leftPlayer.vel * delta
                );
    else if (IsKeyDown(KEY_S))
        leftPlayer.pos.y = fmin(
                    g_ScreenHeight - leftPlayer.size.y,
                    leftPlayer.pos.y + leftPlayer.vel * delta
                );

    if (IsKeyDown(KEY_UP))
        rightPlayer.pos.y = fmax(
                    0,
                    rightPlayer.pos.y + -rightPlayer.vel * delta
                );
    else if (IsKeyDown(KEY_DOWN))
        rightPlayer.pos.y = fmin(
                    g_ScreenHeight - rightPlayer.size.y,
                    rightPlayer.pos.y + rightPlayer.vel * delta
                );

    // Handle Collision
    if (ball.pos.y <= 0 || (ball.pos.y + ball.size.y) >= g_ScreenHeight)
        ball.vel.y *= -1;

    if (IsColliding(&leftPlayer, &ball))
        ball.vel.x *= -1;
    else if (IsColliding(&rightPlayer, &ball))
        ball.vel.x *= -1;

    // Handle Score
    if (ball.pos.x <=0 )
    {
        rightPlayer.score++;
        gameState = start;
    }
    else if ((ball.pos.x + ball.size.x) >= g_ScreenWidth)
    {
        leftPlayer.score++;
    }
}

internal void
GameDraw(void)
{
    // Variables
    Color backgroundColor = { 40, 45, 52, 255 };
    const char* score = TextFormat("%d", leftPlayer.score);
    v2 titleSize = MeasureTextEx(font, "Bong!", 32, 1);

    // Clear the screen and display the title
    ClearBackground(backgroundColor);
    DrawTextEx(font, "Bong!",
               (v2){(g_ScreenWidth / 2.0f) - titleSize.x / 2.0f, 82.0f}, 32, 1,
               WHITE);

    // Display the score
    DrawTextEx(
        font, score,
        (v2){((g_ScreenWidth / 2.0f) - MeasureTextEx(font, score, 64, 1).x) -
                 30.0f,
             g_ScreenHeight / 3.0f},
        64, 1, WHITE);

    score = TextFormat("%d", rightPlayer.score);
    DrawTextEx(font, score,
               (v2){g_ScreenWidth / 2.0f + 30.0f, g_ScreenHeight / 3.0f}, 64, 1,
               WHITE);

    // Draw all the game objects
    DrawRectangle(leftPlayer.pos.x, leftPlayer.pos.y,
            leftPlayer.size.x, leftPlayer.size.y, WHITE);
    DrawRectangle(rightPlayer.pos.x, rightPlayer.pos.y,
            rightPlayer.size.x, rightPlayer.size.y, WHITE);

    DrawRectangleV(ball.pos, ball.size, WHITE);
}

internal void
GameClose(void)
{
    UnloadFont(font);
}

internal b8
IsColliding(Paddle* paddle, Ball* ball)
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
