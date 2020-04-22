#ifndef TILECELL_H
#define TILECELL_H

#include "Cell.h"

class TileCell : public Cell
{
private:
    Direction _dir;
public:
    TileCell(Direction dir);

    Direction get_dir();
};

#endif