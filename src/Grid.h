#ifndef GRID_H
#define GRID_H

#include <vector>
#include <memory>
#include "Cell.h"
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

public:
    Grid(int n_rows, int n_cols);

    bool is_valid_pos(int row, int col);
    bool is_cell_occupied(int row, int col);

    bool set_cell(int row, int col, std::unique_ptr<Cell> cell);
    bool set_cell_block(int row, int col, int width, int height, CellTypes cell_type);

    Cell * get_cell(int row, int col);
    int get_n_rows();
    int get_n_cols();
};

#endif