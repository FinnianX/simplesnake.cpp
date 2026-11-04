#include <iostream>
#include "raylib.h"
#include <vector>
#include <random>
#include <cmath>
#include <deque>
#include <raymath.h>
#include <format>
using namespace std;
Color green = { 173, 204, 96, 255 };
Color darkGreen = { 43, 51, 24, 255 };
const Vector2 DOWN = { 0, 1 };
const Vector2 UP = { 0, -1 };
const Vector2 LEFT = { -1, 0 };
const Vector2 RIGHT = { 1, 0 };
int gridsize = 30;
int gridcount = 30;
bool gameOver = false;
bool victory = false;
void renderCenterText(string text, int fontsize, Color color) {
    int textWidth = MeasureText(text.c_str(), fontsize);
    DrawText(text.c_str(), (gridsize*gridcount - textWidth) / 2, (gridsize*gridcount - fontsize) / 2, fontsize, color);
};
class Food {
public:
    Vector2 position;
    Food() {
        position = GenerateRandomPos();
    }
    void Draw() {
        int x = position.x;
        int y = position.y;
        DrawRectangle(x * gridsize, y * gridsize, gridsize, gridsize, RED);
    }
    Vector2 GenerateRandomPos() {
        float x = GetRandomValue(0, gridcount - 1);
        float y = GetRandomValue(0, gridcount - 1);
        return Vector2{ x, y };
    }
};
class Snake {
    public:
        deque<Vector2> body = { Vector2{ 6,9 }, Vector2{ 5,9 } , Vector2{ 4,9 } };
        Vector2 direction = {1,0};
        int length = 3;
        void Draw() {
            for (int i = 0; i < body.size();i++) {
                int x = body[i].x;
                int y = body[i].y;
                Rectangle segment = Rectangle{ (float)x * gridsize, (float)y * gridsize, (float)gridsize, (float)gridsize };
                DrawRectangleRounded(segment, 0.5, 6, darkGreen);
            };
        };
        void Update(Food& food) {
            body.pop_back();
            body.push_front(Vector2Add(body[0], direction));
            int i = 0;
            for (Vector2 val : body) {
                Vector2 vecval = { val.x, val.y };
                if (vecval.x == body.front().x && vecval.y == body.front().y && i != 0) {    
                    gameOver = true;
                }
                if (val.x >= gridcount || val.y >= gridcount) {
                    gameOver = true;
                }
                if (val.x < 0 || val.y < 0) {
                    gameOver = true;
                }
                if (val.x == food.position.x && val.y == food.position.y) {
                    food.position = food.GenerateRandomPos();
                    body.push_back(Vector2Add(body.back(), direction));
                    length += 1;
                   
                }
                i += 1;
            }
            string point = format("Points: {}", length);
            const char* points = point.c_str();
            DrawText(points, gridsize * 2, gridsize * 2, 40, darkGreen);
        };
}; 
int main()
{
    InitWindow(gridsize*gridcount, gridsize*gridcount, "Circle");
    SetTargetFPS(12);
    Snake snake = Snake();
    Food food = Food();
    Vector2 nextDirection = RIGHT;
    while (!WindowShouldClose())
    {
        if (!gameOver) {
            BeginDrawing();
            ClearBackground(green);
            if (IsKeyPressed(KEY_DOWN)) {
                if (snake.direction != UP) {
                    nextDirection = DOWN;
                }
            }
            else if (IsKeyPressed(KEY_UP)) {
                if (snake.direction != DOWN) {
                    nextDirection = UP;
                }
            }
            else if (IsKeyPressed(KEY_LEFT)) {
                if (snake.direction != RIGHT) {
                    nextDirection = LEFT;
                }
            }
            else if (IsKeyPressed(KEY_RIGHT)) {
                if (snake.direction != LEFT) {
                    nextDirection = RIGHT;
                }
            }
            if (snake.body.size() >= gridsize * gridcount) {
                victory = true;
            }
            snake.direction = nextDirection;
            snake.Update(food);
            snake.Draw();
            food.Draw();
            EndDrawing();
        }
        if (gameOver || victory) {
            BeginDrawing();
            ClearBackground(green);
            if (victory) { renderCenterText("You Win! Press R to restart.", 40, darkGreen); }
            else if (gameOver) { renderCenterText("Game Over. Press R to restart.", 40, darkGreen); }
            if (IsKeyPressed(KEY_R)) {
                gameOver = false;
                snake.body = { Vector2{ 6,9 }, Vector2{ 5,9 } , Vector2{ 4,9 } };
                snake.direction = RIGHT;
                snake.length = 3;

            }
            EndDrawing();
        }
    }
    CloseWindow();
}
