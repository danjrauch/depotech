#ifndef CELL_H
#define CELL_H

enum CellTypes { Delivery, Idle, Pickup, Tile, Error };
enum Direction { North, South, East, West, None };

class Cell
{
private:
    bool _occupied;
    CellTypes _type;
public:
    Cell(CellTypes type);
    virtual ~Cell() = default;

    int get_occupied();
    CellTypes get_type();
};

#endif