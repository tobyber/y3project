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
#include "vec3.h"

class Ray
{
public:
	__device__ Ray();
	__device__ Ray(vec3 origin, vec3 direction);
	vec3 dir;
	vec3 origin;
	//__device__ vec3 colour;
};

__device__ void render_ray(float4* pos, vec3 cameraPos, float x, float y, float width, float height, float left, float right, float top, float bottom);


