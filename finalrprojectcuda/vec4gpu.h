#pragma once
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




struct __device_builtin__ __builtin_align__(16) vec4gpu
{

	float x;
	float y;
	float z;
	float w;
	__device__ vec4gpu() { this->x = 0; this->y = 0; this->z = 0; this->w = 0; }
	__host__ __device__ vec4gpu(const float& x, const float& y,const float& z, const float& w) { this->x = x; this->y = y; this->z = z; this->w = w; }
	//__host__ __device__ vec4gpu operator*(const float& f) { return vec4gpu(x * ray2.x, y * ray2.y, z * ray2.z); }
	//__host__ __device__ vec4gpu operator+(vec4gpu& ray2) { return vec4gpu(x + ray2.x, y + ray2.y, z + ray2.z); }
	//__host__ __device__ vec4gpu operator-(vec4gpu& ray2) { return vec4gpu(x - ray2.x, y - ray2.y, z - ray2.z); }
	//__host__ __device__ vec4gpu operator*(float f1) { return vec4gpu(x * f1, y * f1, z * f1); }
	__host__ __device__ float mag() { return sqrtf((x * x) + (y * y) + (z * z)); }
	__host__ __device__ void normalise()
	{
		float mag = this->mag();

		this->x = this->x / mag;
		this->y = this->y / mag;
		this->z = this->z / mag;
	}

	

};
