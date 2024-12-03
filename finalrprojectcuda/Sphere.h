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



class Sphere
{
public:
	vec3gpu Center;
	float Radius;
	float4 Colour;


	__device__ Sphere() {}


	__device__ Sphere(vec3gpu center, float radius,float4 colour)
	{
		this->Center = center;
		this->Radius = radius;
		this->Colour = colour;
	}




};