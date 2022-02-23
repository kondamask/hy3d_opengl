#ifndef INCLUDE_MODEL_H
#define INCLUDE_MODEL_H

#include "core.h"
#include "math.h"
#include "shader.h"
#include "renderer.h"

struct mesh_part
{
	mat4 model;
	u32 firstVert;
	u32 firstIndex;
	u32 nVertices;
	u32 nIndices;
	mesh_part *sibling;
	mesh_part *child;
	void (*Draw)(ShaderProgram &shader, mesh_part &part);
};

struct mesh
{	
	mesh_part *parts = 0;
	render_buffers buffers;
	//mat4 model;
	
	//------------------------------------------------------------------------
	
	void Draw(ShaderProgram &shader);

	bool MakeCylinder(u32 stacks = 1, u32 slices = 20, f32 height = 1.0f, f32 radius = 1.0f);

	bool MakeSphere(u32 stacks = 25, u32 slices = 10, f32 radius = 1.0f);

	bool MakeDog();

	void Delete();

private:
	inline void Traverse(ShaderProgram &shader, mesh_part *p, mat4 &prevModel);
};

/* CUBE
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
*/

//------------------------------------------------------------------------
// DOG STUFF

enum DOG_PARTS
{
	DOG_TORSO,
	DOG_NECK,
	DOG_HEAD,
	DOG_BACK_LEFT_UPPER_LEG,
	DOG_BACK_LEFT_LOWER_LEG,
	DOG_BACK_LEFT_FOOT,
	DOG_BACK_RIGHT_UPPER_LEG,
	DOG_BACK_RIGHT_LOWER_LEG,
	DOG_BACK_RIGHT_FOOT,
	DOG_FRONT_LEFT_UPPER_LEG,
	DOG_FRONT_LEFT_LOWER_LEG,
	DOG_FRONT_LEFT_FOOT,
	DOG_FRONT_RIGHT_UPPER_LEG,
	DOG_FRONT_RIGHT_LOWER_LEG,
	DOG_FRONT_RIGHT_FOOT,
	DOG_TAIL_BASE,
	DOG_TAIL_END,

	DOG_PARTS_COUNT
};

static_func void Draw_DOG_TORSO(ShaderProgram &shader, mesh_part &part);
static_func void Draw_DOG_NECK(ShaderProgram &shader, mesh_part &part);
static_func void Draw_DOG_HEAD(ShaderProgram &shader, mesh_part &part);
static_func void Draw_DOG_BACK_LEFT_UPPER_LEG(ShaderProgram &shader, mesh_part &part);
static_func void Draw_DOG_BACK_LEFT_LOWER_LEG(ShaderProgram &shader, mesh_part &part);
static_func void Draw_DOG_BACK_LEFT_FOOT(ShaderProgram &shader, mesh_part &part);
static_func void Draw_DOG_BACK_RIGHT_UPPER_LEG(ShaderProgram &shader, mesh_part &part);
static_func void Draw_DOG_BACK_RIGHT_LOWER_LEG(ShaderProgram &shader, mesh_part &part);
static_func void Draw_DOG_BACK_RIGHT_FOOT(ShaderProgram &shader, mesh_part &part);
static_func void Draw_DOG_FRONT_LEFT_UPPER_LEG(ShaderProgram &shader, mesh_part &part);
static_func void Draw_DOG_FRONT_LEFT_LOWER_LEG(ShaderProgram &shader, mesh_part &part);
static_func void Draw_DOG_FRONT_LEFT_FOOT(ShaderProgram &shader, mesh_part &part);
static_func void Draw_DOG_FRONT_RIGHT_UPPER_LEG(ShaderProgram &shader, mesh_part &part);
static_func void Draw_DOG_FRONT_RIGHT_LOWER_LEG(ShaderProgram &shader, mesh_part &part);
static_func void Draw_DOG_FRONT_RIGHT_FOOT(ShaderProgram &shader, mesh_part &part);
static_func void Draw_DOG_TAIL_BASE(ShaderProgram &shader, mesh_part &part);
static_func void Draw_DOG_TAIL_END(ShaderProgram &shader, mesh_part &part);

enum DOG_ANIMATION
{
	DOG_ANIMATION_NONE = 0,
	
	DOG_ANIMATION_NECK = 1,
	DOG_ANIMATION_FRONT_LEFT_UP = 4,
	DOG_ANIMATION_STAND_ON_BACK_LEGS = 8,
	DOG_ANIMATION_RETURN_TO_NEUTRAL = 16,
};

global_var f32 curDogAnimDur[5] = {};
global_var f32 dogNeckAngle = -45.0f;
global_var f32 dogTailAngle = 0.0f;
global_var bool neckDown = false;
void AnimateDog(u32 &animations, f32 dt, f32 time);

#endif