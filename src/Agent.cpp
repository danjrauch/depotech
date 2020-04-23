#include "Agent.h"

bool Agent::set_loaded(bool loaded){
    _loaded = loaded;
    return true;
}

bool Agent::set_path(std::vector<std::pair<int, int>> path)
{
    _path = std::move(path);
    _path_i = 0;
    return true;
}

int Agent::get_row()
{
    return _row;
}

int Agent::get_col()
{
    return _col;
}

bool Agent::get_loaded()
{
    return _loaded;
}

std::vector<std::pair<int, int>> Agent::get_path()
{
    return _path;
}