#include "particles.h"

void particle_system::Initialize()
{
	mesh sphere = {};
	sphere.MakeSphere(10, 10, 0.05f);

	buffers.vertices = ArrAlloc(vertex, sphere.buffers.nVertices);
	buffers.indices = ArrAlloc(u32, sphere.buffers.nIndices);
	buffers.nVertices = sphere.buffers.nVertices;
	buffers.nIndices = sphere.buffers.nIndices;
	//buffers.Initialize(sphere.buffers.nVertices, sphere.buffers.nIndices);
	memcpy(buffers.vertices, sphere.buffers.vertices, sphere.buffers.GetVertexBufferSize());
	memcpy(buffers.indices, sphere.buffers.indices, sphere.buffers.GetIndexBufferSize());

	sphere.Delete();

	randomGen.Seed(1234);
	buffers.Initialize();

}

void particle_system::Delete()
{
	buffers.Delete();
}

void particle_system::UpdateAndRender(ShaderProgram &shader, f32 dt, f32 time)
{
	spawnTimer += dt;

	if (spawnTimer >= spawnFreq)
	{
		spawnTimer = 0.0;

		p[spawnIndex] = {
			randomGen.RandomBetween(-0.2f, 0.2f),
			0.5f,
			randomGen.RandomBetween(-0.2f, 0.2f) };

		dp[spawnIndex] = {
			randomGen.RandomBetween(1.5f, 2.0f),
			5.0f,
			randomGen.RandomBetween(-0.5f, 0.5f)
		};
		
		spawnIndex++;

		if (spawnIndex == MAX_PARTICLES)
			spawnedMax = true;
		else if (!spawnedMax)
			nParticles = spawnIndex;

		spawnIndex %= MAX_PARTICLES;
	}

	glBindVertexArray(buffers.VAO);
	for (u32 i = 0; i < nParticles; i++)
	{
		// Apply forces		
		p[i] += (0.5f * Square(dt) * gravity + dp[i]) * dt;
		dp[i] += gravity * dt;

		if (p[i].Y < 0.0f)
		{
			f32 energyLoss = 0.8f;
			p[i].Y = 0.0f;
			dp[i].Y = -dp[i].Y * energyLoss;
		}

		mat4 model = Translate(p[i]);

		shader.SetUniform(UNIFORM_TYPE::MAT4, "model", &model);
		glDrawElements(GL_TRIANGLES, buffers.nIndices, GL_UNSIGNED_INT, (void *)0);
	}
	glBindVertexArray(0);
}