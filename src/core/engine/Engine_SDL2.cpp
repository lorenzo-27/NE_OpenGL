#if not defined(_WIN32)
#include "core/engine/Engine.h"
#include "game/objects/base/Physical.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <cmath>
#include <iostream>
#include <algorithm>

// --- SDL2-specific code ------------------------------------------------

bool Engine::InitOSWrapper() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	atexit(SDL_Quit); // SDL will be shut down automatically on app exit
	return true;
}

void Engine::SetupInputs() {
	// not needed
}

// from https://github.com/g8kig/LiteXL-PPC
static double query_surface_scale(SDL_Window *window) {
	int w_pixels, h_pixels;
	int w_points, h_points;
	SDL_GL_GetDrawableSize(window, &w_pixels, &h_pixels);
	SDL_GetWindowSize(window, &w_points, &h_points);
	return double(w_pixels) / double(w_points);
}

void Engine::ToggleFullscreen() {
	if (isFullscreen) {
		iWidth = GH_SCREEN_WIDTH;
		iHeight = GH_SCREEN_HEIGHT;
		SDL_SetWindowFullscreen(window, 0);
		double scale = query_surface_scale(window);
		SDL_SetWindowSize(window, int(iWidth / scale), int(iHeight / scale));
	} else {
		double scale = query_surface_scale(window);
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		SDL_GetWindowSize(window, &iWidth, &iHeight);
		iWidth = int(iWidth * scale);
		iHeight = int(iHeight * scale);
		SDL_SetWindowSize(window, iWidth, iHeight);
	}
	isFullscreen = !isFullscreen;
}

void Engine::CreateGLWindow() {
	iWidth = GH_SCREEN_WIDTH;
	iHeight = GH_SCREEN_HEIGHT;
	window = SDL_CreateWindow(
			GH_TITLE,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			iWidth, iHeight,
			SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | 0
	);
	double scale = query_surface_scale(window);
	SDL_SetWindowSize(window, int(iWidth / scale), int(iHeight / scale));
	SDL_SetWindowGrab(window, SDL_TRUE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_CaptureMouse(SDL_TRUE);
	if (!window) {
		SDL_Log("Unable to create GL window: %s", SDL_GetError());
		return;
	}

	// window successfully created

	glContext = SDL_GL_CreateContext(window);
	if (!glContext) {
		SDL_Log("Unable to create GL context: %s", SDL_GetError());
		return;
	}

	// GL context is created and is current for the calling thread

	isWindowGood = true;
}

void Engine::DestroyGLWindow() {
	if (glContext) {
		SDL_GL_DeleteContext(glContext);
	}
	if (window) {
		SDL_DestroyWindow(window);
	}
}

void Engine::EnableVSync() {
	// try adaptive vsync first, if not supported, go full vsync
	if (SDL_GL_SetSwapInterval(-1) != 0) {
		SDL_GL_SetSwapInterval(1);
	}
}

int Engine::EnterMessageLoop() {
	// Setup the timer
	ticks_per_step = timer.SecondsToTicks(GH_DT);
	int64_t cur_ticks = timer.GetTicks();
	GH_FRAME = 0;

	// Game loop
	while (true) {
		// Poll events using adapter - exits loop if false returned (quit requested)
		if (!input.PollEvents()) {
			break;
		}

		// Process special keys
		if (input.key_press['q']) {
			break; // Exit the game loop
		} else if (input.key_press['r']) {
			CheckForShaderUpdates(true);
			std::cout << "Shader reloaded\n";
		} else if (input.key_press['f']) {
			ToggleFullscreen();
		} else if (input.key_press['1']) {
			LoadScene("l1-doubleTunnel");
		} else if (input.key_press['2']) {
			LoadScene("l2-slope");
		} else if (input.key_press['3']) {
			LoadScene("l3-scale");
		} else if (input.key_press['4']) {
			LoadScene("l4-doubleSlope");
		} else if (input.key_press['5']) {
			LoadScene("l5-puzzle");
		}

		PeriodicRender(cur_ticks);
		SDL_GL_SwapWindow(window);

		input.EndFrame();
	}

	return 0;
}

void Engine::ConfineCursor() {
	//not needed
}
#endif