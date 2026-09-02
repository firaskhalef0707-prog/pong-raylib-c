#include <raylib.h>

typedef struct {
    float x;
    float y;
    int speedx;
    int speedy;
    int radius;
} Ball;

void DrawBall(Ball ball)
{
    DrawCircle(
        (int)ball.x,
        (int)ball.y,
        (float)ball.radius,
        WHITE
    );
}

void UpdateBall(Ball *ball)
{
    ball->x += ball->speedx;
    ball->y += ball->speedy;

    if (ball->y + ball->radius >= GetScreenHeight())
    {
        ball->speedy *= -1;
        ball->y = GetScreenHeight() - ball->radius;
    }

    if (ball->y - ball->radius < 1)
    {
        ball->speedy *= -1;
        ball->y = ball->radius;
    }
}

void ResetBall(Ball *ball)
{
    ball->x = GetScreenWidth() / 2;
    ball->y = GetScreenHeight() / 2;

    ball->speedx *= -1;
}

typedef struct {
    float x;
    float y;
    float width;
    float height;
    int speed;
} Paddle;

void DrawPaddle(Paddle paddle)
{
    DrawRectangle(
        (int)paddle.x,
        (int)paddle.y,
        (int)paddle.width,
        (int)paddle.height,
        BLUE
    );
}

void UpdatePlayer(Paddle *player)
{
    if (IsKeyDown(KEY_UP))
    {
        player->y -= player->speed;
    }

    if (IsKeyDown(KEY_DOWN))
    {
        player->y += player->speed;
    }

    if (player->y < 1)
    {
        player->y = 1;
    }

    if (player->y + player->height > GetScreenHeight())
    {
        player->y = GetScreenHeight() - player->height;
    }
}

typedef struct {
    Paddle paddle;
} CpuPaddle;

void DrawCpuPaddle(CpuPaddle cpu)
{
    DrawPaddle(cpu.paddle);
}

void UpdateCpuPaddle(CpuPaddle *cpu, Ball ball)
{
    float paddleCenter =
        cpu->paddle.y + cpu->paddle.height / 2;

    if (paddleCenter < ball.y)
    {
        cpu->paddle.y += cpu->paddle.speed;
    }
    else if (paddleCenter > ball.y)
    {
        cpu->paddle.y -= cpu->paddle.speed;
    }

    if (cpu->paddle.y < 0)
    {
        cpu->paddle.y = 0;
    }

    if (cpu->paddle.y + cpu->paddle.height > GetScreenHeight())
    {
        cpu->paddle.y =
            GetScreenHeight() - cpu->paddle.height;
    }
}

int main()
{
    SetTraceLogLevel(LOG_NONE);

    InitWindow(1000, 800, "Ping Pong");

    SetTargetFPS(60);

    Paddle player;
    player.width = 25;
    player.height = 120;
    player.x = 950;

    player.y = 340;
    player.speed = 7;


    CpuPaddle cpu;

    cpu.paddle.width = 25;
    cpu.paddle.height = 120;
    cpu.paddle.x = 25;
    cpu.paddle.y = 340;
    cpu.paddle.speed = 3;

    Ball ball;


    ball.x = 500;
    ball.y = 400;
    ball.radius = 20;
    ball.speedx = 6;
    ball.speedy = 6;

    int playerScore = 0;

    int cpuScore = 0;

    while (!WindowShouldClose())
    {
        UpdateBall(&ball);

        UpdatePlayer(&player);

        UpdateCpuPaddle(&cpu, ball);


        if (CheckCollisionCircleRec(
                (Vector2){ball.x, ball.y},
                ball.radius,

                (Rectangle){
                    player.x,

                    player.y,
                    player.width,
                    player.height
                }
            ))
        {
            if (ball.speedx > 0)

            {
                ball.speedx *= -1;
                ball.x = player.x - ball.radius;
            }
        }

        if (CheckCollisionCircleRec(
                (Vector2){ball.x, ball.y},
                ball.radius,
                (Rectangle){
                    cpu.paddle.x,
                    cpu.paddle.y,
                    cpu.paddle.width,
                    cpu.paddle.height
                }
            ))
        {
            if (ball.speedx < 0)
            {
                ball.speedx *= -1;

                ball.x =
                    cpu.paddle.x +
                    cpu.paddle.width +
                    ball.radius;
            }
        }

        if (ball.x + ball.radius < 0)
        {
            playerScore++;
            ResetBall(&ball);
        }

        if (ball.x - ball.radius > GetScreenWidth())
        {
            cpuScore++;
            ResetBall(&ball);
        }

        BeginDrawing();

        ClearBackground(BLACK);

        DrawLine(
            GetScreenWidth() / 2,
            0,
            GetScreenWidth() / 2,
            GetScreenHeight(),

            WHITE
        );

        DrawText(
            TextFormat("%d", cpuScore),
            250,
            50,
            60,
            WHITE
        );

        DrawText(
            TextFormat("%d", playerScore),
            750,
            50,
            60,
            WHITE
        );

        DrawBall(ball);

        DrawPaddle(player);

        DrawCpuPaddle(cpu);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
