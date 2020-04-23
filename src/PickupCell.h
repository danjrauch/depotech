#ifndef PICKUPCELL_H
#define PICKUPCELL_H

#include "Cell.h"

class PickupCell : public Cell
{
private:

public:
    PickupCell();

    bool in_action();
    bool out_action();
    bool idle_action();
};

#endif