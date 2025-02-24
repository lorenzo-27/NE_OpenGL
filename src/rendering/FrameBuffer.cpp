#include "rendering/FrameBuffer.h"
#include "core/engine/GameHeader.h"
#include "core/engine/Engine.h"
#include <iostream>

bool FrameBuffer::HasDSASupport() {
	return GLEW_ARB_direct_state_access != 0;
}

bool FrameBuffer::CheckFramebufferStatus(GLuint framebuffer) {
	GLenum status;
	if (HasDSASupport()) {
		status = glCheckNamedFramebufferStatus(framebuffer, GL_FRAMEBUFFER);
	} else {
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);
		status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
	return status == GL_FRAMEBUFFER_COMPLETE;
}

FrameBuffer::FrameBuffer() {
	if (HasDSASupport()) {
		// Create and configure texture using DSA
		glCreateTextures(GL_TEXTURE_2D, 1, &texId);
		glTextureParameteri(texId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(texId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(texId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(texId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureStorage2D(texId, 1, GL_RGB8, GH_FBO_SIZE, GH_FBO_SIZE);

		// Create and configure framebuffer using DSA
		glCreateFramebuffers(1, &fbo);
		glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, texId, 0);

		// Create and configure renderbuffer using DSA
		glCreateRenderbuffers(1, &renderBuf);
		glNamedRenderbufferStorage(renderBuf, GL_DEPTH_COMPONENT16, GH_FBO_SIZE, GH_FBO_SIZE);
		glNamedFramebufferRenderbuffer(fbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuf);
	} else {
		// Legacy path (existing code)
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GH_FBO_SIZE, GH_FBO_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

		glGenFramebuffersEXT(1, &fbo);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texId, 0);

		glGenRenderbuffersEXT(1, &renderBuf);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderBuf);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT16, GH_FBO_SIZE, GH_FBO_SIZE);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, renderBuf);
	}

	if (!CheckFramebufferStatus(fbo)) {
		std::cerr << "Framebuffer is not complete!" << std::endl;
		// Handle error appropriately
	}
}

FrameBuffer::~FrameBuffer() {
	if (HasDSASupport()) {
		glDeleteTextures(1, &texId);
		glDeleteFramebuffers(1, &fbo);
		glDeleteRenderbuffers(1, &renderBuf);
	} else {
		glDeleteFramebuffersEXT(1, &fbo);
		glDeleteRenderbuffersEXT(1, &renderBuf);
		glDeleteTextures(1, &texId);
	}
}

void FrameBuffer::Use() const {
	glBindTexture(GL_TEXTURE_2D, texId);
}

void FrameBuffer::Render(const Camera &cam, GLuint curFBO, const Portal *skipPortal) const {
	if (HasDSASupport()) {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	} else {
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	}

	glViewport(0, 0, GH_FBO_SIZE, GH_FBO_SIZE);
	GH_ENGINE->Render(cam, fbo, skipPortal);

	if (HasDSASupport()) {
		glBindFramebuffer(GL_FRAMEBUFFER, curFBO);
	} else {
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, curFBO);
	}
}
