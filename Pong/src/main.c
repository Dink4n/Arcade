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
    STATE_START,
    STATE_PLAY,
    STATE_SERVE,
    STATE_DONE
} GameState;

//------------------------------------------------------------------------------
// Global variable declarations
//------------------------------------------------------------------------------


internal Entity leftPlayer = { 0 };
internal Entity rightPlayer = { 0 };
internal Entity ball = { 0 };

internal f32 smallSize;
internal f32 scoreSize;
internal f32 largeSize;
internal i8 servingPlayer;
internal i8 winningPlayer;

internal Font font;
internal GameState gameState;

//------------------------------------------------------------------------------
// Game Functions declarations
//------------------------------------------------------------------------------
internal void GameInit(void);
internal void GameHandleEvents(void);
internal void GameUpdate(f64 delta);
internal void GameDraw(void);
internal void GameClose(void);

internal void DisplayFPS(void);
internal void DisplayScore(void);
internal void DrawCenteredText(const char* text, f32 pos, f32 fontSize);

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

    Camera2D camera = { 0 };
    camera.target = (v2){ 0.0f, 0.0f };
    camera.offset = (v2){ 0.0f, 0.0f };
    camera.rotation = 0.0f;
    camera.zoom = (float)SCREEN_WIDTH / VIRTUAL_WIDTH;

    //------------------------------------------------------------------------------
    // Game Loop
    //------------------------------------------------------------------------------
    while (!WindowShouldClose())
    {
        // Update
        //------------------------------------------------------------------------------
        f64 deltaTime = GetFrameTime();
        GameHandleEvents();
        GameUpdate(deltaTime);
        //------------------------------------------------------------------------------

        // Draw
        //------------------------------------------------------------------------------
        BeginDrawing();
            BeginMode2D(camera);
                GameDraw();
            EndMode2D();
        EndDrawing();
        //------------------------------------------------------------------------------
    }

    // De-Initialization
    //------------------------------------------------------------------------------
    GameClose();
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
    v2 paddleSize = { 5.0f, 20.0f };

    servingPlayer = 1;
    PaddleInit(&leftPlayer, (v2){ 10.0f, 30.0f }, paddleSize);
    PaddleInit(&rightPlayer,
            (v2){ (VIRTUAL_WIDTH - paddleSize.x) - 10.0f,
            (VIRTUAL_HEIGHT - paddleSize.y) - 30.0f }, paddleSize);

    // Initialize the ball
    BallInit(&ball);

    // Initialize the fonts
    font = LoadFont("assets/font.ttf");
    smallSize = 8.0f;
    largeSize = 16.0f;
    scoreSize = 32.0f;
}

internal void
GameHandleEvents(void)
{
    if (IsKeyPressed(KEY_SPACE))
        ToggleFullscreen();

    // Handle game states
    if (IsKeyPressed(KEY_ENTER))
    {
        if (gameState == STATE_START)
            gameState = STATE_SERVE;
        else if (gameState == STATE_SERVE)
            gameState = STATE_PLAY;
        else if (gameState == STATE_DONE)
        {
            gameState = STATE_SERVE;

            BallReset(&ball);

            // Reset scores to 0
            leftPlayer.score = 0;
            rightPlayer.score = 0;

            // decide serving player as the opposite to who won
            if (winningPlayer == 1)
                servingPlayer = 2;
            else
                servingPlayer = 1;
        }
    }
}

internal void
GameUpdate(f64 delta)
{
    // State handling
    if (gameState == STATE_SERVE)
    {
        ball.vel.y = GetRandomValue(-50, 50);
        if (servingPlayer == 1)
            ball.vel.x = GetRandomValue(140, 200);
        else
            ball.vel.x = -GetRandomValue(140, 200);
    }

    else if (gameState == STATE_PLAY)
    {
        // Handle Collision
        if (BallCollides(&ball, &leftPlayer))
        {
            ball.vel.x *= -1.03;
            ball.pos.x = leftPlayer.pos.x + 5;

            // keep velocity going in the same direction, but randomize it
            if (ball.vel.y < 0)
                ball.vel.y = -GetRandomValue(10, 150);
            else
                ball.vel.y = GetRandomValue(10, 150);
        }
        else if (BallCollides(&ball, &rightPlayer))
        {
            ball.vel.x *= -1.03;
            ball.pos.x = rightPlayer.pos.x - 5;

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
        else if (ball.pos.y >= VIRTUAL_HEIGHT - ball.size.y)
        {
            ball.pos.y = VIRTUAL_HEIGHT - ball.size.y;
            ball.vel.y *= -1;
        }

        // Handle Score
        if (ball.pos.x < 0 )
        {
            // Player 1 is the Left player
            servingPlayer = 1;
            rightPlayer.score++;

            if (rightPlayer.score == 10)
            {
                // player 2 is the Right player
                winningPlayer = 2;
                gameState = STATE_DONE;
            }
            else
            {
                BallReset(&ball);
                gameState = STATE_SERVE;
            }
        }
        else if (ball.pos.x > VIRTUAL_WIDTH)
        {
            servingPlayer = 2;
            leftPlayer.score++;

            if (leftPlayer.score == 10)
            {
                // Player 1 is left Player
                winningPlayer = 1;
                gameState = STATE_DONE;
            }
            else
            {
                BallReset(&ball);
                gameState = STATE_SERVE;
            }
        }
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

    // Update entities
    if (gameState == STATE_PLAY)
    {
        BallUpdate(&ball, delta);
    }

    PaddleUpdate(&leftPlayer, delta);
    PaddleUpdate(&rightPlayer, delta);
}

internal void
GameDraw(void)
{
    // Variables
    Color backgroundColor = { 40, 45, 52, 255 };

    ClearBackground(backgroundColor);

    DisplayScore();

    if (gameState == STATE_START)
    {
        DrawCenteredText("Welcome to Bong!", 10.0f, smallSize);
        DrawCenteredText("Press Enter to begin!", 20.0f, smallSize);
    }
    else if (gameState == STATE_SERVE)
    {
        DrawCenteredText(TextFormat("Player %d's serve!", servingPlayer), 10.0f, smallSize);
        DrawCenteredText("Press Enter to serve!", 20.0f, smallSize);
    }
    else if (gameState == STATE_PLAY)
    {
        /* no UI messages to display in play */
    }
    else if (gameState == STATE_DONE)
    {
        /* UI messages */
        DrawCenteredText(TextFormat("Player %d wins!", winningPlayer), 10.0f, largeSize);
        DrawCenteredText("Press Enter to restart!", 30.0f, smallSize);
    }

    // Draw all the game objects
    PaddleRender(&leftPlayer);
    PaddleRender(&rightPlayer);
    BallRender(&ball);

    DisplayFPS();

}

internal void
GameClose(void)
{
    UnloadFont(font);
    CloseWindow();
}

// Displays the current FPS
internal void
DisplayFPS(void)
{
    const char* FPS = TextFormat("FPS: %d", GetFPS());
    DrawTextEx(font, FPS, (v2){ 10.0f, 10.0f }, smallSize, 0, GREEN);
}

// Simply draws the score to the screen
internal void
DisplayScore(void)
{
    const char* score;

    // Display the score
    score = TextFormat("%d", leftPlayer.score);
    DrawTextEx(
            font, score,
            (v2){((VIRTUAL_WIDTH / 2.0f) - MeasureTextEx(font, score, scoreSize, 1).x) -
            30.0f,
            VIRTUAL_HEIGHT / 3.0f},
            32, 1, WHITE);

    score = TextFormat("%d", rightPlayer.score);
    DrawTextEx(font, score,
               (v2){VIRTUAL_WIDTH / 2.0f + 30.0f, VIRTUAL_HEIGHT / 3.0f}, scoreSize, 1,
               WHITE);
}

// you know it
internal void
DrawCenteredText(const char* text, f32 pos, f32 fontSize)
{
    v2 textSize = MeasureTextEx(font, text, fontSize, 0);
    v2 position = { VIRTUAL_WIDTH * 0.5f - textSize.x * 0.5f, pos };

    DrawTextEx(font, text, position, fontSize, 0, WHITE);
}
