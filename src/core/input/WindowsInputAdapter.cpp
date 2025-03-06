#include "core/input/WindowsInputAdapter.h"
#include "core/input/Input.h"

#if defined(_WIN32)
WindowsInputAdapter::WindowsInputAdapter(HWND hWnd) : hWnd(hWnd) {}

WindowsInputAdapter::~WindowsInputAdapter() {
	Cleanup();
}

void WindowsInputAdapter::Initialize() {
	// Register raw input devices
	RAWINPUTDEVICE Rid[3];

	// Mouse
	Rid[0].usUsagePage = 0x01;  // HID_USAGE_PAGE_GENERIC
	Rid[0].usUsage = 0x02;      // HID_USAGE_GENERIC_MOUSE
	Rid[0].dwFlags = RIDEV_INPUTSINK;
	Rid[0].hwndTarget = hWnd;

	// Joystick
	Rid[1].usUsagePage = 0x01;  // HID_USAGE_PAGE_GENERIC
	Rid[1].usUsage = 0x04;      // HID_USAGE_GENERIC_JOYSTICK
	Rid[1].dwFlags = 0;
	Rid[1].hwndTarget = 0;

	// Gamepad
	Rid[2].usUsagePage = 0x01;  // HID_USAGE_PAGE_GENERIC
	Rid[2].usUsage = 0x05;      // HID_USAGE_GENERIC_GAMEPAD
	Rid[2].dwFlags = 0;
	Rid[2].hwndTarget = 0;

	RegisterRawInputDevices(Rid, 3, sizeof(Rid[0]));
}

void WindowsInputAdapter::Cleanup() {
	// Nothing specific needed for Windows
}

bool WindowsInputAdapter::PollEvents() {
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			quitRequested = true;
			break;
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return !quitRequested;
}

void WindowsInputAdapter::UpdateInput(Input& input) {
	cachedInput = &input;
	// Input state is updated via ProcessMessage when messages are dispatched
}

void WindowsInputAdapter::ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (!cachedInput) return;

	switch (uMsg) {
		case WM_KEYDOWN:
			if (!(lParam & 0x40000000)) { // Ignore repeats
				cachedInput->key[wParam & 0xFF] = true;
				cachedInput->key_press[wParam & 0xFF] = true;
			}
			break;

		case WM_KEYUP:
			cachedInput->key[wParam & 0xFF] = false;
			break;

		case WM_INPUT:
		{
			UINT dwSize = sizeof(BYTE) * 256;
			BYTE lpb[256];
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
			cachedInput->UpdateRaw((const RAWINPUT*)lpb);
		}
			break;
	}
}
#endif