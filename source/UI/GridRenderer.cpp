// GridRenderer.cpp
#include "GridRenderer.h"
#include <UnigineLog.h>
#include <UnigineWorld.h>
#include <UnigineObjects.h>
#include <UniginePrimitives.h>
#include <UnigineMaterials.h>
#include <UnigineGUID.h>

#include "UnigineNodes.h"

using namespace Unigine;
using namespace Unigine::Math;

GridRenderer::GridRenderer(GridSystem* grid_system, GridConfigComponent* grid_config)
    : grid(grid_system)
    , config(grid_config)
    , visible(true)
{
    Log::message("GridRenderer::GridRenderer() - Created with config: cell_size=%.2fm, coverage=%.0f%%\n",
        config->cell_size, config->tile_coverage * 100.0f);
}

GridRenderer::~GridRenderer() {
    destroyGridVisuals();
}

void GridRenderer::createGridVisuals() {
    Log::message("GridRenderer::createGridVisuals() - Creating grid visuals for %dx%d grid\n",
        grid->getWidth(), grid->getHeight());

    // Create root node to hold all grid visuals
    grid_root = NodeDummy::create();
    grid_root->setName("GridRoot");
    grid_root->setWorldPosition(dvec3(0, 0, 0));
    grid_root->setEnabled(true);

    // CRITICAL: Add to world hierarchy by getting a root node and adding as child
    Vector<NodePtr> root_nodes;
    World::getRootNodes(root_nodes);
    if (root_nodes.size() > 0) {
        root_nodes[0]->addChild(grid_root);
        Log::message("GridRenderer::createGridVisuals() - Added grid_root as child of world root: %s\n",
            root_nodes[0]->getName());
    } else {
        Log::error("GridRenderer::createGridVisuals() - No world root nodes found!\n");
    }

    Log::message("GridRenderer::createGridVisuals() - Created grid_root (ID: %d, Enabled: %d)\n",
        grid_root->getID(), grid_root->isEnabled());
    Log::message("GridRenderer::createGridVisuals() - grid_root parent: %s, isWorld: %d\n",
        grid_root->getParent() ? grid_root->getParent()->getName() : "NULL",
        grid_root->isWorld());

    // Reserve space for cell nodes
    cell_nodes.reserve(grid->getWidth() * grid->getHeight());

    // Create visual for each grid cell
    for (int y = 0; y < grid->getHeight(); y++) {
        for (int x = 0; x < grid->getWidth(); x++) {
            GridCell* cell = grid->getCell(x, y);
            if (cell) {
                NodePtr cell_node = createCellMesh(x, y, cell->elevation);
                cell_nodes.append(cell_node);
            }
        }
    }

    Log::message("GridRenderer::createGridVisuals() - Created %d cell visuals\n", cell_nodes.size());
    Log::message("GridRenderer::createGridVisuals() - GridRoot has %d children\n", grid_root->getNumChildren());
}

void GridRenderer::destroyGridVisuals() {
    // Clear highlights
    clearHighlights();

    // Clear cell nodes
    cell_nodes.clear();

    // Delete root node (this will delete all children)
    if (grid_root) {
        grid_root.deleteLater();
        grid_root.clear();
    }

    Log::message("GridRenderer::destroyGridVisuals() - Destroyed grid visuals\n");
}

void GridRenderer::updateGridVisuals() {
    // TODO: Update cell colors based on grid state (blocked, occupied, etc.)
}

void GridRenderer::highlightCell(GridPosition pos, vec4 color) {
    if (!grid->isValidPosition(pos)) return;

    // Use config highlight color if no color specified (vec4(0,0,0,0))
    vec4 use_color = (color.x == 0 && color.y == 0 && color.z == 0 && color.w == 0)
        ? config->highlight_color
        : color;

    NodePtr highlight = createHighlightMesh(pos, use_color);
    if (highlight) {
        highlight_nodes.append(highlight);
    }
}

void GridRenderer::highlightCells(const Vector<GridPosition>& cells, vec4 color) {
    for (int i = 0; i < cells.size(); i++) {
        highlightCell(cells[i], color);
    }
}

void GridRenderer::clearHighlights() {
    for (int i = 0; i < highlight_nodes.size(); i++) {
        if (highlight_nodes[i]) {
            highlight_nodes[i].deleteLater();
        }
    }
    highlight_nodes.clear();
}

void GridRenderer::show() {
    if (grid_root) {
        grid_root->setEnabled(1);
        visible = true;
    }
}

void GridRenderer::hide() {
    if (grid_root) {
        grid_root->setEnabled(0);
        visible = false;
    }
}

dvec3 GridRenderer::gridToWorld(int x, int y, int elevation) const {
    // Grid origin is at world (0, 0, 0)
    // X axis = world X, Y axis = world Y, elevation = world Z
    double world_x = x * config->cell_size;
    double world_y = y * config->cell_size;
    double world_z = elevation * config->elevation_height;

    return dvec3(world_x, world_y, world_z);
}

dvec3 GridRenderer::gridToWorld(GridPosition pos) const {
    return gridToWorld(pos.x, pos.y, pos.z);
}

NodePtr GridRenderer::createCellMesh(int x, int y, int elevation) {
    // Use Unigine's built-in box primitive instead of manual mesh creation
    dvec3 world_pos = gridToWorld(x, y, elevation);

    // Create a flat box (size based on config tile_coverage to leave gaps)
    float tile_size = config->cell_size * config->tile_coverage;
    ObjectMeshDynamicPtr mesh = Primitives::createBox(vec3(tile_size, tile_size, config->tile_thickness));
    mesh->setName(String::format("GridCell_%d_%d", x, y));

    // Position it at the grid cell location (height based on config)
    mesh->setWorldPosition(world_pos + dvec3(0, 0, config->tile_height_offset));

    // Get and modify the material (color from config)
    MaterialPtr material = mesh->getMaterial(0);
    if (material) {
        material = material->inherit();
        material->setParameterFloat4("albedo_color", config->grid_color);
        mesh->setMaterial(material, 0);
    }

    // Parent to grid root
    if (grid_root) {
        mesh->setParent(grid_root);
    }

    // Debug: Log first cell creation
    if (x == 0 && y == 0) {
        Log::message("GridRenderer::createCellMesh() - First cell: pos=(%.1f,%.1f,%.1f), size=%.2fm\n",
            mesh->getWorldPosition().x, mesh->getWorldPosition().y, mesh->getWorldPosition().z, tile_size);
    }

    return NodePtr(mesh);
}

NodePtr GridRenderer::createHighlightMesh(GridPosition pos, vec4 color) {
    // Similar to createCellMesh but slightly elevated with custom color
    dvec3 world_pos = gridToWorld(pos);

    // Create a flat box (same size as grid cell for highlighting)
    float tile_size = config->cell_size * config->tile_coverage;
    ObjectMeshDynamicPtr mesh = Primitives::createBox(vec3(tile_size, tile_size, config->tile_thickness));
    mesh->setName(String::format("Highlight_%d_%d_%d", pos.x, pos.y, pos.z));

    // Position slightly above the grid tiles
    mesh->setWorldPosition(world_pos + dvec3(0, 0, config->tile_height_offset + 0.02));

    // Get and modify the material with highlight color
    MaterialPtr material = mesh->getMaterial(0);
    if (material) {
        material = material->inherit();
        material->setParameterFloat4("albedo_color", color);
        mesh->setMaterial(material, 0);
    }

    // Parent to grid root
    if (grid_root) {
        mesh->setParent(grid_root);
    }

    return NodePtr(mesh);
}
