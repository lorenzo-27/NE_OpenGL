#pragma once

class InputAdapter {
public:
	virtual ~InputAdapter() = default;

	virtual void Initialize() = 0;
	virtual void Cleanup() = 0;
	virtual bool PollEvents() = 0;  // Returns false if quit requested
	virtual void UpdateInput(class Input& input) = 0;
};