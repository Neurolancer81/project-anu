// GridCell.h
// Individual grid cell with position, elevation, and occupancy

#pragma once

#include <UniginePtr.h>
#include <UnigineNode.h>

// Grid position (x, y on grid, z = elevation level)
struct GridPosition {
    int x;
    int y;
    int z; // Elevation level (0 = ground, 1 = 5ft up, 2 = 10ft up, etc.)

    GridPosition() : x(0), y(0), z(0) {}
    GridPosition(int _x, int _y, int _z = 0) : x(_x), y(_y), z(_z) {}

    bool operator==(const GridPosition& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const GridPosition& other) const {
        return !(*this == other);
    }
};

// Grid cell properties
struct GridCell {
    GridPosition position;
    int elevation;              // Height in 5ft increments (matches position.z)
    bool blocked;               // Is this cell passable?
    Unigine::NodePtr occupant;  // Unit currently on this cell (nullptr if empty)

    GridCell() : elevation(0), blocked(false) {}
    GridCell(GridPosition pos) : position(pos), elevation(pos.z), blocked(false) {}

    bool isOccupied() const { return occupant != nullptr; }
};
