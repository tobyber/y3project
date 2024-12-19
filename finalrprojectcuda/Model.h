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
#include "World.h"
#include <vector>


struct vec3 {
	float x;
	float y;
	float z;
};











class Model
{

public:



	UINT32 NUMBER_OF_TRIANGLES;
	UINT32 NUMBER_OF_VECTORS;
	static const int VECTORS_PER_TRIANGLE = 4;
	static const int FLOATS_PER_VECTOR = 3;
	static const int SIZE_OF_FLOAT = 4;

	vec3gpu* triangles;


	__device__ Model(vec3gpu* triangles, UINT32 NUMBER_OF_TRIANGLES)
	{
		this->triangles = triangles;
		this->NUMBER_OF_TRIANGLES = NUMBER_OF_TRIANGLES;
		this->NUMBER_OF_VECTORS = NUMBER_OF_TRIANGLES * 4;
	}



	__host__ __device__ Model() {}


	


	__host__ Model(std::string filePath)
	{

		//BYTE modelData[500000];


		BYTE fileBuff[128];
		std::ifstream modelFile;
		modelFile.open(filePath, std::ios::in | std::ios::binary);
		modelFile.seekg(80);

		UINT32 NUMBER_OF_TRIANGLES;

		BYTE testbuf[4] = { modelFile.get(),modelFile.get() ,modelFile.get(),modelFile.get() };

		memcpy(&NUMBER_OF_TRIANGLES, testbuf, sizeof(NUMBER_OF_TRIANGLES));

		std::cout << NUMBER_OF_TRIANGLES << std::endl;

		this->NUMBER_OF_TRIANGLES = NUMBER_OF_TRIANGLES;
		this->NUMBER_OF_VECTORS = NUMBER_OF_TRIANGLES * 4;
		this->triangles = new vec3gpu[NUMBER_OF_TRIANGLES*4];


		
		BYTE vectorBuff[SIZE_OF_FLOAT];
		float floatX,floatY,floatZ;
		



		for (int i = 0; i < NUMBER_OF_TRIANGLES; i++)
		{

			for (int j = 0; j < VECTORS_PER_TRIANGLE; j++)
			{
				for (int k = 0; k < FLOATS_PER_VECTOR; k++)
				{

					for (int l = 0; l < SIZE_OF_FLOAT; l++)
					{
						vectorBuff[l] = modelFile.get();
					}
					if (k == 0) std::memcpy(&floatX, &vectorBuff, sizeof(vectorBuff));
					if (k == 1) std::memcpy(&floatY, &vectorBuff, sizeof(vectorBuff));
					if (k == 2) std::memcpy(&floatZ, &vectorBuff, sizeof(vectorBuff));
				}	
				vec3gpu v1(floatX, floatY, floatZ);
				triangles[ (4*i) +j] = v1;
		
			}


			//Two useless bytes at the end of each triangle
			char c;
			modelFile.get(c);
			modelFile.get(c);

		}

	}

	__host__ vec3gpu* copyTrisToGPU()
	{
		vec3gpu *m1;
		size_t size = sizeof(vec3gpu)*NUMBER_OF_TRIANGLES*4;
		checkCudaErrors(cudaMalloc((void**)&m1, size));
		checkCudaErrors(cudaMemcpy(m1,triangles,size, cudaMemcpyHostToDevice));
		return m1;

	}





};
