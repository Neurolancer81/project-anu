// GridConfigComponent.cpp
#include "GridConfigComponent.h"
#include <UnigineLog.h>

REGISTER_COMPONENT(GridConfigComponent);

void GridConfigComponent::init() {
    Unigine::Log::message("GridConfigComponent::init() - Grid: %dx%d, Cell: %.2fm, Elevation: %.2fm\n",
        grid_width, grid_height, cell_size, elevation_height);
}

void GridConfigComponent::update() {
    // No per-frame updates needed - this is just config data
}

void GridConfigComponent::shutdown() {
    Unigine::Log::message("GridConfigComponent::shutdown()\n");
}
