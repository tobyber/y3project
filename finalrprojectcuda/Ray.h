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
	__device__ Ray( vec3gpu cameraPos,vec3gpu cameraRightDir, float x, float y, float width, float height, float left, float right, float bottom, float top,float zPlane )
	{


		//something like that

		//left = camerarotation matrix + 1 on z, -1 on x
		//right = -1 (1,0,1) * rotation matrix
		//top = (0,1,0) * rotation matrix
		//bottom 
		//




		//INVERSE MATRIX THEN MULTIPLY
		//Ray ray(cameraPos,vec3gpu());
		this->origin = cameraPos;

		//IMAGE PLANE = [-1,1]
		//
		//get pixel size from plane size,given that aspect ratio is 1:1
		float pixelSize = 2 / width;
		
		//camera look at is +1 in front of camerapos
		//so left is -1 in the x from the camerapos+cameralookat (image plane)	
		//u,v in image plane space

		//need to rotate and translate imagea plane with camera, so camera is in center -1


		//the positions are always teh same, then rotate every point by the matrix








		
		//left = camPos + lookat then translate by -1

		//DOESNT WORK!


		//CAMERALOOK AT + NORMALISED DIRECTION = CENTRE OF IMAGE PLANE
		//-E.G LEFT WILL BE - X AXIS OF THE NORMALISED DIRECTION VECTOR?





		//the left and bottom can move in world space from the axes e.g need all the information not just u,v
		//float left2 = (cameraPos + camLookAt).x - 1;
		//float bottom2 = (cameraPos + camLookAt).y - 1;
		//u,v are the vectors in world coordinates, i think
		float u = left + ((x + 0.5) * pixelSize);
		float v = bottom + ((y + 0.5) * pixelSize);
		//given camera is aligned to axes.
		vec3gpu ldir;


		//
		
		//cam space -> world space   //inverse of world to cam?


		cameraRightDir.normalise();
		//vec3gpu testDir = cameraRightDir*u;
		//z not calculatred
	
		//ldir.normalise();



		//dir.y = v * 1;
		//direction goes foward since plane is at z = 1;
		//dir.z = 1.0;


		vec3gpu spot(u, v,zPlane);


		//vec3gpu spot2;	

		//spot2.x = camTransform[0] * spot.x + camTransform[1] * spot.y + camTransform[2] * spot.z + camTransform[3] * 1;
		//spot2.y = camTransform[4] * spot.x + camTransform[5] * spot.y + camTransform[6] * spot.z + camTransform[7] * 1;
		//spot2.z = camTransform[8] * spot.x + camTransform[9] * spot.y + camTransform[10] * spot.z + camTransform[11] * 1;


		
		//ldir = (spot - this->origin);

		vec3gpu unNormdir(spot - cameraPos);

		unNormdir.normalise();

		this->dir = unNormdir;

	}
	vec3gpu dir;
	vec3gpu origin;
	//__device__ vec3gpu colour;
};

//__device__ void render_ray(float4* pos, vec3gpu cameraPos, float x, float y, float width, float height, float left, float right, float top, float bottom);


