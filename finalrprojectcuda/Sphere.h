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
#include "Light.h"



class Sphere
{
public:

	


	vec3gpu Center;
	float Radius;
	float4 Colour;
	int material;

	__device__ Sphere() {}


	__device__ Sphere(vec3gpu center, float radius,float4 colour, int mat)
	{
		this->Center = center;
		this->Radius = radius;
		this->Colour = colour;
		this->material = material;
	}



	__device__ float4 colourSphere(vec3gpu& hitPoint, vec3gpu camPos, Light l)
	{

		




		//only works for 1 light




		vec3gpu lightDir = hitPoint- l.Position;
		lightDir.normalise();


		vec3gpu normal = (hitPoint-this->Center);

		normal.normalise();

		vec3gpu reflection = (lightDir * normal) * 2.0;
		reflection = reflection * (normal - lightDir);
		reflection.normalise();
		//need to pass in cam pos

		vec3gpu viewDir = camPos - hitPoint;

		viewDir.normalise();




		float diff = normal.dot(lightDir*-1);
		//diff = 1.0;
		if (diff <= 0) return make_float4(0.0, 0.0, 0.0, 1.0);
		//if (diff == 0.0f) return make_float4(0.0,0.0,0.0,1.0);
		float reflect = reflection.dot(viewDir);

		reflect = powf(reflect, 4);


		float4 lightCol = l.Colour;
		//needs to be changed to easily tell if hit a sphere or not, infinite recursion make sperate function

		//ambient + diffuse + spec

		//spec term =  (0.8 * reflect) + 1.0 * 0.1; (*diff)
		
		float colX = (this->Colour.x * l.Colour.x * diff) + (0.2 * this->Colour.x);
		float colY = (this->Colour.y * l.Colour.y * diff) + (0.2 * this->Colour.y);
		float colZ = (this->Colour.z * l.Colour.z * diff) + (0.2 * this->Colour.z);



		float max = fmaxf(colX, colY);
		max = fmaxf(max, colZ);
		max = 1.0f;
		if (max != 0.0)
		{
			max = 1.0 / max;

			colX = colX * max;
			colY = colY * max;
			colZ = colZ * max;

		}


		return make_float4(colX, colY, colZ, 1.0);


	}





	




};