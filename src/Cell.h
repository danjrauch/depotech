#ifndef CELL_H
#define CELL_H

#include <vector>
#include <utility>

enum CellLabels { Delivery, Idle, Pickup, Tile, Error };
enum Directions { North, South, East, West, None };

class Cell
{
protected:
    std::vector<std::pair<int, int>> _dirs;
    CellLabels _label;
public:
    Cell() : _dirs(), _label() {}
    Cell(CellLabels label) : _label(label) {}
    Cell(std::vector<std::pair<int, int>> dirs) : _dirs(std::move(dirs)) {}
    Cell(std::vector<std::pair<int, int>> dirs, CellLabels label) : _dirs(std::move(dirs)), _label(label) {}
    virtual ~Cell() = default;

    void add_dir(std::pair<int, int> to_pos);
    std::vector<std::pair<int, int>> get_dirs();
    CellLabels get_label();

    virtual bool in_action() = 0;
    virtual bool out_action() = 0;
    virtual bool idle_action() = 0;
};

#endif