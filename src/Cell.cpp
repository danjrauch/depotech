#include "Cell.h"

void Cell::add_dir(std::pair<int, int> to_pos)
{
    if(std::count(_dirs.begin(), _dirs.end(), to_pos) == 0)
        _dirs.push_back(to_pos);
}

std::vector<std::pair<int, int>> Cell::get_dirs()
{
    return _dirs;
}

CellLabels Cell::get_label()
{
    return _label;
}