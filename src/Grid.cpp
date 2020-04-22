#include "Grid.h"

/*
    Constructors
*/

/* FIXME Set TileCells back to Direction::None
         Try not to have to downcast cells
 */

Grid::Grid(int n_rows, int n_cols)
{
    _n_rows = n_rows;
    _n_cols = n_cols;

    _grid.resize(n_rows);
    for(auto &i : _grid){
        i.reserve(n_cols);
        for (int j = 0; j < n_cols; ++j)
            i.push_back(std::make_unique<TileCell>(Direction::North));
    }
}

/*
    Member Functions
*/

bool Grid::is_valid_pos(int row, int col)
{
    return row >= 0 && row < _grid.size() && col >= 0 && col < _grid[row].size();
}

bool Grid::is_cell_occupied(int row, int col)
{
    if(!is_valid_pos(row, col))
        return false;

    return _grid[row][col]->get_occupied();
}

bool Grid::set_cell(int row, int col, std::unique_ptr<Cell> cell)
{
    if(!is_valid_pos(row, col))
        return false;

    _grid[row][col] = std::move(cell);
    return true;
}

bool Grid::set_cell_block(int row, int col, int width, int height, CellTypes cell_type)
{
    for(int i = row; i < row + height; ++i){
        for(int j = col; j < col + width; ++j){
            if(is_valid_pos(i, j)){
                switch(cell_type){
                    case CellTypes::Tile:
                    {
                        set_cell(i, j, std::make_unique<TileCell>(Direction::None));
                        break;
                    }
                    case CellTypes::Idle:
                    {
                        set_cell(i, j, std::make_unique<IdleCell>());
                        break;
                    }
                    case CellTypes::Pickup:
                    {
                        set_cell(i, j, std::make_unique<PickupCell>());
                        break;
                    }
                    case CellTypes::Delivery:
                    {
                        set_cell(i, j, std::make_unique<DeliveryCell>());
                        break;
                    }
                    default:
                        return false;
                }
            }else
                return false;
        }
    }
    return true;
}

Cell * Grid::get_cell(int row, int col)
{
    if(!is_valid_pos(row, col))
        return nullptr;

    return _grid[row][col].get();
}

int Grid::get_n_rows()
{
    return _n_rows;
}

int Grid::get_n_cols()
{
    return _n_cols;
}