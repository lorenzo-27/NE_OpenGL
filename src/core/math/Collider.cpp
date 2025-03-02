#include "core/math/Collider.h"
#include "core/engine/GameHeader.h"
#include "resources/Resources.h"
#include "GL/glew.h"
#include <cassert>
#include <iostream>

Collider::Collider(const Vector3 &a, const Vector3 &b, const Vector3 &c) {
	const Vector3 ab = b - a;
	const Vector3 bc = c - b;
	const Vector3 ca = a - c;
	const float magAB = ab.MagSq();
	const float magBC = bc.MagSq();
	const float magCA = ca.MagSq();
	if (magAB >= magBC && magAB >= magCA) {
		CreateSorted(bc * 0.5f, (a + b) * 0.5f, ca * 0.5f);
	} else if (magBC >= magAB && magBC >= magCA) {
		CreateSorted(ca * 0.5f, (b + c) * 0.5f, ab * 0.5f);
	} else {
		CreateSorted(ab * 0.5f, (c + a) * 0.5f, bc * 0.5f);
	}
}

bool Collider::Collide(const Matrix4 &localToUnit, Vector3 &delta) const {
	//Get world delta
	const Matrix4 local = localToUnit * mat;
	const Vector3 v = -local.Translation();

	//Get axes
	const Vector3 x = local.XAxis();
	const Vector3 y = local.YAxis();

	//Find the closest point
	const float px = GH_CLAMP(v.Dot(x) / x.MagSq(), -1.0f, 1.0f);
	const float py = GH_CLAMP(v.Dot(y) / y.MagSq(), -1.0f, 1.0f);
	const Vector3 closest = x * px + y * py;

	//Calculate distance to the closest point
	delta = v - closest;
	if (delta.MagSq() >= 1.0f) {
		return false;
	} else {
		delta = delta.Normalized() - delta;
		return true;
	}
}

void Collider::DebugDraw(const Camera &cam, const Matrix4 &objMat) const {
	static GLuint vao = 0;
	static GLuint vbo = 0;
	static GLuint shader = 0;

	// Initialize shader and buffers if needed (first call)
	if (shader == 0) {
		// Simple shader for debug lines
		shader = AcquireShader("debug")->GetProgram();

		// Create buffers
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// Define vertex attribute layout
		glEnableVertexAttribArray(0);  // position
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4), (void*)0);

		// Pre-allocate buffer for square vertices
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vector4), nullptr, GL_DYNAMIC_DRAW);
	}

	// Calculate transformation matrix
	const Matrix4 mvp = cam.Matrix() * objMat * mat;

	// Define square vertices
	const Vector4 vertices[4] = {
			mvp * Vector4(1, 1, 0, 1),
			mvp * Vector4(1, -1, 0, 1),
			mvp * Vector4(-1, -1, 0, 1),
			mvp * Vector4(-1, 1, 0, 1)
	};

	// Use our shader
	glUseProgram(shader);

	// Set color uniform (green)
	GLint colorLoc = glGetUniformLocation(shader, "color");
	if (colorLoc >= 0) {
		glUniform3f(colorLoc, 0.0f, 1.0f, 0.0f);
	}

	// Depth test configuration
	glDepthFunc(GL_ALWAYS);

	// Update buffer with new vertices
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

	// Draw the square outline
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	// Restore depth test setting
	glDepthFunc(GL_LESS);
}

void Collider::CreateSorted(const Vector3 &da, const Vector3 &c, const Vector3 &db) {
	assert(std::abs(da.Dot(db)) / (da.Mag() * db.Mag()) < 0.001f);
	mat.MakeIdentity();
	mat.SetTranslation(c);
	mat.SetXAxis(da);
	mat.SetYAxis(db);
}
