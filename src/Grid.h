#ifndef GRID_H
#define GRID_H

#include <vector>
#include <utility>
#include <queue>
#include <stack>
#include <memory>
#include "WorkAgent.h"
#include "TileCell.h"
#include "IdleCell.h"
#include "PickupCell.h"
#include "DeliveryCell.h"

class Grid
{
private:
    int _n_rows;
    int _n_cols;
    std::vector<std::vector<std::unique_ptr<Cell>>> _grid;
    std::vector<std::unique_ptr<Agent>> _agents;

    std::vector<std::pair<int, int>> dijkstras(std::pair<int, int> src, std::pair<int, int> dest);
public:
    Grid(int n_rows, int n_cols);

    bool is_valid_pos(int row, int col);

    std::vector<std::pair<int, int>> find_path(std::pair<int, int> src, std::pair<int, int> dest);
    std::pair<int, int> find_nearest_empty(std::pair<int, int> src, CellLabels label);

    bool add_agent(std::unique_ptr<Agent> agent);
    bool signal_agents();

    bool set_cell(int row, int col, std::unique_ptr<Cell> cell);
    bool set_cell_block(int row, int col, int width, int height, CellLabels cell_type);

    int get_n_rows();
    int get_n_cols();
    Cell * get_cell(int row, int col);
    std::vector<std::pair<int, int>> get_agent_positions();
};

#endif