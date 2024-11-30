
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
#include "Ray.h"


	__device__ Ray::Ray()
	{
		this->dir = vec3gpu();
		this->origin = vec3gpu();
	}

	__device__ Ray::Ray(vec3gpu origin, vec3gpu direction)
	{
		this->dir = direction;
		this->origin = origin;


	}
	






