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
#include "Sphere.h"
#include "Ray.h"
#include "Light.h"
#include "Model.h"


class World
{
	//spheres

	//render?d

public:

	static const int MAX_SPHERES = 10;
	static const int MAX_LIGHTS = 10;
	Light Lights[MAX_LIGHTS];
	Sphere Spheres[MAX_SPHERES];
	Model* model;

	__device__ World() 
	{
		//Sphere s(vec3gpu(0, 0, 0), 1, make_float4(0.0, 0.0, 0.0, 1.0));
		//Spheres[0] = s;
	}


	//make it so cant do more than MAX
	__device__ void AddSphere(Sphere sp1)
	{
		this->Spheres[sphereIdx] = sp1;
		sphereIdx++;

	}

	__device__ void AddLight(Light l1)
	{
		this->Lights[lightSize] = l1;
		sphereIdx++;

	}

	__device__ void AddModel(Model* m1)
	{
		this->model = m1;

	}




	//may cause an error default closest = 0 (e.g if negative but should be fine since not intereseted in behind camera(for now)?)
	__device__ float4 hitSpheres(Ray r, float x, float y)
	{


		vec3gpu origin;


		int closestIdx = -1;
		float closestInteresction = 1000;
		bool sphereChanged = false;





		for (int i = 0; i < sphereIdx; i++)
		{
			vec3gpu center = Spheres[i].Center;
			float radius = Spheres[i].Radius;

			vec3gpu L = center - origin;
			float4 col = make_float4(0.0, 0.0, 0.0, 0.0);
			float distance = L.dot(r.dir);

		


			float distFromCentresquare = L.dot(L) - distance * distance;


			if (distFromCentresquare > radius * radius)
			{
				continue;
			}

			else
			{
				float hordistFromCentre = sqrtf(radius * radius - distFromCentresquare);



				float intersection = distance - hordistFromCentre;


				//MAYBE BUGGED?
				if (closestInteresction > intersection)
				{
					closestIdx = i;
					closestInteresction = intersection;
				}
				


			
			}
			if (closestIdx == -1) return make_float4(0.0, 0.0, 0.0, 1.0);
			return colourSphere(r,closestInteresction,closestIdx);


		}

	}


	__device__ float4 colourSphere(Ray r, float intersection, int sphereIdx)
	{

		Sphere s = Spheres[sphereIdx];


		
		vec3gpu hitpoint = r.dir * intersection;

		vec3gpu normal = hitpoint - s.Center;

		normal.normalise();


		vec3gpu lightDir = Lights[0].Position - hitpoint;
		lightDir.normalise();

		float diff = fmaxf(normal.dot(lightDir), 0.0);

		float4 lightCol = Lights[0].Colour;

		float4 lightdiff = make_float4(lightCol.x * diff, lightCol.y * diff, lightCol.z * diff, 1.0);

		return make_float4(lightdiff.x * s.Colour.x, lightdiff.y * s.Colour.y, lightdiff.z * s.Colour.z, 1.0);





	}






private:
	int sphereIdx = 0;
	int lightSize = 0;

	//other objects etc.


	//list of spehers for now

	///std::list



			
	//hit world e.g

	//use pointers
	



};