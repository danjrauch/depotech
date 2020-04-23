#include "DeliveryCell.h"

/*
    Constructors
*/

DeliveryCell::DeliveryCell() : Cell(CellLabels::Delivery){}

/*
    Member Functions
*/

bool DeliveryCell::in_action(){
    return true;
}

bool DeliveryCell::out_action(){
    return true;
}

bool DeliveryCell::idle_action(){
    return true;
}