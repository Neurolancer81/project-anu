// GridSystem.cpp
#include "GridSystem.h"
#include <UnigineLog.h>
#include <UnigineNode.h>
#include <cmath>

GridSystem::GridSystem(int width, int height)
    : grid_width(width)
    , grid_height(height)
{
    Unigine::Log::message("GridSystem::GridSystem() - Creating %dx%d grid\n", width, height);

    // Allocate cells (flat array for cache efficiency)
    cells.resize(width * height);

    // Initialize cells
    for (int y = 0; y < grid_height; y++) {
        for (int x = 0; x < grid_width; x++) {
            int index = getIndex(x, y);
            cells[index] = GridCell(GridPosition(x, y, 0)); // All start at ground level
        }
    }

    Unigine::Log::message("GridSystem::GridSystem() - %d cells initialized\n", cells.size());
}

GridSystem::~GridSystem() {
    Unigine::Log::message("GridSystem::~GridSystem() - Destroying grid\n");
}

int GridSystem::getIndex(int x, int y) const {
    return y * grid_width + x;
}

GridCell* GridSystem::getCell(int x, int y) {
    if (!isValidPosition(x, y)) {
        return nullptr;
    }
    return &cells[getIndex(x, y)];
}

GridCell* GridSystem::getCell(GridPosition pos) {
    return getCell(pos.x, pos.y);
}

bool GridSystem::isValidPosition(int x, int y) const {
    return x >= 0 && x < grid_width && y >= 0 && y < grid_height;
}

bool GridSystem::isValidPosition(GridPosition pos) const {
    return isValidPosition(pos.x, pos.y);
}

bool GridSystem::isBlocked(GridPosition pos) const {
    if (!isValidPosition(pos)) {
        return true; // Out of bounds = blocked
    }

    const GridCell* cell = const_cast<GridSystem*>(this)->getCell(pos);
    if (!cell) return true;

    return cell->blocked || cell->isOccupied();
}

void GridSystem::setElevation(int x, int y, int elevation) {
    GridCell* cell = getCell(x, y);
    if (cell) {
        cell->elevation = elevation;
        cell->position.z = elevation;
    }
}

void GridSystem::setBlocked(int x, int y, bool blocked) {
    GridCell* cell = getCell(x, y);
    if (cell) {
        cell->blocked = blocked;
    }
}

void GridSystem::setOccupant(GridPosition pos, Unigine::NodePtr unit) {
    GridCell* cell = getCell(pos);
    if (cell) {
        cell->occupant = unit;
    }
}

void GridSystem::clearOccupant(GridPosition pos) {
    GridCell* cell = getCell(pos);
    if (cell) {
        cell->occupant = nullptr;
    }
}

int GridSystem::getElevationDifference(GridPosition a, GridPosition b) const {
    return b.z - a.z;
}

int GridSystem::getDistance(GridPosition a, GridPosition b) const {
    // PF2e 5-5-10 diagonal rule
    // Simplified version: straight-line distance approximation
    // Full implementation will alternate diagonal costs in pathfinding

    int dx = abs(b.x - a.x);
    int dy = abs(b.y - a.y);

    // Diagonal + straight movement
    int diagonal = (dx < dy) ? dx : dy;
    int straight = abs(dx - dy);

    // Each diagonal costs 1.5 on average (alternating 1/2)
    // For now, approximate as: diagonals + straights
    return diagonal + straight;
}
