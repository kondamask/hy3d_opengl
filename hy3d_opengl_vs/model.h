#ifndef INCLUDE_MODEL_H
#define INCLUDE_MODEL_H

#include "core.h"
#include "math.h"

struct vertex
{
	vec3 pos;
	vec3 normal;
	//vec2 tex;
};

struct cylinder
{
	vertex *verts;
	u32 nVerts;

	//------------------------------------------------------------------------

	void Make(i32 stacks, i32 slices, f32 height, f32 radius);

	inline void Delete() { free(verts); }

	inline u32 BufferSize() { return (nVerts * sizeof(vertex)); }

};

struct model_part
{
	f32 modelView[16];
	model_part *sibling;
	model_part *child;
	void (*Draw)();
};

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

	DOG_PARTS_COUNT
};

static void Draw_DOG_TORSO();
static void Draw_DOG_NECK();
static void Draw_DOG_HEAD();
static void Draw_DOG_BACK_LEFT_UPPER_LEG();
static void Draw_DOG_BACK_LEFT_LOWER_LEG();
static void Draw_DOG_BACK_LEFT_FOOT();
static void Draw_DOG_BACK_RIGHT_UPPER_LEG();
static void Draw_DOG_BACK_RIGHT_LOWER_LEG();
static void Draw_DOG_BACK_RIGHT_FOOT();
static void Draw_DOG_FRONT_LEFT_UPPER_LEG();
static void Draw_DOG_FRONT_LEFT_LOWER_LEG();
static void Draw_DOG_FRONT_LEFT_FOOT();
static void Draw_DOG_FRONT_RIGHT_UPPER_LEG();
static void Draw_DOG_FRONT_RIGHT_LOWER_LEG();
static void Draw_DOG_FRONT_RIGHT_FOOT();
static void (*DrawDogFuncs[])() = {
	Draw_DOG_TORSO,
	Draw_DOG_NECK,
	Draw_DOG_HEAD,
	Draw_DOG_BACK_LEFT_UPPER_LEG,
	Draw_DOG_BACK_LEFT_LOWER_LEG,
	Draw_DOG_BACK_LEFT_FOOT,
	Draw_DOG_BACK_RIGHT_UPPER_LEG,
	Draw_DOG_BACK_RIGHT_LOWER_LEG,
	Draw_DOG_BACK_RIGHT_FOOT,
	Draw_DOG_FRONT_LEFT_UPPER_LEG,
	Draw_DOG_FRONT_LEFT_LOWER_LEG,
	Draw_DOG_FRONT_LEFT_FOOT,
	Draw_DOG_FRONT_RIGHT_UPPER_LEG,
	Draw_DOG_FRONT_RIGHT_LOWER_LEG,
	Draw_DOG_FRONT_RIGHT_FOOT
};

struct model
{
	model_part *parts;

	//------------------------------------------------------------------------
	
	void Draw();

private:
	void Traverse(model_part*);
};

#endif