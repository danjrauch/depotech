#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "raylib.h"
#include "raymath.h"
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

void DrawCellDirection(int row, int col, Directions dir)
{
    float x = (float)col*tile_width;
    float y = (float)row*tile_width;
    int offset = (float)tile_width * 0.35;
    Vector2 top_left = {x+offset, y+offset};
    Vector2 top_right = {x+tile_width-offset, y+offset};
    Vector2 bot_left = {x+offset, y+tile_width-offset};
    Vector2 bot_right = {x+tile_width-offset, y+tile_width-offset};
    float arrow_side_length = Vector2Distance(top_left, bot_left);
    float arrow_height = arrow_side_length * sqrtf(3) / 2;
    switch(dir)
    {
        case Directions::West:
            DrawTriangle(top_left, {x+offset-arrow_height, y+offset+arrow_side_length*(float)0.5}, bot_left, RED);
            break;
        case Directions::North:
            DrawTriangle(top_left, top_right, {x+offset+arrow_side_length*(float)0.5, y+offset-arrow_height}, RED);
            break;
        case Directions::East:
            DrawTriangle(top_right, bot_right, {x+tile_width-offset+arrow_height, y+offset+arrow_side_length*(float)0.5}, RED);
            break;
        case Directions::South:
            DrawTriangle(bot_left, {x+offset+arrow_side_length*(float)0.5, y+tile_width-offset+arrow_height}, bot_right, RED);
            break;
        default:
            break;
    }
}

std::pair<int, int> GetMouseCell(){
    int mouse_x = GetMouseX();
    int mouse_y = GetMouseY();
    int row = mouse_y / tile_width;
    int col = mouse_x / tile_width;
    return make_pair(row, col);
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
        G.set_cell_block(b.row, b.col, b.height, b.width, CellLabels::Delivery);

    for(PickupBlock b : pickup_block_positions)
        G.set_cell_block(b.row, b.col, b.height, b.width, CellLabels::Pickup);

    for(IdleBlock b : idle_block_positions)
        G.set_cell_block(b.row, b.col, b.height, b.width, CellLabels::Idle);

    for(int i = 0; i < G.get_n_rows(); ++i)
        for(int j = 0; j < G.get_n_cols(); ++j){
            if(G.get_cell(i, j)->get_label() == CellLabels::Idle){
                std::unique_ptr<Agent> agent = std::make_unique<WorkAgent>(i, j);
                // agent->set_path()
                G.add_agent(std::move(agent));
            }
        }

    std::pair<int, int> prev_mouse_cell = GetMouseCell();
    std::pair<int, int> mouse_cell = GetMouseCell();

    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        if(IsKeyDown(KEY_SPACE)){
            mouse_cell = GetMouseCell();
            if(G.is_valid_pos(mouse_cell.first, mouse_cell.second)){
                std::vector<std::pair<int, int>> path = G.find_path(prev_mouse_cell, {0,0});
                if(!path.empty()){
                    for(auto p : path)
                        DrawRectangle(p.second*tile_width, p.first*tile_width, tile_width, tile_width, LIME);
                }
            }
            if(abs(prev_mouse_cell.first - mouse_cell.first) > 1 || abs(prev_mouse_cell.second - mouse_cell.second) > 1){
                prev_mouse_cell = GetMouseCell();
            }
            if(IsKeyDown(KEY_SPACE) && G.is_valid_pos(mouse_cell.first, mouse_cell.second) && G.is_valid_pos(prev_mouse_cell.first, prev_mouse_cell.second) && mouse_cell != prev_mouse_cell){
                int drow = mouse_cell.first - prev_mouse_cell.first;
                int dcol = mouse_cell.second - prev_mouse_cell.second;
                G.get_cell(prev_mouse_cell.first, prev_mouse_cell.second)->add_dir(std::make_pair(drow, dcol));
            }
            prev_mouse_cell = mouse_cell;
        }

        G.signal_agents();

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(LIGHTGRAY);

            for(int i = 0; i < G.get_n_rows(); ++i)
                for(int j = 0; j < G.get_n_cols(); ++j){
                    switch(G.get_cell(i, j)->get_label()){
                        case CellLabels::Tile:
                            // DrawRectangleLines(j*tile_width, i*tile_width, tile_width, tile_width, BLACK);
                            if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
                                for(auto p : G.get_cell(i, j)->get_dirs()){
                                    if(p == make_pair(0, 1))
                                        DrawCellDirection(i, j, Directions::East);
                                    else if(p == make_pair(1, 0))
                                        DrawCellDirection(i, j, Directions::South);
                                    else if(p == make_pair(0, -1))
                                        DrawCellDirection(i, j, Directions::West);
                                    else if(p == make_pair(-1, 0))
                                        DrawCellDirection(i, j, Directions::North);
                                }
                            }
                            break;
                        case CellLabels::Delivery:
                            DrawRectangle(j*tile_width, i*tile_width, tile_width, tile_width, BLUE);
                            DrawRectangleLines(j*tile_width, i*tile_width, tile_width, tile_width, BLACK);
                            if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
                                DrawText("D", j*tile_width+(float)tile_width*0.2, i*tile_width+(float)tile_width*0.2, 4, BLACK);
                            break;
                        case CellLabels::Pickup:
                            DrawRectangle(j*tile_width, i*tile_width, tile_width, tile_width, GREEN);
                            DrawRectangleLines(j*tile_width, i*tile_width, tile_width, tile_width, BLACK);
                            if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
                                DrawText("P", j*tile_width+(float)tile_width*0.2, i*tile_width+(float)tile_width*0.2, 4, BLACK);
                            break;
                        case CellLabels::Idle:
                            DrawRectangle(j*tile_width, i*tile_width, tile_width, tile_width, MAROON);
                            DrawRectangleLines(j*tile_width, i*tile_width, tile_width, tile_width, BLACK);
                            if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
                                DrawText("I", j*tile_width+(float)tile_width*0.2, i*tile_width+(float)tile_width*0.2, 4, BLACK);
                            break;
                        case CellLabels::Error:
                        default:
                            break;
                    }
                }

            for(auto pos : G.get_agent_positions()){
                int row = pos.first;
                int col = pos.second;
                DrawCircleV({col*tile_width+(float)tile_width/2, row*tile_width+(float)tile_width/2}, agent_radius, BLACK);
            }

            std::stringstream cell_string;
            cell_string << "Mouse is in cell [" << mouse_cell.first << ", " << mouse_cell.second << "]";
            DrawText(cell_string.str().c_str(), 0, 20, 10, BLACK);

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