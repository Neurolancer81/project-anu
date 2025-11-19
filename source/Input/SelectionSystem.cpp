#include "SelectionSystem.h"
#include <UnigineGame.h>
#include <UnigineInput.h>
#include <UnigineWorld.h>
#include <UnigineComponentSystem.h>
#include <UniginePrimitives.h>
#include <UnigineMaterials.h>
#include <UnigineLog.h>
#include <UnigineConsole.h>
#include <UnigineEditor.h>
#include <UnigineRender.h>

using namespace Unigine;
using namespace Math;

SelectionSystem::SelectionSystem()
	: selectedUnit(nullptr)
	, selectionIndicator(nullptr)
	, previousMouseButtonState(false)
{
}

SelectionSystem::~SelectionSystem()
{
	clearSelection();
	if (selectionIndicator)
	{
		selectionIndicator->removeWorldChild(selectionIndicator);
		selectionIndicator = nullptr;
	}
}

void SelectionSystem::init()
{
	createSelectionIndicator();
	Log::message("SelectionSystem initialized\n");
}

void SelectionSystem::update()
{
	handleMouseInput();
	updateIndicatorPosition();
}

void SelectionSystem::clearSelection()
{
	selectedUnit = nullptr;
	if (selectionIndicator)
	{
		selectionIndicator->setEnabled(0);
	}
}

void SelectionSystem::createSelectionIndicator()
{
	// Create a thin cylinder (ring-like) to show selection
	// Radius: 0.5m, Height: 0.05m (thin disc)
	selectionIndicator = Primitives::createCylinder(0.5f, 0.05f);

	if (selectionIndicator)
	{
		// Get material and modify color (using same approach as GridRenderer)
		MaterialPtr material = selectionIndicator->getMaterial(0);
		if (material)
		{
			MaterialPtr instanceMaterial = material->inherit();
			instanceMaterial->setParameterFloat4("albedo_color", vec4(1.0f, 0.8f, 0.0f, 1.0f)); // Yellow/gold color
			selectionIndicator->setMaterial(instanceMaterial, 0);
		}

		// Initially hidden
		selectionIndicator->setEnabled(0);

		// Add to world hierarchy (CRITICAL - same as GridRenderer pattern)
		Vector<NodePtr> root_nodes;
		World::getRootNodes(root_nodes);
		if (root_nodes.size() > 0)
		{
			root_nodes[0]->addChild(selectionIndicator);
			Log::message("Selection indicator created and added to world (isWorld: %d)\n", selectionIndicator->isWorld());
		}
		else
		{
			Log::error("Failed to find world root nodes for selection indicator\n");
		}
	}
	else
	{
		Log::error("Failed to create selection indicator\n");
	}
}

void SelectionSystem::updateIndicatorPosition()
{
	if (selectedUnit && selectionIndicator)
	{
		// Get the unit's node
		NodePtr unitNode = selectedUnit->getNode();
		if (unitNode)
		{
			// Position indicator at unit's feet (slightly above ground)
			dvec3 unitPos = unitNode->getWorldPosition();
			unitPos.z = 0.05; // Just above ground level

			selectionIndicator->setWorldPosition(unitPos);
			selectionIndicator->setEnabled(1);
		}
	}
}

UnitComponent* SelectionSystem::raycastForUnit()
{
	// Get the active camera/player
	PlayerPtr player = Game::getPlayer();
	if (!player)
	{
		return nullptr;
	}

	// Get mouse position in screen space
	ivec2 mouse = Input::getMousePosition();

	// Get camera position
	dvec3 camera_pos = player->getWorldPosition();

	// Calculate ray direction from camera through mouse position
	// Use player's projection to get direction
	vec3 direction = player->getDirectionFromMainWindow(mouse.x, mouse.y);

	// Perform physics intersection
	ObjectPtr hit_object = World::getIntersection(camera_pos, camera_pos + dvec3(direction) * 1000.0, 1);

	if (hit_object)
	{
		NodePtr hitNode = hit_object;
		if (hitNode)
		{
			// Check if this node has a UnitComponent
			UnitComponent* unit = ComponentSystem::get()->getComponent<UnitComponent>(hitNode);
			if (unit)
			{
				return unit;
			}
		}
	}

	return nullptr;
}

void SelectionSystem::handleMouseInput()
{
	// Get current mouse button state
	bool currentMouseButtonState = Input::isMouseButtonDown(Input::MOUSE_BUTTON_LEFT);

	// Detect one-shot click (transition from up to down)
	if (currentMouseButtonState && !previousMouseButtonState)
	{
		// Mouse was just clicked
		UnitComponent* clickedUnit = raycastForUnit();

		if (clickedUnit)
		{
			// Select the unit
			selectedUnit = clickedUnit;
			Log::message("Unit selected: %s\n", clickedUnit->unit_name.get());
		}
		else
		{
			// Clicked on empty space - deselect
			clearSelection();
			Log::message("Selection cleared\n");
		}
	}

	// Store current state for next frame
	previousMouseButtonState = currentMouseButtonState;
}
