#pragma once

#if defined(WIN32)
	#include <winuser.h>
#endif

class InputAdapter;

class Input {
public:
	Input();
	~Input();

	void SetAdapter(InputAdapter* adapter);
	void EndFrame();
	bool PollEvents();

#if defined(_WIN32)
	void UpdateRaw(const tagRAWINPUT* raw);
#endif
#if not defined(_WIN32)
	void UpdateRaw(unsigned state, int mouse_x, int mouse_y);
#endif

	// Keyboard
	bool key[256]{};
	bool key_press[256]{};

	// Mouse
	bool mouse_button[3]{};
	bool mouse_button_press[3]{};
	float mouse_dx{};
	float mouse_dy{};
	float mouse_ddx{};
	float mouse_ddy{};

private:
	InputAdapter* adapter = nullptr;
};