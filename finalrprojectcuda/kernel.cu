﻿

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
#include "Sphere.h"
#include "World.h"
#include "Light.h"
#include "Model.h"


__device__ void renderRay(float4* pos, vec3gpu cameraPos, float x, float y, float width, float height, float left, float right, float top, float bottom);




__global__ void addKernel(float4* pos, int screen_width, int screen_height,vec3gpu* modelTris ,UINT32 modelTrisNo)
{








	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;

	if (y > screen_height || x > screen_width) return;
	
	//setup light positions

	//pass light positions to shading?





	//Ray r(	vec3(0, 0, 0), vec3(x / (float)screen_width, y / (float)screen_height, -1.0)	);
	//render_ray(pos, vec3(0,0,0), (float)x, (float)y, (float)screen_width, (float)screen_height, -1.0f, 1.0f, 1.0f, -1.0f);
	
	//Ray r();
	vec3gpu thing();
	World w;
	Light l1(vec3gpu(0, 0, 0), make_float4(1.0, 1.0, 1.0, 1.0));
	//Sphere s1(vec3gpu(1, 1, 2), 0.5, make_float4(0.0, 1.0, 0.0, 1.0));
	Sphere s(vec3gpu(0, 1, 2), 1.0, make_float4(1.0, 1.0, 1.0, 1.0));
	//Sphere s2(vec3gpu(-1, 1, 2), 0.5, make_float4(1.0, 0.0, 0.0, 1.0));
	

	//w.AddModel(m2);
	
	//spheres dont show if added after light?
	w.AddSphere(s);
	//w.AddSphere(s2);
	//w.AddSphere(s1);
	w.AddLight(l1);
	

	

	Ray r(pos, vec3gpu(0, 0, 0), (float)x, (float)y, (float)screen_width, (float)screen_height, -1.0f, 1.0f, -1.0f, 1.0f);

	
	
	//float4 col = w.hitSpheres(r, float(x), float(y));
	


	float4 col2 = testIntersect(r, modelTris, modelTrisNo);
	//col.x = col.x + col2.x;
	//col.y = col.y + col2.y;
	//col.z = col.z + col2.z;

	pos[y * screen_width + x] = col2;

	


	//pos[y*screen_width+x] = make_float4(r_col, g_col, z, z);
	//float test = 0.8f;
	//float r_col = sin(time / (float)screen_width);
	//float g_col = sin(time / (float)screen_height);
	//float z = 1.0;
	//pos[y*screen_width+x] = make_float4(r_col, g_col, z, z);
	
}



int prevTime = 0;
int window_width = 480;
int window_height = 480;
int f_count = 0;



size_t num_bytes = (window_width * window_height) * 4 * sizeof(float);
CShader* myShader;
cudaDeviceProp deviceProp;
GLuint vao;
GLuint vbo;
GLuint tex, buff;
Model m1;
vec3gpu *modelTrisGPU;

cudaGraphicsResource* cuda_tex_resource;

float vertices[] = {
    
    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,  // Bottom-left
    -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,  // Top-left
     1.0f,  1.0f, 0.0f,   1.0f, 1.0f,  // Top-right

    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,  // Bottom-left
     1.0f,  1.0f, 0.0f,   1.0f, 1.0f,  // Top-right
     1.0f, -1.0f, 0.0f,   1.0f, 0.0f   // Bottom-right
};





void display();				//called in winmain to draw everything to the screen
//void reshape(int width, int height);				//called when the window is resized
void init();				//called in winmain when the program starts.
//void processKeys();         //called in winmain to process keyboard input
void idle();		//idle function


//https://github.com/Forceflow/cuda2GLcore/blob/main/src/main.cpp - memcpy into the texture instead?


//__device__ vec3gpu hitSphere(vec3gpu origin, vec3gpu dir, vec3gpu Center, float radius);



__device__ void renderRay(float4* pos, vec3gpu cameraPos, float x, float y, float width, float height, float left, float right, float top, float bottom)
{

	
	//sphere intersection

	
	//pos[(int)y * (int)width + (int)x] = make_float4(col, 0.0, 0.0, 1.0);


}
void display()
{
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	
	//generate texture data from cuda
	float4* data_ptr;
	
	


	checkCudaErrors(cudaGraphicsMapResources(1, &cuda_tex_resource, 0));
	checkCudaErrors(cudaGraphicsResourceGetMappedPointer((void**)&data_ptr, &num_bytes,
		cuda_tex_resource));
	dim3 block(16, 16, 1);
	dim3 grid(window_width / block.x, window_height / block.y, 1);
	addKernel<<<grid, block >>>(data_ptr, window_width, window_height,modelTrisGPU,m1.NUMBER_OF_TRIANGLES);




	//float test = time_f / (float)window_width;
	//std::cout << test << std::endl;
	cudaGraphicsUnmapResources(1, &cuda_tex_resource, 0);
	

	glUseProgram(myShader->GetProgramObjID());

	//bind texture and get the data from pixel unpack buffer
	glBindTexture(GL_TEXTURE_2D, buff);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window_width, window_height, 0, GL_RGBA, GL_FLOAT, 0);


	//send the texture data to the shader
	//glActiveTexture(GL_TEXTURE0);
	//GLuint sampler = glGetUniformLocation(myShader->GetProgramObjID(), "sampler2D");
	//glUniform1i(sampler, 0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));


	//draw the triangles
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint vbo1;
	glGenBuffers(1, &vbo1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	

	glActiveTexture(GL_TEXTURE0);
	GLuint sampler = glGetUniformLocation(myShader->GetProgramObjID(), "sampler2D");
	glUniform1i(sampler, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));


	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

//	if (time_f >= 460) time_f = 0;
	//time_f += 5.0;
	glFlush();
	glutSwapBuffers();
}


void init()
{



	m1 = Model("ImageToStl.com_cottage_obj.stl");
	
	modelTrisGPU = m1.copyTrisToGPU();

	/*
	for (int i = 0; i < m1.NUMBER_OF_TRIANGLES; i++)
	{
		std::cout << "Triangle: " << i << std::endl;
		std::cout << "Normal:" << m1.triangles[4 * i + 0].x << std::endl;
		std::cout << "Vert 1:" << m1.triangles[4 * i + 1].x << std::endl;
		std::cout << "Vert 2:" << m1.triangles[4 * i + 2].x << std::endl;
		std::cout << "Vert 3:" << m1.triangles[4 * i + 3].x << std::endl;
			


	}
	*/


	//create shader
	myShader = new CShader();
	if (!myShader->CreateShaderProgram("basic", "shader/shader.vert", "shader/shader.frag"))
	{
		std::cout << "failed to load shader" << std::endl;
	}




	//gen buffers and textures for output
	glEnable(GL_TEXTURE_2D);
	glGenBuffers(1, &tex);

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, tex);

	glBufferData(GL_PIXEL_UNPACK_BUFFER, num_bytes, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	checkCudaErrors(cudaGraphicsGLRegisterBuffer(&cuda_tex_resource, tex, cudaGraphicsMapFlagsWriteDiscard));

	glGenTextures(1, &buff);
	glBindTexture(GL_TEXTURE_2D, buff);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	glClearColor(1.0, 1.0, 0.0, 1.0);
		
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(60, window_width / window_height, 0.1, 1);
	glOrtho(-1.0,window_width,-1.0,window_height,-1.0,1.0);

	prevTime = glutGet(GLUT_ELAPSED_TIME);




}









void idle()
{

	int curTime = glutGet(GLUT_ELAPSED_TIME);

	int dt = curTime - prevTime;
	f_count++;
	std::cout << dt << ',';

	prevTime = curTime;

	
	glutPostRedisplay();

}

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("test");

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << " GLEW ERROR" << std::endl;
	}
	const GLubyte* renderer = glGetString(GL_RENDERER);
	std::cout << "OPENGL RENDERER: " << renderer << std::endl;

	cudaGetDeviceProperties(&deviceProp,0);
	cudaSetDevice(0);
	std::cout << "CUDA DEVICE: "<< deviceProp.name << std::endl;
	//glutReshapeFunc(reshape);
	//specify which function will be called to refresh the screen.
	glutDisplayFunc(display);

	glutIdleFunc(idle);

	//starts the main loop. Program loops and calls callback functions as appropriate.
	init();
	glutMainLoop();

	return 0;

}
