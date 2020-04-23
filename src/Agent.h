#ifndef AGENT_H
#define AGENT_H

#include <vector>
#include <utility>

class Agent
{
protected:
    int _row;
    int _col;
    bool _loaded;
    std::vector<std::pair<int, int>> _path;
    int _path_i;
public:
    Agent() : _row(), _col() {}
    Agent(int row, int col) : _row(row), _col(col) {}
    virtual ~Agent() = default;

    bool set_loaded(bool loaded);
    bool set_path(std::vector<std::pair<int, int>> path);

    int get_row();
    int get_col();
    bool get_loaded();
    std::vector<std::pair<int, int>> get_path();

    virtual bool move(int drow, int dcol) = 0;
    virtual bool move() = 0;
};

#endif