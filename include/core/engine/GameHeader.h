#pragma once

#include <cstdint>

//Windows
static constexpr char GH_TITLE[] = "Non Euclidean OpenGL";
static constexpr char GH_CLASS[] = "NE_OpenGL";

//General
static constexpr float GH_PI = 3.141592653589793f;
static constexpr int GH_MAX_PORTALS = 16;

//Graphics
static constexpr bool GH_START_FULLSCREEN = false;
static constexpr bool GH_HIDE_MOUSE = true;
static constexpr bool GH_USE_SKY = true;
static constexpr int GH_SCREEN_WIDTH = 1280;
static constexpr int GH_SCREEN_HEIGHT = 720;
static constexpr int GH_SCREEN_X = 50;
static constexpr int GH_SCREEN_Y = 50;
static constexpr float GH_FOV = 90.0f;
static constexpr float GH_NEAR_MIN = 1e-3f;
static constexpr float GH_NEAR_MAX = 1e-1f;
static constexpr float GH_FAR = 100.0f;
static constexpr int GH_FBO_SIZE = 2048;
static constexpr int GH_MAX_RECURSION = 4;

//Gameplay
static constexpr float GH_MOUSE_SENSITIVITY = 0.005f;
static constexpr float GH_MOUSE_SMOOTH = 0.5f;
static constexpr float GH_WALK_SPEED = 2.9f;
static constexpr float GH_WALK_ACCEL = 50.0f;
static constexpr float GH_BOB_FREQ = 8.0f;
static constexpr float GH_BOB_OFFS = 0.015f;
static constexpr float GH_BOB_DAMP = 0.04f;
static constexpr float GH_BOB_MIN = 0.1f;
static constexpr float GH_DT = 0.002f;
static constexpr int GH_MAX_STEPS = 30;
static constexpr float GH_PLAYER_HEIGHT = 1.5f;
static constexpr float GH_PLAYER_RADIUS = 0.2f;
static constexpr float GH_GRAVITY = -9.8f;

//Global variables
class Engine;

class Input;

class Player;

extern Engine *GH_ENGINE;
extern Player *GH_PLAYER;
extern const Input *GH_INPUT;
extern int GH_REC_LEVEL;
extern int64_t GH_FRAME;

//Functions
template<class T>
inline T GH_CLAMP(T a, T mn, T mx) {
	return a < mn ? mn : (a > mx ? mx : a);
}

template<class T>
inline T GH_MIN(T a, T b) {
	return a < b ? a : b;
}

template<class T>
inline T GH_MAX(T a, T b) {
	return a > b ? a : b;
}
