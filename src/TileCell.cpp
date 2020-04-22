#include "TileCell.h"

/*
    Constructors
*/

TileCell::TileCell(Direction dir) : Cell(CellTypes::Tile)
{
    _dir = dir;
}

/*
    Member Functions
*/

Direction TileCell::get_dir()
{
    return _dir;
}