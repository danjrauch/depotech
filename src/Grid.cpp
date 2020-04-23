#include "Grid.h"

/*
    Constructors
*/

Grid::Grid(int n_rows, int n_cols)
{
    _n_rows = n_rows;
    _n_cols = n_cols;

    _grid.resize(n_rows);
    for(auto &i : _grid){
        i.reserve(n_cols);
        for (int j = 0; j < n_cols; ++j)
            i.push_back(std::make_unique<TileCell>());
    }
}

/*
    Member Functions
*/

//pretty fast for grids
std::vector<std::pair<int, int>> Grid::dijkstras(std::pair<int, int> src, std::pair<int, int> dest){
    long INF = LONG_MAX;
    int dr[] = {1, -1, 0, 0};
    int dc[] = {0, 0, 1, -1};
    std::vector<std::vector<long>> dists(_n_rows, std::vector<long>(_n_cols, INF));
    std::vector<std::vector<std::pair<int, int>>> preds(_n_rows, std::vector<std::pair<int, int>>(_n_cols, std::make_pair(-1, -1)));
    dists[src.first][src.second] = 0;
    std::priority_queue<std::pair<long, std::pair<int, int>>, std::vector<std::pair<long, std::pair<int, int>> >, std::greater<std::pair<long, std::pair<int, int>>>> pq;
    pq.push(std::make_pair(1, std::make_pair(src.first, src.second)));
    while(!pq.empty()){
        std::pair<long, std::pair<int, int>> p = pq.top();
        pq.pop();
        int r = p.second.first;
        int c = p.second.second;
        if(r == dest.first && c == dest.second) break;
        std::vector<std::pair<int, int>> valid_dirs = _grid[r][c]->get_dirs();
        for(int i = 0; i<4; ++i){
            int nr = r+dr[i];
            int nc = c+dc[i];
            // && _grid[nr][nc]->get_label() == CellLabels::Tile
            if(is_valid_pos(nr, nc) && std::count(valid_dirs.begin(), valid_dirs.end(), std::make_pair(dr[i], dc[i]))){
                if(dists[r][c] + 1 < dists[nr][nc]){
                    dists[nr][nc] = dists[r][c] + 1;
                    preds[nr][nc] = std::make_pair(r, c);
                    //FIXME Look at this function, maybe make the beach effect easier
                    if((r == src.first && c == src.second && _grid[nr][nc]->get_label() == CellLabels::Tile) || _grid[r][c]->get_label() == CellLabels::Tile)
                        pq.push(std::make_pair(dists[nr][nc], std::make_pair(nr, nc)));
                }
            }
        }
    }
    // printf("Shortest Path: %ld\n", dists[dest.first][dest.second]);
    std::vector<std::pair<int, int>> path;
    if(dists[dest.first][dest.second] != INF){
        std::pair<int, int> pos = dest;
        while(pos != src){
            path.push_back(pos);
            pos = preds[pos.first][pos.second];
        }
        std::reverse(path.begin(), path.end());
    }
    return path;
}

std::pair<int, int> Grid::find_nearest_empty(std::pair<int, int> src, CellLabels label)
{
    int dr[] = {1, -1, 0, 0};
    int dc[] = {0, 0, 1, -1};
    std::vector<std::vector<bool>> vis(_n_rows, std::vector<bool>(_n_cols, false));
    std::stack<std::pair<int, int>> st;
    st.push(src);
    while(!st.empty()){
        std::pair<int, int> pos = st.top();
        int r = pos.first;
        int c = pos.second;
        if(_grid[r][c]->get_label() == label){
            //FIXME Look at this function, maybe apply occupied to a cell
            bool occupied = false;
            for(auto & agent : _agents)
                if(agent->get_row() == r && agent->get_col() == c){
                    occupied = true;
                    break;
                }
            if(!occupied)
                return pos;
        }
        st.pop();
        vis[r][c] = true;
        std::vector<std::pair<int, int>> valid_dirs = _grid[r][c]->get_dirs();
        for(int i = 0; i < 4; ++i){
            int nr = r+dr[i];
            int nc = c+dc[i];
            if(is_valid_pos(nr, nc) && !vis[nr][nc] && std::count(valid_dirs.begin(), valid_dirs.end(), std::make_pair(dr[i], dc[i])))
                st.push({nr, nc});
        }
    }
    return {-1, -1};
}

bool Grid::is_valid_pos(int row, int col)
{
    return row >= 0 && row < _grid.size() && col >= 0 && col < _grid[row].size();
}

std::vector<std::pair<int, int>> Grid::find_path(std::pair<int, int> src, std::pair<int, int> dest)
{
    if(!is_valid_pos(dest.first, dest.second) || !is_valid_pos(src.first, src.second))
        return { {-1, -1} };

    std::vector<std::pair<int, int>> path = dijkstras(src, dest);
    return path;
}

bool Grid::add_agent(std::unique_ptr<Agent> agent)
{
    _agents.push_back(std::move(agent));
    return true;
}

bool Grid::signal_agents()
{
    for(auto & agent : _agents){
        int row = agent->get_row();
        int col = agent->get_col();
        if(_grid[row][col]->get_label() == CellLabels::Idle){
            std::vector<std::pair<int, int>> path = find_path({row, col}, {0,0});
            agent->set_path(path);
        }else if(agent->get_path().empty() && _grid[row][col]->get_label() == CellLabels::Tile){
            //FIXME Consider concurrency issues with agent movement
            std::pair<int, int> nearest_idle = find_nearest_empty({row, col}, CellLabels::Idle);
            printf("%d %d\n", nearest_idle.first, nearest_idle.second);
            if(nearest_idle != std::make_pair(-1, -1)){
                std::vector<std::pair<int, int>> path = find_path({row, col}, nearest_idle);
                for(auto p : path)
                    printf("(%d %d) ", p.first, p.second);
                printf("\n");
                agent->set_path(path);
            }
        }
    }
    for(auto & agent : _agents)
        agent->move();
    return true;
}

bool Grid::set_cell(int row, int col, std::unique_ptr<Cell> cell)
{
    if(!is_valid_pos(row, col))
        return false;

    _grid[row][col] = std::move(cell);
    return true;
}

bool Grid::set_cell_block(int row, int col, int width, int height, CellLabels cell_type)
{
    for(int i = row; i < row + height; ++i){
        for(int j = col; j < col + width; ++j){
            if(is_valid_pos(i, j)){
                switch(cell_type){
                    case CellLabels::Tile:
                    {
                        set_cell(i, j, std::make_unique<TileCell>());
                        break;
                    }
                    case CellLabels::Idle:
                    {
                        set_cell(i, j, std::make_unique<IdleCell>());
                        break;
                    }
                    case CellLabels::Pickup:
                    {
                        set_cell(i, j, std::make_unique<PickupCell>());
                        break;
                    }
                    case CellLabels::Delivery:
                    {
                        set_cell(i, j, std::make_unique<DeliveryCell>());
                        break;
                    }
                    default:
                        return false;
                }
            }else
                return false;
        }
    }
    return true;
}

int Grid::get_n_rows()
{
    return _n_rows;
}

int Grid::get_n_cols()
{
    return _n_cols;
}

Cell * Grid::get_cell(int row, int col)
{
    if(!is_valid_pos(row, col))
        return nullptr;

    return _grid[row][col].get();
}

std::vector<std::pair<int, int>> Grid::get_agent_positions()
{
    std::vector<std::pair<int, int>> positions;
    for(auto & agent : _agents)
        positions.push_back(std::make_pair(agent->get_row(), agent->get_col()));
    return positions;
}