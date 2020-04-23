#include "IdleCell.h"

/*
    Constructors
*/

IdleCell::IdleCell() : Cell(CellLabels::Idle){}

/*
    Member Functions
*/

bool IdleCell::in_action(){
    return true;
}

bool IdleCell::out_action(){
    return true;
}

bool IdleCell::idle_action(){
    return true;
}