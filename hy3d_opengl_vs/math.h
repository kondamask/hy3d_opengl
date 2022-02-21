#ifndef INCLUDE_MATH_H
#define INCLUDE_MATH_H

#include "HandmadeMath.h"

#define VEC3_UP { 0.0f, 1.0f, 0.0f }
#define VEC3_DOWN { 0.0f, -1.0f, 0.0f }
#define VEC3_RIGHT { 1.0f, 0.0f, 0.0f }
#define VEC3_LEFT { -1.0f, 1.0f, 0.0f }
#define VEC3_FRONT { 0.0f, 0.0f, 1.0f }
#define VEC3_BACK { 0.0f, 0.0f, -1.0f }

typedef hmm_vec2 vec2;
typedef hmm_vec3 vec3;
typedef hmm_vec4 vec4;
typedef hmm_mat4 mat4;
typedef hmm_quaternion quart;

#endif