#include "PickupCell.h"

/*
    Constructors
*/

PickupCell::PickupCell() : Cell(CellLabels::Pickup){}

/*
    Member Functions
*/

bool PickupCell::in_action(){
    return true;
}

bool PickupCell::out_action(){
    return true;
}

bool PickupCell::idle_action(){
    return true;
}