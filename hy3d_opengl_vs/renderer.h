#ifndef INCLUDE_RENDERED_H
#define INCLUDE_RENDERED_H

#include "core.h"
#include "math.h"
#include <glad/glad.h>

struct vertex
{
	vec3 pos;
	vec3 normal;
	//vec2 tex;
};

struct render_buffers
{
	vertex *vertices = 0;
	u32 *indices = 0;
	u32 nVertices = 0;
	u32 nIndices = 0;
	u32 VAO;
	u32 VBO;
	u32 EBO;

	//------------------------------------------------------------------------

	bool Initialize()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		if (indices)
			glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, GetVertexBufferSize(), vertices, GL_STATIC_DRAW);

		if (indices)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetIndexBufferSize(), indices, GL_STATIC_DRAW);
		}

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
		// vertex texture coords
		// glEnableVertexAttribArray(2);	
		// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);

		return true;
	}

	void Delete()
	{
		if (vertices) { free(vertices); }
		if (indices) { free(indices); }

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	inline u32 GetVertexBufferSize() { return (nVertices * sizeof(vertex)); }

	inline u32 GetIndexBufferSize() { return (nIndices * sizeof(u32)); }
};

#endif