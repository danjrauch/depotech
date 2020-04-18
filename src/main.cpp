#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "raylib.h"
#include "block.h"

using namespace std;

const int tileWidth = 15;

const int screenWidth = tileWidth * 56;
const int screenHeight = tileWidth * 24;

const double agentRadius = 0.3*tileWidth;

const int dr[4] = { 0, 0, 1, -1 };
const int dc[4] = { 1, -1, 0, 0 };

vector<DeliveryBlock> deliveryBlockPositions { {5,5,6,2}, {9,5,6,2}, {13,5,6,2}, {17,5,6,2},
                                                {5,45,6,2}, {9,45,6,2}, {13,45,6,2}, {17,45,6,2} };

vector<PickupBlock> pickupBlockPositions { {5,13,6,2}, {5,21,6,2}, {5,29,6,2}, {5,37,6,2},
                                           {9,13,6,2}, {9,21,6,2}, {9,29,6,2}, {9,37,6,2},
                                           {13,13,6,2}, {13,21,6,2}, {13,29,6,2}, {13,37,6,2},
                                           {17,13,6,2}, {17,21,6,2}, {17,29,6,2}, {17,37,6,2} };

vector<IdleBlock> idleBlockPositions { {4,0,3,4}, {10,0,3,4}, {16,0,3,4},
                                       {0,10,4,3}, {0,18,4,3}, {0,26,4,3}, {0,34,4,3}, {0,42,4,3},
                                       {4,53,3,4}, {10,53,3,4}, {16,53,3,4},
                                       {21,10,4,3}, {21,18,4,3}, {21,26,4,3}, {21,34,4,3}, {21,42,4,3} };

bool createDeliveryBlock(int row, int col, int width, int height, vector<vector<char>> &grid){
    for(int i = row; i < row + height; ++i){
        for(int j = col; j < col + width; ++j){
            if(i >= 0 && i < grid.size() && j >= 0 && j < grid[i].size())
                grid[i][j] = 'D';
            else
                return false;
        }
    }
    return true;
}

bool createPickupBlock(int row, int col, int width, int height, vector<vector<char>> &grid){
    for(int i = row; i < row + height; ++i){
        for(int j = col; j < col + width; ++j){
            if(i >= 0 && i < grid.size() && j >= 0 && j < grid[i].size())
                grid[i][j] = 'P';
            else
                return false;
        }
    }
    return true;
}

bool createIdleBlock(int row, int col, int width, int height, vector<vector<char>> &grid){
    for(int i = row; i < row + height; ++i){
        for(int j = col; j < col + width; ++j){
            if(i >= 0 && i < grid.size() && j >= 0 && j < grid[i].size())
                grid[i][j] = 'I';
            else
                return false;
        }
    }
    return true;
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Depotech Simulator");

    SetTargetFPS(60);

    srand(time(NULL));

    vector<vector<char>> grid(screenHeight/tileWidth, vector<char>(screenWidth/tileWidth, 'O'));

    for(DeliveryBlock b : deliveryBlockPositions)
        createDeliveryBlock(b.row, b.col, b.height, b.width, grid);

    for(PickupBlock b : pickupBlockPositions)
        createPickupBlock(b.row, b.col, b.height, b.width, grid);

    for(IdleBlock b : idleBlockPositions)
        createIdleBlock(b.row, b.col, b.height, b.width, grid);

    for(int i = 0; i < grid.size(); ++i)
        for(int j = 0; j < grid[i].size(); ++j){
            if((rand() % 100 + 1) == 1 && grid[i][j] == 'O')
                grid[i][j] = 'A';
        }

    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        for(int i = 0; i < grid.size(); ++i){
            for(int j = 0; j < grid[i].size(); ++j){
                int k = rand() % 4;
                int r = i+dr[k];
                int c = j+dc[k];
                if(r >= 0 && r < grid.size() &&
                   c >= 0 && c < grid[i].size() &&
                   grid[i][j] == 'A' && grid[r][c] == 'O'){
                    grid[i][j] = 'O';
                    grid[r][c] = 'A';
                }
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(LIGHTGRAY);

            for(int i = 0; i < grid.size(); ++i)
                for(int j = 0; j < grid[i].size(); ++j){
                    switch(grid[i][j]){
                        case 'O':
                            // DrawRectangleLines(j*tileWidth, i*tileWidth, tileWidth, tileWidth, BLACK);
                            break;
                        case 'D':
                            DrawRectangle(j*tileWidth, i*tileWidth, tileWidth, tileWidth, BLUE);
                            DrawRectangleLines(j*tileWidth, i*tileWidth, tileWidth, tileWidth, BLACK);
                            if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
                                DrawText("D", j*tileWidth+(float)tileWidth*0.2, i*tileWidth+(float)tileWidth*0.2, 4, BLACK);
                            break;
                        case 'P':
                            DrawRectangle(j*tileWidth, i*tileWidth, tileWidth, tileWidth, GREEN);
                            DrawRectangleLines(j*tileWidth, i*tileWidth, tileWidth, tileWidth, BLACK);
                            if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
                                DrawText("P", j*tileWidth+(float)tileWidth*0.2, i*tileWidth+(float)tileWidth*0.2, 4, BLACK);
                            break;
                        case 'I':
                            DrawRectangle(j*tileWidth, i*tileWidth, tileWidth, tileWidth, MAROON);
                            DrawRectangleLines(j*tileWidth, i*tileWidth, tileWidth, tileWidth, BLACK);
                            if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
                                DrawText("I", j*tileWidth+(float)tileWidth*0.2, i*tileWidth+(float)tileWidth*0.2, 4, BLACK);
                            break;
                        case 'A':
                            DrawCircleV({j*tileWidth+(float)tileWidth/2, i*tileWidth+(float)tileWidth/2}, agentRadius, BLACK);
                            if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
                                DrawText("A", j*tileWidth+(float)tileWidth*0.2, i*tileWidth+(float)tileWidth*0.2, 4, BLACK);
                            break;
                    }
                }

            DrawFPS(0, 0);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}