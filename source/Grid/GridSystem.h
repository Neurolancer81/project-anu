// GridSystem.h
// Manages the tactical grid, cell queries, and occupancy

#pragma once

#include "GridCell.h"
#include <UnigineVector.h>
#include <UniginePtr.h>

namespace Unigine {
    class Node;
}

class GridSystem {
public:
    GridSystem(int width, int height);
    ~GridSystem();

    // Grid queries
    GridCell* getCell(int x, int y);
    GridCell* getCell(GridPosition pos);
    bool isValidPosition(int x, int y) const;
    bool isValidPosition(GridPosition pos) const;
    bool isBlocked(GridPosition pos) const;

    // Grid dimensions
    int getWidth() const { return grid_width; }
    int getHeight() const { return grid_height; }

    // Grid state modification
    void setElevation(int x, int y, int elevation);
    void setBlocked(int x, int y, bool blocked);
    void setOccupant(GridPosition pos, Unigine::NodePtr unit);
    void clearOccupant(GridPosition pos);

    // Distance calculations (PF2e 5-5-10 diagonal rule)
    int getDistance(GridPosition a, GridPosition b) const;
    int getElevationDifference(GridPosition a, GridPosition b) const;

private:
    int grid_width;
    int grid_height;
    Unigine::Vector<GridCell> cells; // Flat array: index = y * width + x

    // Helper: convert 2D coords to 1D index
    int getIndex(int x, int y) const;
};
