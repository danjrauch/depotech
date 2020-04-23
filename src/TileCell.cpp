#include "TileCell.h"

/*
    Constructors
*/

TileCell::TileCell() : Cell(CellLabels::Tile){}

/*
    Member Functions
*/

bool TileCell::in_action(){
    return true;
}

bool TileCell::out_action(){
    return true;
}

bool TileCell::idle_action(){
    return true;
}