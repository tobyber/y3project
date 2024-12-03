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

class World
{
	//spheres

	//render?

public:

	static const int MAX_SPHERES = 10;
	
	Sphere Spheres[MAX_SPHERES];

	__device__ World() 
	{
		//Sphere s(vec3gpu(0, 0, 0), 1, make_float4(0.0, 0.0, 0.0, 1.0));
		//Spheres[0] = s;
	}


	__device__ void AddSphere(Sphere sp1)
	{
		this->Spheres[sphereIdx] = sp1;
		sphereIdx++;

	}



	//may cause an error default closest = 0 (e.g if negative but should be fine since not intereseted in behind camera(for now)?)
	__device__ float4 hitSpheres(Ray r, float x, float y)
	{


		vec3gpu origin;

		//IMAGE PLANE = [-1,1]
		//
		//get pixel size from plane size,given that aspect ratio is 1:1
		//float pixelSize = 2.0 / 480.0;

		//u,v are the vectors in world coordinates, i think
		//float u = -1.0 + ((x + 0.5) * pixelSize);
		//float v = -1.0 + ((y + 0.5) * pixelSize);
		//given camera is aligned to axes.
		//vec3gpu ldir;
		//dir.x = u * 1;
		//dir.y = v * 1;
		//direction goes foward since plane is at z = 1;
		//dir.z = 1.0;


		//vec3gpu spot(u, v, 1.0f);

		//ldir = (spot - origin);

		//vec3gpu unNormdir(spot - origin);

		//unNormdir.normalise();

		//vec3gpu dir = unNormdir;








		int closestIdx = NULL;
		float closestInteresction = 0;





		//make it work for mulltiple spheres



		//for (int i = 0; i < sphereIdx-1; i++)
		//{
			int i = 0;
			vec3gpu center = Spheres[i].Center;
			float radius = Spheres[i].Radius;

			//vec3gpu center = vec3gpu(0, 1, 3);
			//float radius = 1.0;

			vec3gpu L = center - origin;
			float4 col = make_float4(0.0,0.0,0.0,0.0);
			float distance = L.dot(r.dir);

			//if (distance < 0) col = 0.0;


			float distFromCentresquare = L.dot(L) - distance * distance;


			if (distFromCentresquare > radius * radius)
			{
				//continue;
				return make_float4(0.0, 0.0, 0.0, 1.0);
			}

			else
			{
				float hordistFromCentre = sqrtf(radius * radius - distFromCentresquare);



				float intersection = distance - hordistFromCentre;


			//	if (closestInteresction > intersection)
				//{
					closestIdx = i;
					closestInteresction = intersection;
				//}
				//if (intersection < 0) col = 0.0;s
				//dir*intersection = the point of intersection, for now just need to check if it does;



				//FUTURE CALCULATION!!!
				//vec3gpu hitpoint = r.dir * intersection;
				//vec3gpu normal = hitpoint - center;
				//vec3gpu camvec = hitpoint * -1.0;

				//normal.normalise();
				//camvec.normalise();
				//only shows as one colour?
				//col = abs(normal.dot(camvec));
				//if (dir.z * 3 >= 2.5f) return;
				
					//return make_float4(1.0, 0.0, 0.0, 1.0);
					return Spheres[i].Colour;
			}
		//}
		//if (closestIdx == NULL) return make_float4(0.0, 0.0, 0.0, 0.0);
		//return Spheres[closestIdx].Colour;



	}



private:
	int sphereIdx = 0;


	//other objects etc.


	//list of spehers for now

	///std::list



			
	//hit world e.g

	//use pointers
	



};