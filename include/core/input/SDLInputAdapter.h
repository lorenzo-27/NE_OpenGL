#pragma once

#include "core/input/InputAdapter.h"
#include <SDL2/SDL.h>
#include <vector>

class SDLInputAdapter : public InputAdapter {
public:
	explicit SDLInputAdapter(SDL_Window* window);
	~SDLInputAdapter() override;

	void Initialize() override;
	void Cleanup() override;
	bool PollEvents() override;
	void UpdateInput(class Input& input) override;

private:
	SDL_Window* window;
	bool quitRequested = false;
	std::vector<SDL_Event> pendingEvents;
};