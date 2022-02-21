#ifndef INCLUDE_CAMERA_H
#define INCLUDE_CAMERA_H

#include "core.h"
#include "math.h"

struct camera
{
	vec3 pos;
	vec3 dir;
	vec3 up;
	
	f32 pitch;
	f32 yaw;
	
	f32 speed;
	f32 sens;
	f32 fov;
	
	//------------------------------------------------------------------------

	inline void Initialize(vec3 posIn, vec3 dirIn, vec3 upIn, f32 speedIn, f32 sensIn, f32 fovIn = 45.0f)
	{
		pos = posIn;
		dir = dirIn;
		up = upIn;
		
		pitch = 0.0f;
		yaw = -90.0f;
		
		speed = speedIn;
		sens = sensIn;
		fov = fovIn;
	}
	
	inline void UpdateDir(f32 xOffset, f32 yOffset)
	{
		yaw += xOffset;
		pitch += yOffset;
		pitch = Clamp(-89.0f, pitch, 89.0f);
		
		f32 pitchRad = ToRadians(pitch);
		f32 yawRad = ToRadians(yaw);
		
		dir.X = CosF(yawRad) * CosF(pitchRad);
		dir.Y = SinF(pitchRad);
		dir.Z = SinF(yawRad) * CosF(pitchRad);
		dir = Normalize(dir);
	}
};

#endif