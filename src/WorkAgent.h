#ifndef WORKAGENT_H
#define WORKAGENT_H

#include "Agent.h"

class WorkAgent : public Agent
{
private:

public:
    WorkAgent(int row, int col);

    bool move(int drow, int dcol);
    bool move();
};

#endif