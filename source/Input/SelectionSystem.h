#ifndef SELECTIONSYSTEM_H
#define SELECTIONSYSTEM_H

#include <UnigineNode.h>
#include <UnigineGame.h>
#include <UnigineObjects.h>
#include "../Components/UnitComponent.h"

namespace Unigine
{
	class SelectionSystem
	{
	public:
		SelectionSystem();
		~SelectionSystem();

		// Initialize the selection system
		void init();

		// Update each frame - handles input and selection logic
		void update();

		// Get the currently selected unit
		UnitComponent* getSelectedUnit() const { return selectedUnit; }

		// Check if a unit is selected
		bool hasSelection() const { return selectedUnit != nullptr; }

		// Clear current selection
		void clearSelection();

	private:
		// Currently selected unit
		UnitComponent* selectedUnit;

		// Visual selection indicator (ring around unit)
		ObjectMeshDynamicPtr selectionIndicator;

		// Previous mouse button state for one-shot detection
		bool previousMouseButtonState;

		// Create the visual selection indicator
		void createSelectionIndicator();

		// Update indicator position to follow selected unit
		void updateIndicatorPosition();

		// Perform raycasting to detect unit under mouse
		UnitComponent* raycastForUnit();

		// Handle mouse click for selection
		void handleMouseInput();
	};
}

#endif // SELECTIONSYSTEM_H
