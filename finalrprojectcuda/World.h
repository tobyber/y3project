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
#include <vector>
#include "vec3gpu.h"
#include "Sphere.h"
#include "Ray.h"
#include "Light.h"
#include "Model.h"
#include <algorithm>

class World
{
	//spheres

	//render?d

public:

	static const int MAX_SPHERES = 10;
	static const int MAX_LIGHTS = 10;
	static const int MAX_MODELS = 5;

	//change this

	//std::vector<vec3gpu*> models;
	Light Lights[MAX_LIGHTS];
	Sphere* Spheres[MAX_SPHERES];
	vec3gpu* modelTris[MAX_MODELS];
	UINT32 modelTrisNo[MAX_MODELS];
	float4 modelColours[MAX_MODELS];


	__device__ World()
	{
		//Sphere s(vec3gpu(0, 0, 0), 1, make_float4(0.0, 0.0, 0.0, 1.0));
		//Spheres[0] = s;
	}


	//make it so cant do more than MAX
	__device__ void AddSphere(Sphere* sp1)
	{
		this->Spheres[sphereIdx] = sp1;
		sphereIdx++;

	}

	__device__ void AddLight(Light l1)
	{
		this->Lights[lightSize] = l1;
		lightSize++;

	}

	__device__ void AddModel(vec3gpu* tris, UINT32 noTris,float4 modelCol)
	{
		this->modelTris[modelIdx] = tris;
		this->modelTrisNo[modelIdx] = noTris;
		this->modelColours[modelIdx] = modelCol;
		modelIdx++;

	}


	__device__ int testIntersect(Ray r, Light l, vec3gpu &hitPoint, vec3gpu &hitNormal,float &tVal)
	{


		//normal calculations wrong?
		int closestIdx = -1;
		float closestInteresction = 1000;
		vec3gpu closestNormal;

		for (int j = 0; j < this->modelIdx; j++)
		{



			//this doesnt work.

			//array of pointers to pointers to block of memory->
			for (UINT32 i = 0; i < modelTrisNo[j]; i++)
			{
				vec3gpu edge1, edge2, p;
				float det;
				//0,1,2,3
				vec3gpu vert0 = (modelTris[j])[(4 * i) + 1];
				vec3gpu vert1 = (modelTris[j])[(4 * i) + 2];
				vec3gpu vert2 = (modelTris[j])[(4 * i) + 3];
				vec3gpu normal = (modelTris[j])[(4 * i) + 0];

				//vert0.normalise();
				//vert1.normalise();
				//vert2.normalise();
				//vert0.y = -vert0.y;
			//	vert1.y = -vert1.y;
				//vert2.y = -vert2.y;
				//normal.y = -normal.y;

				vert0.z += 10;
				vert1.z += 10;
				vert2.z += 10;
				//normal. += 10;

				edge1 = (vert1 - vert0);
				edge2 = (vert2 - vert0);
				p = r.dir.cross(edge2);


				//required for culling
				det = edge1.dot(p);


				//currently doing non culling
				float EPSILON = FLT_EPSILON;

				//if det near 0, then parallel so cant intersect
				if (det > -EPSILON && det < EPSILON) continue;
				float inv_det = 1.0f / det;

				vec3gpu tvec = r.origin - vert0;



				float u = (tvec.dot(p)) * inv_det;

				// u + v + t = 1
				if (u < 0.0f || u > 1.0f) continue;

				vec3gpu q = tvec.cross(edge1);

				float v = (r.dir.dot(q)) * inv_det;

				// u + v + t = 1
				if (v < 0.0f || v + u > 1.0f) continue;

				float t = (edge2.dot(q)) * inv_det;

				if (t > EPSILON && t < closestInteresction)
				{

					closestInteresction = t;
					closestIdx = j;
					closestNormal = normal;
				}

			}

		}


		if (closestIdx == -1) return -1;


		tVal = closestInteresction;
		hitPoint = r.origin + (r.dir * closestInteresction);
		hitNormal = closestNormal;


		return closestIdx;



	}


	//need to pass in cameraPos (or just a new ray)
	__device__ float4 colourModel(vec3gpu hitPoint, vec3gpu hitNormal, int modelIdx, Light l)
	{

		


		//only works for 1 light

		
		vec3gpu lightDir = l.Position - hitPoint;
		lightDir.normalise();
		hitNormal.normalise();


		float diff = fmaxf(hitNormal.dot(lightDir*-1), 0.0);
		

		float4 lightCol = Lights[0].Colour;

		vec3gpu reflection = (lightDir * hitNormal) * 2.0;
		reflection = reflection * (hitNormal - lightDir);
		
		//need to pass in cam pos
		vec3gpu camPos(0, 0, 0);
		vec3gpu viewDir = camPos - hitPoint;

		viewDir.normalise();
		

		//make this atbitrary
		float4 col = this->modelColours[modelIdx];
		float4 lightdiff = make_float4(lightCol.x * diff, lightCol.y * diff, lightCol.z * diff, 1.0);
		
		//float4 finalCol = fmaxf(1.0)
		//diffCONST * DIFF + specCONST*(reflection*viewer)^shinyness



		return make_float4(col.x,  col.y,  col.z, 1.0);
		//return col;


	}








	__device__ bool isPointInModelShadow(vec3gpu hitPoint, Light l, int modelIdx)
	{

		vec3gpu shadowrayDir = l.Position - hitPoint;
		shadowrayDir.normalise();
		Ray shadowray(hitPoint, shadowrayDir);
		vec3gpu shadowhit;
		vec3gpu shadowNormal;
		float t;
		int shadowHitIdx = this->testIntersect(shadowray, l, shadowhit, shadowNormal,t);
		//this might be wrong, shadows of models can hit itself?
		if (shadowHitIdx != -1 && shadowHitIdx != modelIdx) {
			return true;

		}
		return false;

	}









	//SOMETHING IS DEFO WRONG WITH THIS?
	//may cause an error default closest = 0 (e.g if negative but should be fine since not intereseted in behind camera(for now)?)
	__device__ bool hitSphere(int idx, Ray r, float& intersectVal)
	{

		vec3gpu L = r.origin - this->Spheres[idx]->Center;
		float a = r.dir.dot(r.dir);
		float b = 2 * r.dir.dot(L);
		float c = L.dot(L) - this->Spheres[idx]->Radius * this->Spheres[idx]->Radius;

		float root1, root2;
		float discriminant = b * b - (4 * a * c);
		if (discriminant < 0) return false;
		if (discriminant == 0)
		{
			root1 = -b / (2 * a);
			root2 = -b / (2 * a);
		}
		else
		{
			root1 = (-b + sqrtf(discriminant)) / 2 * a;
			root2 = (-b - sqrtf(discriminant)) / 2 * a;

		}


		if (root1 > root2)
		{
			intersectVal = root2;
			return true;
		}
		intersectVal = root1;
		return true;



		//vec3gpu center = Spheres[idx]->Center;
		//float radius = Spheres[idx]->Radius;
		//r.dir.normalise();
		//vec3gpu L = center - r.origin;
		//float4 col = make_float4(0.0, 0.0, 0.0, 0.0);
		//float distance = L.dot(r.dir);
		//if (distance < 0.0f) continue;
		//float distFromCentresquare = L.dot(L) - (distance * distance);
		//if (distFromCentresquare > (radius * radius)) return false;

		//float hordistFromCentre = sqrtf((radius * radius) - distFromCentresquare);



		//float intersection = distance - hordistFromCentre;
		//float intersect2 = distance + hordistFromCentre;

		//intersection = fminf(intersection, intersect2);

		//intersectVal = intersection;
		return true;


	}

	__device__ bool isPointInSphereShadow(vec3gpu& hitPoint, Light l,int curIdx)
	{

		vec3gpu shadowrayDir = l.Position - hitPoint;
		shadowrayDir.normalise();
		Ray shadowray(hitPoint, shadowrayDir);
		vec3gpu shadowhit;
		float t;
		for (int i = 0; i < this->getNumSpheres(); i++)
		{

			//cant hit itself to check (currently only doing 1 intersection on spheres)
			if (i == curIdx) continue;
			if (this->hitSphere(i, shadowray, t)) return true;

		}
		
		return false;


	}
	



	__device__ int getNumSpheres()
	{
		return this->sphereIdx;
	}




private:
	int sphereIdx = 0;
	int lightSize = 0;
	int modelIdx = 0;
	int trisIdx = 0;

	//other objects etc.


	//list of spehers for now

	///std::list



			
	//hit world e.g

	//use pointers
	



};