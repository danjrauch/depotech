#include "WorkAgent.h"

/*
    Constructors
*/

WorkAgent::WorkAgent(int row, int col) : Agent(row, col){}

/*
    Member Functions
*/

bool WorkAgent::move(int drow, int dcol)
{
    _row += drow;
    _col += dcol;
    return true;
}

bool WorkAgent::move()
{
    if(_path.empty() || _path_i < 0 || _path_i >= _path.size())
        return false;
    int drow = _path[_path_i].first - _row;
    int dcol = _path[_path_i].second - _col;
    move(drow, dcol);
    _path_i++;
    if(_path_i == _path.size()){
        _path.clear();
        _path_i = 0;
    }
    return true;
}