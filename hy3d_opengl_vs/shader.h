#ifndef INCLUDE_SHADER_H
#define INCLUDE_SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

#include "core.h"

enum class UNIFORM_TYPE
{
	BOOL,
	INT,
	FLOAT,
	VEC2,
	VEC3,
	VEC4,
	MAT4
};

struct shader_info
{
	const char *path;
	u32 type;
	u32 id; // this is filles by the program. not the user
};

struct ShaderProgram
{
	u32 id;
	
	//------------------------------------------------------------------------

	bool Create(shader_info *shaders, u32 nShaders);
	
	void SetUniform(UNIFORM_TYPE type, const char *varName, void *val, u32 count = 1);
	
	inline void Use() { glUseProgram(id); }
	
	inline void Delete() { glDeleteProgram(id); }
};

#endif