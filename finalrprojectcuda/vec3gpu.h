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

	__host__ __device__ vec3gpu() { this->x = 0; this->y = 0; this->z = 0;}
	__host__ __device__ vec3gpu(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
	__host__ __device__ vec3gpu operator*(vec3gpu &ray2) { return vec3gpu(x * ray2.x, y * ray2.y, z * ray2.z); }
	__host__ __device__ vec3gpu operator+(vec3gpu& ray2) { return vec3gpu(x + ray2.x, y + ray2.y, z + ray2.z); }
	__host__ __device__ vec3gpu operator-(vec3gpu& ray2) { return vec3gpu(x - ray2.x, y - ray2.y, z - ray2.z); }
	__host__ __device__ vec3gpu operator*(float f1) { return vec3gpu(x * f1, y * f1, z * f1); }
	__host__ __device__ float mag() { return sqrtf((x * x) + (y * y) + (z * z)); }
	__host__ __device__ void normalise()
	{
		float mag = this->mag();

		this->x = this->x / mag;
		this->y = this->y / mag;
		this->z = this->z / mag;
	}

	__host__ __device__ float dot(vec3gpu &vec2) { return (this->x * vec2.x) + (this->y * vec2.y) +( this->z * vec2.z);	}
	__host__ __device__ vec3gpu cross(vec3gpu& vec2) { return vec3gpu((this->y * vec2.z) - (this->z * vec2.y), (this->z * vec2.x) - (this->x*vec2.z),(this->x*vec2.y)-(this->y*vec2.x)); }
	




};