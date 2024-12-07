#pragma once

#include "GL\glew.h"
#include "GL\wglew.h"
#pragma comment(lib, "glew32.lib")
//--------------

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"

#include "GL\freeglut.h"

#include "cuda_runtime.h"
#include "cudaGL.h"
#include "cuda_gl_interop.h"
#include "device_launch_parameters.h"

#include "shaders\Shader.h"
#include "helper_cuda.h"
#include <stdio.h>
#include <iostream>
#include "vec3gpu.h"

class Ray
{
public:
	__device__ Ray() {this->dir = vec3gpu();	this->origin = vec3gpu(); }
	__device__ Ray(vec3gpu origin, vec3gpu direction) {	this->dir = direction;	this->origin = origin;}
	__device__ Ray(float4* pos, vec3gpu cameraPos, float x, float y, float width, float height, float left, float right, float bottom, float top)
	{
		//Ray ray(cameraPos,vec3gpu());
		this->origin = cameraPos;

		//IMAGE PLANE = [-1,1]
		//
		//get pixel size from plane size,given that aspect ratio is 1:1
		float pixelSize = 2 / width;

		//u,v are the vectors in world coordinates, i think
		float u = left + ((x + 0.5) * pixelSize);
		float v = bottom + ((y + 0.5) * pixelSize);
		//given camera is aligned to axes.
		vec3gpu ldir;
		//dir.x = u * 1;
		//dir.y = v * 1;
		//direction goes foward since plane is at z = 1;
		//dir.z = 1.0;


		vec3gpu spot(u, v, 1.0f);

		//ldir = (spot - this->origin);

		vec3gpu unNormdir(spot - this->origin);

		unNormdir.normalise();

		this->dir = unNormdir;

	}
	vec3gpu dir;
	vec3gpu origin;
	//__device__ vec3gpu colour;
};

//__device__ void render_ray(float4* pos, vec3gpu cameraPos, float x, float y, float width, float height, float left, float right, float top, float bottom);


