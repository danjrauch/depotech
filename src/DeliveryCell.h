#ifndef DELIVERYCELL_H
#define DELIVERYCELL_H

#include "Cell.h"

class DeliveryCell : public Cell
{
private:

public:
    DeliveryCell();

    bool in_action();
    bool out_action();
    bool idle_action();
};

#endif