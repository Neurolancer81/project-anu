// GridConfigComponent.h
// Configuration component for the tactical grid system
// Place this on a "GridConfig" dummy node in the Editor to configure grid parameters

#pragma once

#include <UnigineComponentSystem.h>

class GridConfigComponent : public Unigine::ComponentBase {
public:
    COMPONENT_DEFINE(GridConfigComponent, Unigine::ComponentBase);
    COMPONENT_INIT(init);
    COMPONENT_UPDATE(update);
    COMPONENT_SHUTDOWN(shutdown);

    // Grid dimensions
    PROP_PARAM(Int, grid_width, 20);     // Number of cells in X direction
    PROP_PARAM(Int, grid_height, 20);    // Number of cells in Y direction

    // Cell sizing
    PROP_PARAM(Float, cell_size, 1.0f);          // Meters per cell (1m = PF2e 5ft square)
    PROP_PARAM(Float, elevation_height, 1.0f);   // Meters per elevation level

    // Visual rendering
    PROP_PARAM(Float, tile_coverage, 0.9f);      // 0.9 = tiles cover 90%, leaving 10% gaps
    PROP_PARAM(Float, tile_height_offset, 0.1f); // How far above ground to render (meters)
    PROP_PARAM(Float, tile_thickness, 0.01f);    // Thickness of grid tiles (meters)

    // Grid colors (RGBA)
    PROP_PARAM(Vec4, grid_color, Unigine::Math::vec4(0.4f, 0.4f, 0.4f, 0.7f)); // Semi-transparent gray
    PROP_PARAM(Vec4, highlight_color, Unigine::Math::vec4(0.0f, 0.8f, 1.0f, 0.8f)); // Cyan for highlights

protected:
    void init();
    void update();
    void shutdown();
};
