

#include "raylib.h"
#include <iostream>


const int screenWidth = 800;
const int screenHeight = 450;
const int paddleWidth = 20;
const int paddleHeight = 80;
const int ballSize = 15;
const int paddleSpeed = 5;
const int ballSpeed = 4;

enum GameState {
    MENU,
    GAME,
    GAME_OVER
};

int main()
{
    // Initialization
    InitWindow(screenWidth, screenHeight, "Pong Game");
    SetTargetFPS(60);

    // Game state
    GameState gameState = MENU;

    // Paddle variables
    Rectangle player1Paddle = { screenWidth / 6, screenHeight / 2 - paddleHeight / 2, paddleWidth, paddleHeight };
    Rectangle player2Paddle = { screenWidth - screenWidth / 6 - paddleWidth, screenHeight / 2 - paddleHeight / 2, paddleWidth, paddleHeight };

    // Ball variables
    Vector2 ballPosition = { screenWidth / 2, screenHeight / 2 };
    Vector2 ballSpeedVec = { ballSpeed, ballSpeed };
    Rectangle ballRect = { ballPosition.x - ballSize / 2, ballPosition.y - ballSize / 2, ballSize, ballSize };

    // Scoring variables
    int player1Score = 0;
    int player2Score = 0;
    const int maxScore = 5;

    while (!WindowShouldClose())
    {
        // Update
        switch (gameState)
        {
            case MENU:
            {
                if (IsKeyPressed(KEY_ENTER))
                {
                    gameState = GAME;
                }
                break;
            }
            case GAME:
            {
                if (IsKeyDown(KEY_W) && player1Paddle.y > 0)
                    player1Paddle.y -= paddleSpeed;
                if (IsKeyDown(KEY_S) && player1Paddle.y + player1Paddle.height < screenHeight)
                    player1Paddle.y += paddleSpeed;

                // AI-controlled paddle movement
                if (ballPosition.y < player2Paddle.y + player2Paddle.height / 2 && player2Paddle.y > 0)
                    player2Paddle.y -= paddleSpeed * 0.75; // Adjust the speed here (0.75 is a slower speed)
                if (ballPosition.y > player2Paddle.y + player2Paddle.height / 2 && player2Paddle.y + player2Paddle.height < screenHeight)
                    player2Paddle.y += paddleSpeed * 0.75; // Adjust the speed here (0.75 is a slower speed)

                ballPosition.x += ballSpeedVec.x;
                ballPosition.y += ballSpeedVec.y;

                // Update ball rectangle position
                ballRect.x = ballPosition.x - ballSize / 2;
                ballRect.y = ballPosition.y - ballSize / 2;

                // Ball collision with paddles
                if (CheckCollisionRecs(ballRect, player1Paddle) && ballSpeedVec.x < 0) {
                    ballSpeedVec.x *= -1;
                    ballSpeedVec.y = (ballPosition.y - (player1Paddle.y + player1Paddle.height / 2)) / (player1Paddle.height / 2) * ballSpeed;
                }
                if (CheckCollisionRecs(ballRect, player2Paddle) && ballSpeedVec.x > 0) {
                    ballSpeedVec.x *= -1;
                    ballSpeedVec.y = (ballPosition.y - (player2Paddle.y + player2Paddle.height / 2)) / (player2Paddle.height / 2) * ballSpeed;
                }

                // Ball collision with walls
                if (ballPosition.y + ballSize / 2 >= screenHeight || ballPosition.y - ballSize / 2 <= 0)
                    ballSpeedVec.y *= -1;

                // Ball out of bounds
                if (ballPosition.x - ballSize / 2 <= 0)
                {
                    player2Score++;
                    ballPosition = { screenWidth / 2, screenHeight / 2 };
                    ballSpeedVec = { ballSpeed, ballSpeed };
                }
                else if (ballPosition.x + ballSize / 2 >= screenWidth)
                {
                    player1Score++;
                    ballPosition = { screenWidth / 2, screenHeight / 2 };
                    ballSpeedVec = { -ballSpeed, -ballSpeed };
                }

                if (player1Score >= maxScore || player2Score >= maxScore)
                {
                    gameState = GAME_OVER;
                }

                break;
            }
            case GAME_OVER:
            {
                if (IsKeyPressed(KEY_ENTER))
                {
                    player1Score = 0;
                    player2Score = 0;
                    gameState = GAME;
                }
                break;
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        switch (gameState)
        {
            case MENU:
            {
                DrawText("PONG", screenWidth / 2 - MeasureText("PONG", 50) / 2, screenHeight / 2 - 50, 50, WHITE);
                DrawText("Press ENTER to start", screenWidth / 2 - MeasureText("Press ENTER to start", 20) / 2, screenHeight / 2 + 50, 20, WHITE);
                break;
            }
            case GAME:
            {
                DrawRectangleRec(player1Paddle, WHITE);
                DrawRectangleRec(player2Paddle, WHITE);
                DrawCircleV(ballPosition, ballSize / 2, WHITE);

                DrawText(FormatText("%02d", player1Score), screenWidth / 2 - 50, 10, 40, WHITE);
                DrawText(FormatText("%02d", player2Score), screenWidth / 2 + 30, 10, 40, WHITE);
                break;
            }
            case GAME_OVER:
            {
                DrawText("GAME OVER", screenWidth / 2 - MeasureText("GAME OVER", 50) / 2, screenHeight / 2 - 50, 50, WHITE);
                DrawText("Press ENTER to restart", screenWidth / 2 - MeasureText("Press ENTER to restart", 20) / 2, screenHeight / 2 + 50, 20, WHITE);
                break;
            }
        }

        EndDrawing();
    }

    // Cleanup
    CloseWindow();

    return 0;
}