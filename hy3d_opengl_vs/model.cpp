#include "model.h"

void cylinder::Make(i32 stacks, i32 slices, f32 height, f32 radius)
{
	nVerts = stacks * (slices + 1) * 2;
	verts = (vertex *)malloc(BufferSize());

	float stackStep = height / stacks;
	float sliceStep = 2.0f * HMM_PI32 / slices;

	u32 iVert = 0;
	for (i32 iStack = 0; iStack < stacks; ++iStack)
	{
		f32 z0 = 0.5f * height - iStack * stackStep;
		f32 z1 = z0 - stackStep;

		for (i32 iSlice = 0; iSlice <= slices; ++iSlice)
		{
			f32 theta = iSlice * sliceStep;

			f32 x = radius * cos(theta);
			f32 y = radius * sin(theta);

			verts[iVert].pos = { x, y, z0 };
			verts[iVert].normal = { x / radius, y / radius, 0.0 };
			//glTexCoord2f(j / (GLfloat) numMinor, i / (GLfloat) numMajor);
			iVert++;

			verts[iVert].pos = { x, y, z1 };
			verts[iVert].normal = { x / radius, y / radius, 0.0 };
			//glTexCoord2f(j / (GLfloat) numMinor, (i + 1) / (GLfloat) numMajor);
			iVert++;
		}
	}
}