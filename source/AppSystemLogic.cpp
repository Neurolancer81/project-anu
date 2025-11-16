#include "AppSystemLogic.h"
#include <UnigineComponentSystem.h>
#include <UnigineLog.h>

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

	Log::message("AppSystemLogic::init() - Complete\n");
	return 1;
}

////////////////////////////////////////////////////////////////////////////////
// start of the main loop
////////////////////////////////////////////////////////////////////////////////

int AppSystemLogic::update()
{
	// Write here code to be called before updating each render frame.
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
