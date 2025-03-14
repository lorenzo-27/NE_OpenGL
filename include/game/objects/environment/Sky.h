#pragma once

#include "resources/Resources.h"
#include "core/math/Vector.h"

class Sky {
public:
	Sky() {
		mesh = AcquireMesh("quad.obj");
		shader = AcquireShader("sky");
	}

	void Draw(const Camera &cam) const {
		glDepthMask(GL_FALSE);
		const Matrix4 mvp = cam.projection.Inverse();
		const Matrix4 mv = cam.worldView.Inverse();
		shader->Use();
		shader->SetMVP(mvp.m, mv.m);
		mesh->Draw();
		glDepthMask(GL_TRUE);
	}

private:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Shader> shader;
};
