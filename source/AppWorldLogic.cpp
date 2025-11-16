#include "AppWorldLogic.h"

#include <UnigineLog.h>
#include <UnigineGame.h>

// World logic, it takes effect only when the world is loaded.
// These methods are called right after corresponding world script's (UnigineScript) methods.

AppWorldLogic::AppWorldLogic()
	: game(nullptr)
{}

AppWorldLogic::~AppWorldLogic()
{}

int AppWorldLogic::init()
{
	Unigine::Log::message("AppWorldLogic::init() - Creating GameManager...\n");

	// Create and initialize game manager
	game = new GameManager();
	game->init();

	Unigine::Log::message("AppWorldLogic::init() - Complete\n");
	return 1;
}

////////////////////////////////////////////////////////////////////////////////
// start of the main loop
////////////////////////////////////////////////////////////////////////////////

int AppWorldLogic::update()
{
	// Per-frame update: handle input
	// Delegate to GameManager
	game->handleInput();

	return 1;
}

int AppWorldLogic::postUpdate()
{
	// The engine calls this function after updating each render frame: correct behavior after the state of the node has been updated.
	return 1;
}

int AppWorldLogic::updatePhysics()
{
	// Fixed 60 FPS update: game logic
	// Delegate to GameManager
	float dt = Unigine::Game::getIFps(); // Inverse FPS (delta time)
	game->update(dt);

	// WARNING: do not create, delete or change transformations of nodes here, because rendering is already in progress.
	return 1;
}

////////////////////////////////////////////////////////////////////////////////
// end of the main loop
////////////////////////////////////////////////////////////////////////////////

int AppWorldLogic::shutdown()
{
	Unigine::Log::message("AppWorldLogic::shutdown() - Destroying GameManager...\n");

	// Delete game manager (which will clean up all game systems)
	delete game;
	game = nullptr;

	Unigine::Log::message("AppWorldLogic::shutdown() - Complete\n");
	return 1;
}

int AppWorldLogic::save(const Unigine::StreamPtr &stream)
{
	// Write here code to be called when the world is saving its state (i.e. state_save is called): save custom user data to a file.
	UNIGINE_UNUSED(stream);
	return 1;
}

int AppWorldLogic::restore(const Unigine::StreamPtr &stream)
{
	// Write here code to be called when the world is restoring its state (i.e. state_restore is called): restore custom user data to a file here.
	UNIGINE_UNUSED(stream);
	return 1;
}
