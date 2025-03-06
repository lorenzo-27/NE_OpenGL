#pragma once

#include "core/input/InputAdapter.h"

#if defined(_WIN32)
#include <Windows.h>

class WindowsInputAdapter : public InputAdapter {
public:
	WindowsInputAdapter(HWND hWnd);
	~WindowsInputAdapter() override;

	void Initialize() override;
	void Cleanup() override;
	bool PollEvents() override;
	void UpdateInput(class Input& input) override;

	// Windows-specific event handler
	void ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND hWnd;
	bool quitRequested = false;
	Input* cachedInput = nullptr;  // Cache for messages between polls
};
#endif