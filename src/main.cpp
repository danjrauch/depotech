#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "raylib.h"
#include "block.h"
#include "Grid.h"

using namespace std;

const int tile_width = 15;

const int screen_width = tile_width * 56;
const int screen_height = tile_width * 26;

const double agent_radius = 0.3*tile_width;

const int dr[4] = { 0, 0, 1, -1 };
const int dc[4] = { 1, -1, 0, 0 };

vector<DeliveryBlock> delivery_block_positions { {6,5,6,2}, {10,5,6,2}, {14,5,6,2}, {18,5,6,2},
                                                {6,45,6,2}, {10,45,6,2}, {14,45,6,2}, {18,45,6,2} };

vector<PickupBlock> pickup_block_positions { {6,13,6,2}, {6,21,6,2}, {6,29,6,2}, {6,37,6,2},
                                           {10,13,6,2}, {10,21,6,2}, {10,29,6,2}, {10,37,6,2},
                                           {14,13,6,2}, {14,21,6,2}, {14,29,6,2}, {14,37,6,2},
                                           {18,13,6,2}, {18,21,6,2}, {18,29,6,2}, {18,37,6,2} };

vector<IdleBlock> idle_block_positions { {5,0,3,4}, {11,0,3,4}, {17,0,3,4},
                                       {0,10,4,3}, {0,18,4,3}, {0,26,4,3}, {0,34,4,3}, {0,42,4,3},
                                       {5,53,3,4}, {11,53,3,4}, {17,53,3,4},
                                       {23,10,4,3}, {23,18,4,3}, {23,26,4,3}, {23,34,4,3}, {23,42,4,3} };

void DrawCellDirection(int row, int col, Direction dir)
{
    float x = (float)col*tile_width;
    float y = (float)row*tile_width;
    int offset = (float)tile_width * 0.25;
    switch(dir)
    {
        case Direction::West:
            DrawTriangle({x+offset, y+tile_width*(float)0.5}, {x+tile_width-offset, y+tile_width-offset}, {x+tile_width-offset, y+offset}, RED);
            break;
        case Direction::North:
            DrawTriangle({x+offset, y+tile_width-offset}, {x+tile_width-offset, y+tile_width-offset}, {x+tile_width*(float)0.5, y+offset}, RED);
            break;
        case Direction::East:
            DrawTriangle({x+offset, y+offset}, {x+offset, y+tile_width-offset}, {x+tile_width-offset, y+tile_width*(float)0.5}, RED);
            break;
        case Direction::South:
            DrawTriangle({x+offset, y+offset}, {x+tile_width*(float)0.5, y+tile_width-offset}, {x+tile_width-offset, y+offset}, RED);
            break;
        default:
            break;
    }
}

int main(void)
{
    // Grid G (3, 3);

    // G.set_cell(2, 2, std::make_unique<PickupCell>(true));

    // for(int i = 0; i < G.get_n_rows(); ++i){
    //     for(int j = 0; j < G.get_n_cols(); ++j)
    //         printf("%d ", G.is_cell_occupied(i, j));
    //     printf("\n");
    // }
    // printf("\n");

    // printf("(2,2) is %s\n", G.is_cell_occupied(2,2) ? "occupied" : "unoccupied");

    

    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screen_width, screen_height, "Depotech Simulator");

    SetTargetFPS(60);

    srand(time(NULL));

    Grid G (screen_height/tile_width, screen_width/tile_width);

    for(DeliveryBlock b : delivery_block_positions)
        G.set_cell_block(b.row, b.col, b.height, b.width, CellTypes::Delivery);

    for(PickupBlock b : pickup_block_positions)
        G.set_cell_block(b.row, b.col, b.height, b.width, CellTypes::Pickup);

    for(IdleBlock b : idle_block_positions)
        G.set_cell_block(b.row, b.col, b.height, b.width, CellTypes::Idle);

    // for(int i = 0; i < grid.size(); ++i)
    //     for(int j = 0; j < grid[i].size(); ++j){
    //         if((rand() % 100 + 1) == 1 && grid[i][j] == 'O')
    //             grid[i][j] = 'A';
    //     }

    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        // for(int i = 0; i < grid.size(); ++i){
        //     for(int j = 0; j < grid[i].size(); ++j){
        //         int k = rand() % 4;
        //         int r = i+dr[k];
        //         int c = j+dc[k];
        //         if(r >= 0 && r < grid.size() &&
        //            c >= 0 && c < grid[i].size() &&
        //            grid[i][j] == 'A' && grid[r][c] == 'O'){
        //             grid[i][j] = 'O';
        //             grid[r][c] = 'A';
        //         }
        //     }
        // }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(LIGHTGRAY);

            for(int i = 0; i < G.get_n_rows(); ++i)
                for(int j = 0; j < G.get_n_cols(); ++j){
                    switch(G.get_cell(i, j)->get_type()){
                        case CellTypes::Tile:
                            // DrawRectangleLines(j*tile_width, i*tile_width, tile_width, tile_width, BLACK);
                            if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON)){
                                TileCell * tc = dynamic_cast<TileCell*>(G.get_cell(i, j));
                                if(tc)
                                    DrawCellDirection(i, j, tc->get_dir());
                            }
                            break;
                        case CellTypes::Delivery:
                            DrawRectangle(j*tile_width, i*tile_width, tile_width, tile_width, BLUE);
                            DrawRectangleLines(j*tile_width, i*tile_width, tile_width, tile_width, BLACK);
                            if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
                                DrawText("D", j*tile_width+(float)tile_width*0.2, i*tile_width+(float)tile_width*0.2, 4, BLACK);
                            break;
                        case CellTypes::Pickup:
                            DrawRectangle(j*tile_width, i*tile_width, tile_width, tile_width, GREEN);
                            DrawRectangleLines(j*tile_width, i*tile_width, tile_width, tile_width, BLACK);
                            if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
                                DrawText("P", j*tile_width+(float)tile_width*0.2, i*tile_width+(float)tile_width*0.2, 4, BLACK);
                            break;
                        case CellTypes::Idle:
                            DrawRectangle(j*tile_width, i*tile_width, tile_width, tile_width, MAROON);
                            DrawRectangleLines(j*tile_width, i*tile_width, tile_width, tile_width, BLACK);
                            if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
                                DrawText("I", j*tile_width+(float)tile_width*0.2, i*tile_width+(float)tile_width*0.2, 4, BLACK);
                            break;
                        // case 'A':
                        //     DrawCircleV({j*tile_width+(float)tile_width/2, i*tile_width+(float)tile_width/2}, agent_radius, BLACK);
                        //     if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
                        //         DrawText("A", j*tile_width+(float)tile_width*0.2, i*tile_width+(float)tile_width*0.2, 4, BLACK);
                        //     break;
                        case CellTypes::Error:
                        default:
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