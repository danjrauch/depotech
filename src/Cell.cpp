#include "Cell.h"

/*
    Constructors
*/

Cell::Cell(CellTypes type)
{
    _type = type;
}

/*
    Member Functions
*/

int Cell::get_occupied()
{
    return _occupied;
}

CellTypes Cell::get_type()
{
    return _type;
}