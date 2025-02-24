#pragma once

#include "core/camera/Camera.h"
#include <GL/glew.h>

// Forward declaration
class Portal;

class FrameBuffer {
public:
	FrameBuffer();

	~FrameBuffer();

	void Render(const Camera &cam, GLuint curFBO, const Portal *skipPortal) const;

	void Use() const;

	// Delete copy constructor and assignment operator
	FrameBuffer(const FrameBuffer &) = delete;

	FrameBuffer &operator=(const FrameBuffer &) = delete;

private:
	GLuint texId{};
	GLuint fbo{};
	GLuint renderBuf{};

	// Helper function to check if DSA is available
	static bool HasDSASupport();

	// Helper function to check framebuffer status
	static bool CheckFramebufferStatus(GLuint framebuffer);
};