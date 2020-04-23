#ifndef TILECELL_H
#define TILECELL_H

#include "Cell.h"

class TileCell : public Cell
{
private:

public:
    TileCell();

    bool in_action();
    bool out_action();
    bool idle_action();
};

#endif