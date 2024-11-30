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




class vec3gpu
{
public:

	float x;
	float y;
	float z;

	__device__ vec3gpu() { this->x = 0; this->y = 0; this->z = 0;}
	__device__ vec3gpu(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
	__device__ vec3gpu operator*(vec3gpu &ray2) { return vec3gpu(x * ray2.x, y * ray2.y, z * ray2.z); }
	__device__ vec3gpu operator*(float f1) { return vec3gpu(x * f1, y * f1, z * f1); }
	__device__ vec3gpu operator-(vec3gpu &ray2) {	return vec3gpu(x - ray2.x, y - ray2.y, z - ray2.z); }
	__device__ void normalise() 
	{
		float mag = sqrtf(x * x + y * y + z * z);

		this->x = this->x / mag;
		this->y = this->y / mag;
		this->z = this->z / mag;
	}


	




};