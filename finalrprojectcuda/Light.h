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
#include "vec3gpu.h"



class Light
{
public:
	vec3gpu Position;
	float4 Colour;


	__device__ Light() {}


	__device__ Light(vec3gpu pos, float4 colour)
	{
		this->Position = pos;
		this->Colour = colour;
	}




};