#ifndef INCLUDE_MODEL_H
#define INCLUDE_MODEL_H

#include "core.h"
#include "math.h"
#include "shader.h"

struct vertex
{
	vec3 pos;
	vec3 normal;
	//vec2 tex;
};
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

class mesh
{	
public:
	void Draw(ShaderProgram &shader);

	bool MakeCylinder(u32 stacks = 1, u32 slices = 20, f32 height = 1.0f, f32 radius = 1.0f);

	bool MakeSphere(u32 stacks = 25, u32 slices = 25, f32 radius = 1.0f);

	bool MakeDog();

	void Delete();
	
	inline u32 GetVertexBufferSize() { return (nVertices * sizeof(vertex)); }

	inline u32 GetIndexBufferSize() { return (nIndices * sizeof(u32)); }

private:
	mesh_part *parts = 0;
	vertex *vertices = 0;
	u32 *indices = 0;
	u32 nVertices = 0;
	u32 nIndices = 0;
	vec3 pos = {};
	u32 VAO;
	u32 VBO;
	u32 EBO;
	
	inline void GLSetUp();

	inline void Traverse(ShaderProgram &shader, mesh_part *p, mat4 &prevModel);

	static_func void DivideTriangle(vec3 a, vec3 b, vec3 c, int reps);
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

	DOG_PARTS_COUNT
};

static void Draw_DOG_TORSO(ShaderProgram &shader, mesh_part &part);
static void Draw_DOG_NECK(ShaderProgram &shader, mesh_part &part);
static void Draw_DOG_HEAD(ShaderProgram &shader, mesh_part &part);
static void Draw_DOG_BACK_LEFT_UPPER_LEG(ShaderProgram &shader, mesh_part &part);
static void Draw_DOG_BACK_LEFT_LOWER_LEG(ShaderProgram &shader, mesh_part &part);
static void Draw_DOG_BACK_LEFT_FOOT(ShaderProgram &shader, mesh_part &part);
static void Draw_DOG_BACK_RIGHT_UPPER_LEG(ShaderProgram &shader, mesh_part &part);
static void Draw_DOG_BACK_RIGHT_LOWER_LEG(ShaderProgram &shader, mesh_part &part);
static void Draw_DOG_BACK_RIGHT_FOOT(ShaderProgram &shader, mesh_part &part);
static void Draw_DOG_FRONT_LEFT_UPPER_LEG(ShaderProgram &shader, mesh_part &part);
static void Draw_DOG_FRONT_LEFT_LOWER_LEG(ShaderProgram &shader, mesh_part &part);
static void Draw_DOG_FRONT_LEFT_FOOT(ShaderProgram &shader, mesh_part &part);
static void Draw_DOG_FRONT_RIGHT_UPPER_LEG(ShaderProgram &shader, mesh_part &part);
static void Draw_DOG_FRONT_RIGHT_LOWER_LEG(ShaderProgram &shader, mesh_part &part);
static void Draw_DOG_FRONT_RIGHT_FOOT(ShaderProgram &shader, mesh_part &part);

#endif