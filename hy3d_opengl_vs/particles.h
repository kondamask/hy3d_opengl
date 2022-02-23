#ifndef INCLUDE_PARTICLES_H
#define INCLUDE_PARTICLES_H

#include "core.h"
#include "math.h"
#include "renderer.h"
#include "model.h"
#include "shader.h"
#include "random.h"

#define MAX_PARTICLES 1024

struct particle_system
{
	vec3 p[MAX_PARTICLES];
	vec3 dp[MAX_PARTICLES];
	render_buffers buffers;

	//------------------------------------------------------------------------

	void Initialize();

	void Delete();

	void UpdateAndRender(ShaderProgram &shader, f32 dt, f32 time);

private:
	vec3 gravity = { 0.0f, -4.0f, 0.0f };
	u32 spawnIndex = 0;
	f32 spawnTimer = 0.0f;
	f32 spawnFreq = 0.01f;
	u32 nParticles = 0;
	bool spawnedMax = false;
	random_series randomGen = {};
};

#endif