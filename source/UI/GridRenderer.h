// GridRenderer.h
// Renders the tactical grid visually in the world

#pragma once

#include "../Grid/GridSystem.h"
#include "../Components/GridConfigComponent.h"
#include <UnigineVector.h>
#include <UniginePtr.h>
#include <UnigineNode.h>
#include <UnigineMathLib.h>

namespace Unigine {
    class ObjectMeshDynamic;
}

class GridRenderer {
public:
    GridRenderer(GridSystem* grid_system, GridConfigComponent* config);
    ~GridRenderer();

    // Grid visualization
    void createGridVisuals();
    void destroyGridVisuals();
    void updateGridVisuals();

    // Cell highlighting (for movement range, targeting, etc.)
    void highlightCell(GridPosition pos, Unigine::Math::vec4 color);
    void highlightCells(const Unigine::Vector<GridPosition>& cells, Unigine::Math::vec4 color);
    void clearHighlights();

    // Visibility
    void show();
    void hide();
    bool isVisible() const { return visible; }

    // Grid-to-world coordinate conversion
    Unigine::Math::dvec3 gridToWorld(int x, int y, int elevation = 0) const;
    Unigine::Math::dvec3 gridToWorld(GridPosition pos) const;

private:
    GridSystem* grid;
    GridConfigComponent* config;  // Configuration (cell size, colors, etc.)
    bool visible;

    // Visual nodes
    Unigine::NodePtr grid_root;                     // Parent node for all grid visuals
    Unigine::Vector<Unigine::NodePtr> cell_nodes;   // One node per grid cell
    Unigine::Vector<Unigine::NodePtr> highlight_nodes; // Highlight overlays

    // Helper: Create a single grid cell mesh
    Unigine::NodePtr createCellMesh(int x, int y, int elevation);

    // Helper: Create highlight overlay for a cell
    Unigine::NodePtr createHighlightMesh(GridPosition pos, Unigine::Math::vec4 color);
};
