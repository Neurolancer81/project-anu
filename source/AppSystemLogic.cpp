#include "AppSystemLogic.h"
#include <UnigineComponentSystem.h>
#include <UnigineLog.h>
#include <UnigineInput.h>
#include <UnigineConsole.h>
#include <UnigineGui.h>

using namespace Unigine;

// System logic, it exists during the application life cycle.
// These methods are called right after corresponding system script's (UnigineScript) methods.

AppSystemLogic::AppSystemLogic()
{
}

AppSystemLogic::~AppSystemLogic()
{
}

int AppSystemLogic::init()
{
	Log::message("AppSystemLogic::init() - Initializing component system...\n");

	// Initialize C++ component system
	// Components with REGISTER_COMPONENT() macro register automatically
	ComponentSystem::get()->initialize();

	// Configure mouse cursor for tactical game (not first-person)
	// Make cursor visible and not grabbed/locked
	Input::setMouseGrab(false);  // false = cursor not grabbed
	Console::run("app_cursor_show 1");  // Show cursor
	Log::message("AppSystemLogic::init() - Mouse cursor enabled (tactical mode)\n");

	Log::message("AppSystemLogic::init() - Complete\n");
	return 1;
}

////////////////////////////////////////////////////////////////////////////////
// start of the main loop
////////////////////////////////////////////////////////////////////////////////

int AppSystemLogic::update()
{
	// Tactical game mode: continuously ensure mouse cursor remains free
	// NOTE: Unigine's player/camera systems may re-grab the mouse, so we must check every frame
	// This is intentional behavior for turn-based tactical games (vs. first-person games)
	if (Input::isMouseGrab())
	{
		Input::setMouseGrab(false);              // Disable mouse grab/lock
		Gui::getCurrent()->setMouseEnabled(true); // Ensure GUI can receive mouse events
	}
	return 1;
}

int AppSystemLogic::postUpdate()
{
	// Write here code to be called after updating each render frame.
	return 1;
}

////////////////////////////////////////////////////////////////////////////////
// end of the main loop
////////////////////////////////////////////////////////////////////////////////

int AppSystemLogic::shutdown()
{
	// Write here code to be called on engine shutdown.
	return 1;
}
