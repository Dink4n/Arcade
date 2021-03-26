#include <raylib.h>
#include <stdio.h>
#include <math.h>

#include "entity.h"
#include "types.h"
#include "constants.h"

//------------------------------------------------------------------------------
// Types and structure definitions
//------------------------------------------------------------------------------
typedef enum GameState
{
    start,
    play
} GameState;

//------------------------------------------------------------------------------
// Global variable declarations
//------------------------------------------------------------------------------
internal GameState gameState;
internal Entity leftPlayer = { 0 };
internal Entity rightPlayer = { 0 };
internal Entity ball = { 0 };

internal Font font;

//------------------------------------------------------------------------------
// Game Functions declarations
//------------------------------------------------------------------------------
internal void GameInit(void);
internal void GameUpdate(f64 delta);
internal void GameDraw(void);
internal void GameClose(void);

internal void DisplayFPS();

//------------------------------------------------------------------------------
// Main Entry Point
//------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game");
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
    v2 paddleSize = { 20.0f, 100.0f };
    PaddleInit(&leftPlayer, (v2){ 50.0f, 50.0f }, paddleSize);
    PaddleInit(&rightPlayer, (v2){(SCREEN_WIDTH - paddleSize.x) - 50,
            (SCREEN_HEIGHT - paddleSize.y) - 50.0f}, paddleSize);

    // Initialize the ball
    BallInit(&ball);

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
            BallReset(&ball);
        }
    }

    if (gameState == play)
    {
        BallUpdate(&ball, delta);
        PaddleUpdate(&leftPlayer, delta);
        PaddleUpdate(&rightPlayer, delta);
    }

    // Handle player movement
    if (IsKeyDown(KEY_W))
        leftPlayer.vel.y = -PADDLE_SPEED;
    else if (IsKeyDown(KEY_S))
        leftPlayer.vel.y = PADDLE_SPEED;
    else
        leftPlayer.vel.y = 0;

    if (IsKeyDown(KEY_UP))
        rightPlayer.vel.y = -PADDLE_SPEED;
    else if (IsKeyDown(KEY_DOWN))
        rightPlayer.vel.y = PADDLE_SPEED;
    else
        rightPlayer.vel.y = 0;

    // Handle Collision

    if (BallCollides(&ball, &leftPlayer))
    {
        ball.vel.x *= -1.03;
        ball.pos.x = leftPlayer.pos.x + 20.0f;

        // keep velocity going in the same direction, but randomize it
        if (ball.vel.y < 0)
            ball.vel.y = -GetRandomValue(10, 150);
        else
            ball.vel.y = GetRandomValue(10, 150);
    }
    else if (BallCollides(&ball, &rightPlayer))
    {
        ball.vel.x *= -1.03;
        ball.pos.x = rightPlayer.pos.x - 20.0f;

        // keep velocity going in the same direction, but randomize it
        if (ball.vel.y < 0)
            ball.vel.y = -GetRandomValue(10, 150);
        else
            ball.vel.y = GetRandomValue(10, 150);
    }

    // detect upper and lower boundary collision and reverse if colliding
    if (ball.pos.y <= 0)
    {
        ball.pos.y = 0;
        ball.vel.y *= -1;

    }
    else if (ball.pos.y >= SCREEN_HEIGHT - ball.size.y)
    {
        ball.pos.y = SCREEN_HEIGHT - ball.size.y;
        ball.vel.y *= -1;
    }

    // Handle Score
    if (ball.pos.x <= 0 )
    {
        rightPlayer.score++;
        BallReset(&ball);
    }
    else if ((ball.pos.x + ball.size.x) >= SCREEN_WIDTH)
    {
        leftPlayer.score++;
        BallReset(&ball);
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
               (v2){(SCREEN_WIDTH / 2.0f) - titleSize.x / 2.0f, 82.0f}, 32, 1,
               WHITE);

    DisplayFPS();

    // Display the score
    DrawTextEx(
            font, score,
            (v2){((SCREEN_WIDTH / 2.0f) - MeasureTextEx(font, score, 64, 1).x) -
            30.0f,
            SCREEN_HEIGHT / 3.0f},
            64, 1, WHITE);

    score = TextFormat("%d", rightPlayer.score);
    DrawTextEx(font, score,
               (v2){SCREEN_WIDTH / 2.0f + 30.0f, SCREEN_HEIGHT / 3.0f}, 64, 1,
               WHITE);

    // Draw all the game objects
    PaddleRender(&leftPlayer);
    PaddleRender(&rightPlayer);

    BallRender(&ball);
}

internal void
GameClose(void)
{
    UnloadFont(font);
}

internal void
DisplayFPS()
{
    const char* FPS = TextFormat("FPS: %d", GetFPS());
    DrawTextEx(font, FPS, (v2){ 10.0f, 10.0f }, 25, 1, GREEN);
}
