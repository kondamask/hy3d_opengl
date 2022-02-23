#include "model.h"

#include "glad/glad.h"

bool mesh::MakeCylinder(u32 stacks, u32 slices, f32 height, f32 radius)
{
	if (parts || vertices || indices)
		return false;

	nVertices = stacks * (slices + 1) * 2;
	vertices = ArrAlloc(vertex, nVertices);

	float stackStep = height / stacks;
	float sliceStep = 2.0f * HMM_PI32 / slices;

	u32 iVert = 0;
	for (u32 iStack = 0; iStack < stacks; ++iStack)
	{
		f32 z0 = 0.5f * height - iStack * stackStep;
		f32 z1 = z0 - stackStep;

		for (u32 iSlice = 0; iSlice <= slices; ++iSlice)
		{
			f32 theta = iSlice * sliceStep;

			f32 x = radius * cos(theta);
			f32 y = radius * sin(theta);

			vertices[iVert].pos = { x, y, z0 };
			vertices[iVert].normal = { x / radius, y / radius, 0.0 };
			//glTexCoord2f(j / (GLfloat) numMinor, i / (GLfloat) numMajor);
			iVert++;

			vertices[iVert].pos = { x, y, z1 };
			vertices[iVert].normal = { x / radius, y / radius, 0.0 };
			//glTexCoord2f(j / (GLfloat) numMinor, (i + 1) / (GLfloat) numMajor);
			iVert++;
		}
	}

	return true;
}

bool mesh::MakeSphere(u32 stacks, u32 slices, f32 radius)
{
	if (parts || vertices || indices)
		return false;

	nVertices = (stacks + 1)* (slices + 1);
	vertices = ArrAlloc(vertex, nVertices);

	u32 iVert = 0;
	for (u32 y = 0; y <= stacks; ++y)
	{
		for (u32 x = 0; x <= slices; ++x)
		{
			f32 iSlice = (float)x / (float)slices;
			f32 iStack = (float)y / (float)stacks;
			vec3 pos = {
				CosF(iSlice * 2 * HMM_PI32) * SinF(iStack * HMM_PI32),
				CosF(iStack * HMM_PI32),
				SinF(iSlice * 2 * HMM_PI32) * SinF(iStack * HMM_PI32)
			};

			vertices[iVert].pos = pos * radius;
			vertices[iVert].normal = pos;
			iVert++;
		}
	}

	nIndices = stacks * slices * 6;
	indices = ArrAlloc(u32, nIndices);

	u32 iInd = 0;
	bool oddRow = false;
	for (u32 y = 0; y < stacks; ++y)
	{
		for (u32 x = 0; x < slices; ++x)
		{
			indices[iInd] = (y + 1) * (slices + 1) + x;
			indices[iInd + 1] = y * (slices + 1) + x;
			indices[iInd + 2] = y * (slices + 1) + x + 1;

			indices[iInd + 3] = (y + 1) * (slices + 1) + x;
			indices[iInd + 4] = y * (slices + 1) + x + 1;
			indices[iInd + 5] = (y + 1) * (slices + 1) + x + 1;

			iInd += 6;
		}
	}

	return true;
}

bool mesh::MakeDog()
{
	if (parts || vertices || indices)
		return false;

	// Allocate a parts array
	parts = ArrAlloc(mesh_part, DOG_PARTS_COUNT);
	memset(parts, 0, sizeof(mesh_part) * DOG_PARTS_COUNT);

	// Create vertices and copy the to our buffers
	mesh sphere = {};
	sphere.MakeSphere();

	mesh cylinder = {};
	cylinder.MakeCylinder();

	nVertices = sphere.nVertices + cylinder.nVertices;
	nIndices = sphere.nIndices;
	vertices = ArrAlloc(vertex, nVertices);
	indices = ArrAlloc(u32, nIndices);

	memcpy(vertices, sphere.vertices, sphere.GetVertexBufferSize());
	memcpy(vertices + sphere.nVertices, cylinder.vertices, cylinder.GetVertexBufferSize());
	memcpy(indices, sphere.indices, sphere.GetIndexBufferSize()); // Only the sphere has indices	

	// Create tree
	parts[DOG_TORSO].child = &parts[DOG_NECK];
	parts[DOG_NECK].child = &parts[DOG_HEAD];

	parts[DOG_NECK].sibling = &parts[DOG_FRONT_LEFT_UPPER_LEG];
	parts[DOG_FRONT_LEFT_UPPER_LEG].sibling = &parts[DOG_FRONT_RIGHT_UPPER_LEG];
	parts[DOG_FRONT_RIGHT_UPPER_LEG].sibling = &parts[DOG_BACK_LEFT_UPPER_LEG];
	parts[DOG_BACK_LEFT_UPPER_LEG].sibling = &parts[DOG_BACK_RIGHT_UPPER_LEG];
	parts[DOG_BACK_RIGHT_UPPER_LEG].sibling = &parts[DOG_TAIL_BASE];
	
	parts[DOG_FRONT_LEFT_UPPER_LEG].child = &parts[DOG_FRONT_LEFT_LOWER_LEG];
	parts[DOG_FRONT_RIGHT_UPPER_LEG].child = &parts[DOG_FRONT_RIGHT_LOWER_LEG];
	parts[DOG_BACK_LEFT_UPPER_LEG].child = &parts[DOG_BACK_LEFT_LOWER_LEG];
	parts[DOG_BACK_RIGHT_UPPER_LEG].child = &parts[DOG_BACK_RIGHT_LOWER_LEG];

	parts[DOG_FRONT_LEFT_LOWER_LEG].child = &parts[DOG_FRONT_LEFT_FOOT];
	parts[DOG_FRONT_RIGHT_LOWER_LEG].child = &parts[DOG_FRONT_RIGHT_FOOT];
	parts[DOG_BACK_LEFT_LOWER_LEG].child = &parts[DOG_BACK_LEFT_FOOT];
	parts[DOG_BACK_RIGHT_LOWER_LEG].child = &parts[DOG_BACK_RIGHT_FOOT];
	
	parts[DOG_TAIL_BASE].child = &parts[DOG_TAIL_END];

	// Set Draw functions
	parts[DOG_TORSO].Draw = Draw_DOG_TORSO;
	parts[DOG_NECK].Draw = Draw_DOG_NECK;
	parts[DOG_HEAD].Draw = Draw_DOG_HEAD;
	parts[DOG_BACK_LEFT_UPPER_LEG].Draw = Draw_DOG_BACK_LEFT_UPPER_LEG;
	parts[DOG_BACK_LEFT_LOWER_LEG].Draw = Draw_DOG_BACK_LEFT_LOWER_LEG;
	parts[DOG_BACK_LEFT_FOOT].Draw = Draw_DOG_BACK_LEFT_FOOT;
	parts[DOG_BACK_RIGHT_UPPER_LEG].Draw = Draw_DOG_BACK_RIGHT_UPPER_LEG;
	parts[DOG_BACK_RIGHT_LOWER_LEG].Draw = Draw_DOG_BACK_RIGHT_LOWER_LEG;
	parts[DOG_BACK_RIGHT_FOOT].Draw = Draw_DOG_BACK_RIGHT_FOOT;
	parts[DOG_FRONT_LEFT_UPPER_LEG].Draw = Draw_DOG_FRONT_LEFT_UPPER_LEG;
	parts[DOG_FRONT_LEFT_LOWER_LEG].Draw = Draw_DOG_FRONT_LEFT_LOWER_LEG;
	parts[DOG_FRONT_LEFT_FOOT].Draw = Draw_DOG_FRONT_LEFT_FOOT;
	parts[DOG_FRONT_RIGHT_UPPER_LEG].Draw = Draw_DOG_FRONT_RIGHT_UPPER_LEG;
	parts[DOG_FRONT_RIGHT_LOWER_LEG].Draw = Draw_DOG_FRONT_RIGHT_LOWER_LEG;
	parts[DOG_FRONT_RIGHT_FOOT].Draw = Draw_DOG_FRONT_RIGHT_FOOT;
	parts[DOG_TAIL_BASE].Draw = Draw_DOG_TAIL_BASE;
	parts[DOG_TAIL_END].Draw = Draw_DOG_TAIL_END;

	// Set vertices locations
	for (u32 i = 0; i < DOG_PARTS_COUNT; i++)
	{
		parts[i].firstVert = sphere.nVertices;
		parts[i].nVertices = cylinder.nVertices;
	}
	parts[DOG_HEAD].firstVert = 0;
	parts[DOG_HEAD].firstIndex = 0;
	parts[DOG_HEAD].nVertices = sphere.nVertices;
	parts[DOG_HEAD].nIndices = sphere.nIndices;

	sphere.Delete();
	cylinder.Delete();

	GLSetUp();

	return true;
}

void mesh::GLSetUp()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, GetVertexBufferSize(), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetIndexBufferSize(), &indices[0], GL_STATIC_DRAW);

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
}

void mesh::Delete()
{
	if (vertices) { free(vertices); }
	if (indices) { free(indices); }
	if (parts) { free(parts); }

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void mesh::Draw(ShaderProgram &shader)
{
	glBindVertexArray(VAO);
	mat4 identity = Mat4d(1.0f);
	Traverse(shader, &parts[0], identity);
	glBindVertexArray(0);
}

void mesh::Traverse(ShaderProgram &shader, mesh_part *p, mat4 &parentModel)
{
	// NOTE: ONLY TRANSLATIONS AND ROTATIONS ARE APLIED TO CHILDREN.
	// NOT SCALINGS.
	if (p)
	{
		p->model = parentModel;
		p->Draw(shader, *p);
		if (p->child)
			Traverse(shader, p->child, p->model);
		if (p->sibling)
			Traverse(shader, p->sibling, parentModel);
	}
}

static_func void Draw_DOG_TORSO(ShaderProgram &shader, mesh_part &part)
{
	mat4 trans = Multiply(part.model, Scale( { 0.3f, 0.3f, 1.5f }));
	shader.SetUniform(UNIFORM_TYPE::MAT4, "model", &trans);
	glDrawArrays(GL_TRIANGLE_STRIP, part.firstVert, part.nVertices);
}

static_func void Draw_DOG_NECK(ShaderProgram &shader, mesh_part &part)
{
	part.model = part.model *
		Translate( { 0.0f, 0.0f, 0.4f }) *
		Rotate(dogNeckAngle, { 1.0f, 0.0f, 0.0f }) *
		Translate( { 0.0f, 0.0f, 0.5f });
	mat4 trans = part.model * Scale( { 0.15f, 0.15f, 0.4f });

	shader.SetUniform(UNIFORM_TYPE::MAT4, "model", &trans);
	glDrawArrays(GL_TRIANGLE_STRIP, part.firstVert, part.nVertices);
}

static_func void Draw_DOG_HEAD(ShaderProgram &shader, mesh_part &part)
{
	part.model = part.model * Translate( { 0.0f, -0.1f, 0.3f }) * Rotate(45.0f, { 1.0f, 0.0f, 0.0f }); ;
	mat4 trans = part.model * Scale( { 0.2f, 0.2f, 0.3f });

	shader.SetUniform(UNIFORM_TYPE::MAT4, "model", &trans);
	glDrawElements(GL_TRIANGLES, part.nIndices, GL_UNSIGNED_INT, (void *)0);
}

static_func void Draw_DOG_BACK_LEFT_UPPER_LEG(ShaderProgram &shader, mesh_part &part)
{
	part.model = part.model * Translate( { 0.2f, -0.35f, -0.6f }) * Rotate(80.0f, { 1.0f, 0.0f, 0.0f });
	mat4 trans = part.model * Scale( { 0.07f, 0.07f, 0.5f });

	shader.SetUniform(UNIFORM_TYPE::MAT4, "model", &trans);
	glDrawArrays(GL_TRIANGLE_STRIP, part.firstVert, part.nVertices);
}

static_func void Draw_DOG_BACK_LEFT_LOWER_LEG(ShaderProgram &shader, mesh_part &part)
{
	part.model = part.model * Translate( { 0.0f, -0.09f, 0.45f }) * Rotate(20.0f, { 1.0f, 0.0f, 0.0f });
	mat4 trans = part.model * Scale( { 0.07f, 0.07f, 0.5f });

	shader.SetUniform(UNIFORM_TYPE::MAT4, "model", &trans);
	glDrawArrays(GL_TRIANGLE_STRIP, part.firstVert, part.nVertices);
}

static_func void Draw_DOG_BACK_LEFT_FOOT(ShaderProgram &shader, mesh_part &part)
{
	part.model = part.model * Translate( { 0.0f, 0.15f, 0.19f }) * Rotate(-100.0f, { 1.0f, 0.0f, 0.0f }); ;
	mat4 trans = part.model * Scale( { 0.07f, 0.07f, 0.3f });

	shader.SetUniform(UNIFORM_TYPE::MAT4, "model", &trans);
	glDrawArrays(GL_TRIANGLE_STRIP, part.firstVert, part.nVertices);
}

static_func void Draw_DOG_BACK_RIGHT_UPPER_LEG(ShaderProgram &shader, mesh_part &part)
{
	part.model = part.model * Translate( { -0.2f, -0.35f, -0.6f }) * Rotate(80.0f, { 1.0f, 0.0f, 0.0f });
	mat4 trans = part.model * Scale( { 0.07f, 0.07f, 0.5f });

	shader.SetUniform(UNIFORM_TYPE::MAT4, "model", &trans);
	glDrawArrays(GL_TRIANGLE_STRIP, part.firstVert, part.nVertices);
}

static_func void Draw_DOG_BACK_RIGHT_LOWER_LEG(ShaderProgram &shader, mesh_part &part)
{
	part.model = part.model * Translate( { 0.0f, -0.09f, 0.45f }) * Rotate(20.0f, { 1.0f, 0.0f, 0.0f });
	mat4 trans = part.model * Scale( { 0.07f, 0.07f, 0.5f });

	shader.SetUniform(UNIFORM_TYPE::MAT4, "model", &trans);
	glDrawArrays(GL_TRIANGLE_STRIP, part.firstVert, part.nVertices);
}

static_func void Draw_DOG_BACK_RIGHT_FOOT(ShaderProgram &shader, mesh_part &part)
{
	part.model = part.model * Translate( { 0.0f, 0.15f, 0.19f }) * Rotate(-100.0f, { 1.0f, 0.0f, 0.0f }); ;
	mat4 trans = part.model * Scale( { 0.07f, 0.07f, 0.3f });

	shader.SetUniform(UNIFORM_TYPE::MAT4, "model", &trans);
	glDrawArrays(GL_TRIANGLE_STRIP, part.firstVert, part.nVertices);
}

static_func void Draw_DOG_FRONT_LEFT_UPPER_LEG(ShaderProgram &shader, mesh_part &part)
{
	part.model = part.model * Translate( { 0.2f, -0.35f, 0.6f }) * Rotate(80.0f, { 1.0f, 0.0f, 0.0f });
	mat4 trans = part.model * Scale( { 0.07f, 0.07f, 0.5f });

	shader.SetUniform(UNIFORM_TYPE::MAT4, "model", &trans);
	glDrawArrays(GL_TRIANGLE_STRIP, part.firstVert, part.nVertices);
}

static_func void Draw_DOG_FRONT_LEFT_LOWER_LEG(ShaderProgram &shader, mesh_part &part)
{
	part.model = part.model * Translate( { 0.0f, -0.09f, 0.45f }) * Rotate(20.0f, { 1.0f, 0.0f, 0.0f });
	mat4 trans = part.model * Scale( { 0.07f, 0.07f, 0.5f });

	shader.SetUniform(UNIFORM_TYPE::MAT4, "model", &trans);
	glDrawArrays(GL_TRIANGLE_STRIP, part.firstVert, part.nVertices);
}

static_func void Draw_DOG_FRONT_LEFT_FOOT(ShaderProgram &shader, mesh_part &part)
{
	part.model = part.model * Translate( { 0.0f, 0.15f, 0.19f }) * Rotate(-100.0f, { 1.0f, 0.0f, 0.0f }); ;
	mat4 trans = part.model * Scale( { 0.07f, 0.07f, 0.3f });

	shader.SetUniform(UNIFORM_TYPE::MAT4, "model", &trans);
	glDrawArrays(GL_TRIANGLE_STRIP, part.firstVert, part.nVertices);
}

static_func void Draw_DOG_FRONT_RIGHT_UPPER_LEG(ShaderProgram &shader, mesh_part &part)
{
	part.model = part.model * Translate( { -0.2f, -0.35f, 0.6f }) * Rotate(80.0f, { 1.0f, 0.0f, 0.0f });
	mat4 trans = part.model * Scale( { 0.07f, 0.07f, 0.5f });

	shader.SetUniform(UNIFORM_TYPE::MAT4, "model", &trans);
	glDrawArrays(GL_TRIANGLE_STRIP, part.firstVert, part.nVertices);
}

static_func void Draw_DOG_FRONT_RIGHT_LOWER_LEG(ShaderProgram &shader, mesh_part &part)
{
	part.model = part.model * Translate( { 0.0f, -0.09f, 0.45f }) * Rotate(20.0f, { 1.0f, 0.0f, 0.0f });
	mat4 trans = part.model * Scale( { 0.07f, 0.07f, 0.5f });

	shader.SetUniform(UNIFORM_TYPE::MAT4, "model", &trans);
	glDrawArrays(GL_TRIANGLE_STRIP, part.firstVert, part.nVertices);
}

static_func void Draw_DOG_FRONT_RIGHT_FOOT(ShaderProgram &shader, mesh_part &part)
{
	part.model = part.model * Translate( { 0.0f, 0.15f, 0.19f }) * Rotate(-100.0f, { 1.0f, 0.0f, 0.0f });
	mat4 trans = part.model * Scale( { 0.07f, 0.07f, 0.3f });

	shader.SetUniform(UNIFORM_TYPE::MAT4, "model", &trans);
	glDrawArrays(GL_TRIANGLE_STRIP, part.firstVert, part.nVertices);
}

static_func void Draw_DOG_TAIL_BASE(ShaderProgram &shader, mesh_part &part)
{
	part.model = part.model * 
		Translate( { 0.0f, 0.1f, -0.45f }) * 
		Rotate(20.0f, { 1.0f, 0.0f, 0.0f }) *
		Rotate(dogTailAngle, { 0.0f, 1.0f, 0.0f }) *
		Translate( { 0.0f, 0.0f, -0.5f });
	mat4 trans = part.model * Scale( { 0.07f, 0.07f, 0.5f });

	shader.SetUniform(UNIFORM_TYPE::MAT4, "model", &trans);
	glDrawArrays(GL_TRIANGLE_STRIP, part.firstVert, part.nVertices);
}

static_func void Draw_DOG_TAIL_END(ShaderProgram &shader, mesh_part &part)
{
	part.model = part.model * 
		Translate( { 0.0f, -0.14f, -0.4f }) * 
		Rotate(-40.0f, { 1.0f, 0.0f, 0.0f });
	mat4 trans = part.model * Scale( { 0.07f, 0.07f, 0.5f });

	shader.SetUniform(UNIFORM_TYPE::MAT4, "model", &trans);
	glDrawArrays(GL_TRIANGLE_STRIP, part.firstVert, part.nVertices);
}

void AnimateDog(u32 &animations, f32 dt, f32 time)
{
	dogTailAngle = SinF(10.0f * time) * 20.0f;
	
	if (!animations)
		return;
	
	if (animations & DOG_ANIMATION_NECK)
	{
		f32 duration = 1.0f;

		u32 animIndex = (animations & DOG_ANIMATION_NECK) / 2;
		
		curDogAnimDur[animIndex] += dt;

		bool finished = curDogAnimDur[animIndex] > duration;
		if (finished)
			curDogAnimDur[animIndex] = duration;

		
		f32 startAngle = -45.f;
		f32 endAngle = 20.0f;
		if (neckDown)
		{
			startAngle = 20.0f;
			endAngle = -45.f;
		}
		f32 range = endAngle - startAngle;

		f32 progress = curDogAnimDur[animIndex] / duration;
		dogNeckAngle = startAngle + progress * range;
			
		//DebugPrint(progress << std::endl);
			
		if (finished)
		{
			neckDown = !neckDown;
			animations &= ~DOG_ANIMATION_NECK;
			curDogAnimDur[animIndex] = 0.f;
		}
	}
}