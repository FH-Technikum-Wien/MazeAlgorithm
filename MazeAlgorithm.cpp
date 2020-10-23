#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

/*
* https://www.wikiwand.com/en/A*_search_algorithm
*
* f -> Estimated cost from start to goal through cell.
* g -> Cost from start to cell.
*/

#define uintPair std::pair<unsigned int, unsigned int>

// Definition for path and wall character
const char PATH = '.';
const char WALL = '#';

// Maze size
const unsigned int WIDTH = 41;
const unsigned int HEIGHT = 41;
char maze[WIDTH * HEIGHT];

// Start and finish
const uintPair startCell(1, 1);
const uintPair endCell(39, 39);

struct Cell
{
public:
    // Position of this cell.
    uintPair pos;
    // Estimated cost to goal through cell.
    unsigned int f = 0;

    Cell(uintPair pos, unsigned int f = std::numeric_limits<unsigned int>::max()) : pos(pos), f(f) {}
};

void showPath(std::map<uintPair, uintPair> &cameFrom, uintPair last) {
    uintPair current = last;
    unsigned int pos = (current.second * WIDTH) + current.first;
    maze[pos] = 'o';
    while (cameFrom.count(current))
    {
        current = cameFrom[current];
        pos = (current.second * WIDTH) + current.first;
        maze[pos] = 'o';
    }

    int index = 0;
    for (char element : maze)
    {
        std::cout << element;
        index++;
        if (index == WIDTH) {
            index = 0;
            std::cout << "\n";
        }
    }
}


int main()
{
    // Read input and convert to array.
    std::for_each(std::begin(maze), std::end(maze), [](char& mazeData) {std::cin >> mazeData; });

    std::vector<Cell> openList;

    // Add start cell.
    openList.push_back(Cell(startCell, 0));
    std::push_heap(openList.begin(), openList.end(), [](Cell first, Cell second) { return first.f > second.f; });

    std::map<uintPair, uintPair> cameFrom;

    // Map of costs from start to cells (Cells here defined with position in maze).
    std::map<uintPair, unsigned int> gScores;
    gScores[uintPair{ 1,1 }] = 1;

    while (!openList.empty())
    {
        // Create heap.
        std::make_heap(openList.begin(), openList.end(), [](Cell first, Cell second) { return first.f > second.f; });
        // Sort by f.
        std::sort_heap(openList.begin(), openList.end(), [](Cell first, Cell second) { return first.f > second.f; });

        // Front == smallest 'f'.
        Cell front = openList.back();
        uintPair current = uintPair{front.pos.first, front.pos.second};

        // Reached goal.
        if (current.first == endCell.first && current.second == endCell.second) {
            showPath(cameFrom, current);
            return 0;
        }

        // Remove from open-list.
        openList.pop_back();

        // Go through all neighbors and find new cells to move to.
        for (int y = -1; y <= 1; y++)
        {
            for (int x = -1; x <= 1; x++)
            {
                // Skip self.
                if (x == 0 && y == 0)
                    continue;

                // Due to border, no need to check if out of bounds.
                int pos = ((current.second + y) * WIDTH) + current.first + x;

                if (maze[pos] == WALL)
                    continue;


                uintPair neighbor = uintPair{current.first + x, current.second + y };
                unsigned int newGScore = gScores[current] + std::abs(x) + std::abs(y);

                // New shorter path to goal with neighbor found.
                if (gScores.find(neighbor) == gScores.end() || newGScore < gScores[neighbor]) {
                    // Update new g.
                    gScores[neighbor] = newGScore;
                    // Calculate f using Manhattan-Distance.
                    unsigned int f = newGScore + (endCell.first - x) + (endCell.second - y);

                    cameFrom[neighbor] = current;

                    // Add to openList.
                    openList.push_back(Cell(neighbor, f));
                }
            }
        }
    }
    std::cout << "COULD NOT FIND PATH!";
    return -1;

}
