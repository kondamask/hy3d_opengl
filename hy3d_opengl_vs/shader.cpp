#include "shader.h"

bool ShaderProgram::Create(shader_info *shaders, u32 nShaders)
{
	// Must at least provide vertex and fragment shader.
	if (nShaders < 2)
		return false;

	id = glCreateProgram();

	for (u32 iShader = 0; iShader < nShaders; iShader++)
	{
		shader_info &curShader = shaders[iShader];
		
		std::ifstream shaderFile(curShader.path);
		if (shaderFile.fail())
		{
			DebugPrint("ERROR: Could not open " << curShader.path << std::endl);
			return false;
		}

		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();

		std::string shaderString = shaderStream.str();
		const char *shaderCode = shaderString.c_str();

		curShader.id = glCreateShader(curShader.type);
		glShaderSource(curShader.id, 1, &shaderCode, 0);

#if DEBUG_MODE
		int success;
		glCompileShader(curShader.id);
		glGetShaderiv(curShader.id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(curShader.id, 512, NULL, infoLog);
			DebugPrint("ERROR: Compilation Failed " << curShader.path << std::endl << infoLog << std::endl);
			return false;
		};
#endif

		glAttachShader(id, curShader.id);
	}

	glLinkProgram(id);

#if DEBUG_MODE
	int success;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		DebugPrint("ERROR: Linking Failed\n" << infoLog << std::endl);
		return false;
	}
#endif
	for (u32 iShader = 0; iShader < nShaders; iShader++)
		glDeleteShader(shaders[iShader].id);

	return true;
}

void ShaderProgram::SetUniform(UNIFORM_TYPE type, const char *varName, void *data, u32 count)
{
	int uniformLocation = glGetUniformLocation(id, varName);
	switch (type)
	{
		case UNIFORM_TYPE::VEC3:
		{
			glUniform3fv(uniformLocation, count, (f32 *)data);
		} break;
		
		case UNIFORM_TYPE::VEC4:
		{
			glUniform4fv(uniformLocation, count, (f32 *)data);
		} break;
		
		case UNIFORM_TYPE::MAT4:
		{
			glUniformMatrix4fv(uniformLocation, count, GL_FALSE, (f32 *)data);
		} break;
	}
}