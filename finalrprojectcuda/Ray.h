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
	__device__ Ray( vec3gpu cameraPos,vec3gpu cameraRightDir, float x, float y, float width, float height)
	{

		



		//this->origin = cameraPos;
		this->origin = cameraPos;

		//IMAGE PLANE = [-1,1]
		//
		//get pixel size from plane size,given that aspect ratio is 1:1
		float pixelSize = 2 / width;


		
		float u = -1 + ((x + 0.5) * pixelSize);
		float v = -1 + ((y + 0.5) * pixelSize);
		


		cameraRightDir.normalise();
		

		vec3gpu viewDir = gpucamLookAt - cameraPos;
		viewDir.normalise();
		float planeWidthHalf = tanf(20.0f / 2.0f);
		float aspectRatio = height/width;
		float planeHeightHalf = aspectRatio * planeWidthHalf;
		vec3gpu planeRDir = viewDir.cross(vec3gpu(0, 1, 0));

		vec3gpu planeUDir = planeRDir.cross(viewDir);
		planeRDir.normalise();
		planeUDir.normalise();



		vec3gpu bottomleft = gpucamLookAt - (planeRDir*planeWidthHalf)-(planeUDir*planeHeightHalf);

		vec3gpu xOffset = (planeRDir * 2 * planeWidthHalf) * (1.0f/width);
		vec3gpu yOffset = (planeUDir * 2 * planeHeightHalf) * (1.0f / height);


		vec3gpu planePoint = bottomleft + (xOffset*x) + (yOffset * y);
		

		vec3gpu unNormdir(planePoint - cameraPos);

		unNormdir.normalise();

		this->dir = unNormdir;



	}
	vec3gpu dir;
	vec3gpu origin;
	//__device__ vec3gpu colour;
};

//__device__ void render_ray(float4* pos, vec3gpu cameraPos, float x, float y, float width, float height, float left, float right, float top, float bottom);


