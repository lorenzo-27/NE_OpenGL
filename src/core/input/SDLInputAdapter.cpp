#if not defined(_WIN32)
#include "core/input/SDLInputAdapter.h"
#include "core/input/Input.h"
#include "core/engine/GameHeader.h"

SDLInputAdapter::SDLInputAdapter(SDL_Window* window) : window(window) {}

SDLInputAdapter::~SDLInputAdapter() {
	Cleanup();
}

void SDLInputAdapter::Initialize() {
	SDL_SetWindowGrab(window, SDL_TRUE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_CaptureMouse(SDL_TRUE);
}

void SDLInputAdapter::Cleanup() {
	SDL_SetRelativeMouseMode(SDL_FALSE);
	SDL_SetWindowGrab(window, SDL_FALSE);
	SDL_CaptureMouse(SDL_FALSE);
}

bool SDLInputAdapter::PollEvents() {
	// Store events in our event queue for later processing in UpdateInput
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			quitRequested = true;
			break;
		}

		// Store the event for processing in UpdateInput
		pendingEvents.push_back(event);
	}

	return !quitRequested;
}

void SDLInputAdapter::UpdateInput(Input& input) {
	// Process all pending events that we stored in PollEvents
	for (const SDL_Event& event : pendingEvents) {
		switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.repeat == 0) {
					SDL_Keycode keycode = event.key.keysym.sym;

					// Handle special keys
					if (keycode == SDLK_q) {
						input.key_press['q'] = true;
						input.key['q'] = true;
					} else if (keycode == SDLK_r) {
						input.key_press['r'] = true;
						input.key['r'] = true;
					} else if (keycode == SDLK_f) {
						input.key_press['f'] = true;
						input.key['f'] = true;
					} else if (keycode == SDLK_w) {
						input.key_press['W'] = true;
						input.key['W'] = true;
					} else if (keycode == SDLK_a) {
						input.key_press['A'] = true;
						input.key['A'] = true;
					} else if (keycode == SDLK_s) {
						input.key_press['S'] = true;
						input.key['S'] = true;
					} else if (keycode == SDLK_d) {
						input.key_press['D'] = true;
						input.key['D'] = true;
					} else if (keycode >= SDLK_1 && keycode <= SDLK_5) {
						// Handle number keys 1-5
						char numChar = '1' + (keycode - SDLK_1);
						input.key_press[numChar] = true;
						input.key[numChar] = true;
					} else {
						// Convert any other keys to uppercase for consistent handling
						if (keycode >= 'a' && keycode <= 'z') {
							char upperKey = toupper(keycode);
							input.key_press[upperKey] = true;
							input.key[upperKey] = true;
						}
					}
				}
				break;

			case SDL_KEYUP:
			{
				SDL_Keycode keycode = event.key.keysym.sym;

				// Handle special keys
				if (keycode == SDLK_q) {
					input.key['q'] = false;
				} else if (keycode == SDLK_r) {
					input.key['r'] = false;
				} else if (keycode == SDLK_f) {
					input.key['f'] = false;
				} else if (keycode == SDLK_w) {
					input.key['W'] = false;
				} else if (keycode == SDLK_a) {
					input.key['A'] = false;
				} else if (keycode == SDLK_s) {
					input.key['S'] = false;
				} else if (keycode == SDLK_d) {
					input.key['D'] = false;
				} else if (keycode >= SDLK_1 && keycode <= SDLK_5) {
					// Handle number keys 1-5
					char numChar = '1' + (keycode - SDLK_1);
					input.key[numChar] = false;
				} else {
					// Convert any other keys to uppercase for consistent handling
					if (keycode >= 'a' && keycode <= 'z') {
						char upperKey = toupper(keycode);
						input.key[upperKey] = false;
					}
				}
			}
				break;

			case SDL_MOUSEMOTION:
				input.UpdateRaw(event.motion.state, event.motion.xrel, event.motion.yrel);
				break;
		}
	}

	// Clear the event queue now that we've processed them
	pendingEvents.clear();
}
#endif