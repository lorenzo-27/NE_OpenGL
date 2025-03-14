#include "core/input/Input.h"
#include "core/input/InputAdapter.h"
#include "core/engine/GameHeader.h"

#if defined(_WIN32)
#include <Windows.h>
#endif
#if not defined(_WIN32)
#include <SDL2/SDL.h>
#endif

#include <memory>

Input::Input() {
	memset(this, 0, sizeof(Input));
}

Input::~Input() {
	// Adapter is owned by the Engine
}

void Input::SetAdapter(InputAdapter* adapter) {
	this->adapter = adapter;
}

bool Input::PollEvents() {
	if (adapter) {
		bool shouldContinue = adapter->PollEvents();
		adapter->UpdateInput(*this);
		return shouldContinue;
	}
	return true;
}

void Input::EndFrame() {
	memset(key_press, 0, sizeof(key_press));
	memset(mouse_button_press, 0, sizeof(mouse_button_press));
	mouse_dx = mouse_dx * GH_MOUSE_SMOOTH + mouse_ddx * (1.0f - GH_MOUSE_SMOOTH);
	mouse_dy = mouse_dy * GH_MOUSE_SMOOTH + mouse_ddy * (1.0f - GH_MOUSE_SMOOTH);
	mouse_ddx = 0.0f;
	mouse_ddy = 0.0f;
}

#if defined(_WIN32)

void Input::UpdateRaw(const tagRAWINPUT* raw) {
    if (raw->header.dwType == RIM_TYPEMOUSE) {
        if (raw->data.mouse.usFlags == MOUSE_MOVE_RELATIVE) {
            mouse_ddx += raw->data.mouse.lLastX;
            mouse_ddy += raw->data.mouse.lLastY;
        }
        if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
            mouse_button[0] = true;
            mouse_button_press[0] = true;
        }
        if (raw->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) {
            mouse_button[1] = true;
            mouse_button_press[1] = true;
        }
        if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) {
            mouse_button[2] = true;
            mouse_button_press[2] = true;
        }
        if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) mouse_button[0] = false;
        if (raw->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) mouse_button[1] = false;
        if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) mouse_button[2] = false;
    }
}

#endif

#if not defined(_WIN32)

void Input::UpdateRaw(unsigned state, int mouse_x, int mouse_y) {
	mouse_dx = static_cast<float>(mouse_x);
	mouse_dy = static_cast<float>(mouse_y);
	mouse_ddx += mouse_dx;
	mouse_ddy += mouse_dy;
	if (state & SDL_BUTTON_LMASK) {
		mouse_button[0] = true;
		mouse_button_press[0] = true;
	}
	if (state & SDL_BUTTON_MMASK) {
		mouse_button[1] = true;
		mouse_button_press[1] = true;
	}
	if (state & SDL_BUTTON_RMASK) {
		mouse_button[2] = true;
		mouse_button_press[2] = true;
	}
}

#endif