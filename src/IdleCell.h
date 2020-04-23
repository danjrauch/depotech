#ifndef IDLECELL_H
#define IDLECELL_H

#include "Cell.h"

class IdleCell : public Cell
{
private:

public:
    IdleCell();

    bool in_action();
    bool out_action();
    bool idle_action();
};

#endif