

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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl3.h"




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









void handleKeys();





__device__ void renderRay(float4* pos, vec3gpu cameraPos, float x, float y, float width, float height, float left, float right, float top, float bottom);




__global__ void addKernel(float4* pos, int screen_width, int screen_height,vec3gpu* modelTris ,UINT32 modelTrisNo, vec3gpu cameraPos, vec3gpu cameraRightDir,vec3gpu gpucamLookAt)
{

	
	vec3gpu background[4] = {
		vec3gpu(0,0,-1),
		vec3gpu(-20, -50, 10),
		vec3gpu(20, -50, 10),
		vec3gpu(20, 50, 10),

	};


	vec3gpu background2[4] = {
		vec3gpu(0,0,-1),
		vec3gpu(20, -50, 10),
		vec3gpu(-20, -50, 10),
		vec3gpu(-20, 50, 10),

	};

	vec3gpu floor[4] = {
		vec3gpu(0,1.0,0),
		vec3gpu(-20, 10, -20),
		vec3gpu(20, 10, -20),
		vec3gpu(20, 10, 20),

	};



	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;

	if (y > screen_height || x > screen_width) return;

	vec3gpu thing();
	World w;
	Light l1(vec3gpu(0, -1 , 0), make_float4(1.0, 1.0, 1.0, 1.0));
	Sphere s1(vec3gpu(0, 3, -7), 1.0, make_float4(1.0, 0.0, 0.0, 1.0),1);
	Sphere s2(vec3gpu(0, 0, -5), 1.0, make_float4(1.0, 0.0, 1.0, 1.0),1);


	w.AddSphere(&s2);
	w.AddSphere(&s1);

	w.AddLight(l1);
	//w.AddModel(modelTris, modelTrisNo);
	w.AddModel(background, 1,make_float4(1.0,1.0,1.0,1.0));
	w.AddModel(background2, 1, make_float4(1.0, 1.0, 1.0, 1.0));
	float4 col = make_float4(0.0, 0.0, 0.0, 1.0);
	
	float xPos = cameraPos.x;
	float yPos = cameraPos.y;
	float zPos = cameraPos.z;
	Ray r(cameraPos,cameraRightDir, (float)x, (float)y, (float)screen_width, (float)screen_height,cameraPos.x-1.0f,cameraPos.x+1.0f,cameraPos.y-1.0f, cameraPos.y+1.0f,cameraPos.z+1.0f,gpucamLookAt);
	


	vec3gpu hitPoint;
	vec3gpu hitNormal;

		
		int sphereIntersect = -1;
		float closestT = 10000;
		sphereIntersect = w.hitClosestSphere(r, closestT);
		float modelT = 0;
		int hitmodel = w.testIntersect(r, l1, hitPoint, hitNormal,modelT);
		if ((modelT < closestT))
		{

			if (hitmodel != -1)
			{
			



			if (w.isPointInWorldShadow(hitPoint, l1))
			{
				col = make_float4(0.0, 0.0, 0.0, 1.0);

			}
			else
			{
				float4 newCol = w.colourModel(hitPoint, hitNormal, hitmodel, l1);
				col.x = newCol.x ;
				col.y = newCol.y ;
				col.z = newCol.z;

			}
		}

		}

		else if (sphereIntersect != -1)
		{
			vec3gpu hitPoint2 = r.origin + (r.dir * closestT);

			//if glass do this,

			//check material; visual artifacts
			//col = w.getColourFromReflect(r, hitPoint2, sphereIntersect, l1, cameraPos);
			float4 newCol = w.Spheres[sphereIntersect]->colourSphere(hitPoint2, cameraPos, l1);
			//else normal colour
			col.x = newCol.x;
			col.y = newCol.y;
			col.z = newCol.z;

			//}

		} 
		else
		{
			//hits nothing
	
		}


		

		
	


	pos[y * screen_width + x] = col;

	


	
}


//glut vars
int prevTime = 0;
int window_width = 480;
int window_height = 480;
int f_count = 0;

//key press
bool wPressed = false;
bool aPressed = false;
bool sPressed = false;
bool dPressed = false;
float cameraSpeed = 0.01;


//mouse handles
float yaw = 90;
float pitch = 0;
int	lastMouseX = 240, lastMouseY = 240;
glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 0.0);
glm::vec3 cameraLookAt = glm::vec3(0.0, 0.0, -1.0);
glm::vec3 cameraDirection = glm::vec3(0.0, 0.0, 0.0);
glm::vec3 rightDir;
vec3gpu gpuRightDir;
vec3gpu gpucamLookAt(0, 0, -1.0);
vec3gpu gpuUpDir;

bool LeftPressed = false;
float* camMatrix;
float* gpucamMatrix;








size_t num_bytes = (window_width * window_height) * 4 * sizeof(float);
CShader* myShader;
cudaDeviceProp deviceProp;
GLuint vao;
GLuint vbo;
GLuint tex, buff;
Model m1;
vec3gpu *modelTrisGPU;
vec3gpu gpucameraPos;


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



__device__ void renderRay(float4* pos, vec3gpu gpucameraPos, float x, float y, float width, float height, float left, float right, float top, float bottom)
{

	
	//sphere intersection

	
	//pos[(int)y * (int)width + (int)x] = make_float4(col, 0.0, 0.0, 1.0);


}
void display()
{
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	
	//generate texture data from cuda
	float4* data_ptr;
	
	

	//get pointer to opengl buffer
	checkCudaErrors(cudaGraphicsMapResources(1, &cuda_tex_resource, 0));
	checkCudaErrors(cudaGraphicsResourceGetMappedPointer((void**)&data_ptr, &num_bytes,
		cuda_tex_resource));

	
	dim3 block(16, 16, 1);
	dim3 grid(window_width / block.x, window_height / block.y, 1);
	addKernel<<<grid, block >>>(data_ptr, window_width, window_height,modelTrisGPU,m1.NUMBER_OF_TRIANGLES,gpucameraPos,gpuRightDir,gpucamLookAt);
	
	
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

		

	m1 = Model("ImageToStl.stl");
	
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
	//std::cout << dt << ',';

	prevTime = curTime;

	
	handleKeys();


	glutPostRedisplay();

}





void keyDown(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
		wPressed = true;
		break;
	case 'a':
		aPressed = true;
		break;
	case 's':
		sPressed = true;
		break;
	case 'd':
		dPressed = true;
		break;

	}
}

void keyUp(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
		wPressed = false;
		break;
	case 'a':
		aPressed = false;
		break;
	case 's':
		sPressed = false;
		break;
	case 'd':
		dPressed = false;
		break;
	}


}



void mouseMove(int x, int y) {

	//Inspired by learnopgl's code: https://learnopengl.com/Getting-started/Camera
	const float sensitivity = 0.05f;
	float xdiff = (x - lastMouseX) * sensitivity;
	float ydiff = (lastMouseY - y) * sensitivity;


	//lastMouseX = x;
	//lastMouseY = y;


	yaw += xdiff;
	pitch += ydiff;

	//allow cursor to stay in centre of screen
	glutWarpPointer(window_width / 2, window_height / 2);




	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction = glm::vec3(0, 0, 0);
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraLookAt = glm::normalize(direction);
		
	
	
	
	glm::mat4 temp = glm::lookAt(cameraPos, cameraPos + cameraLookAt, glm::vec3(0, 1, 0));

	//take the inverse to go cam -> world, transpose to get in row-major order.
	temp = glm::inverseTranspose(temp);
	
	camMatrix = (float*)glm::value_ptr(temp);
	
	
	cudaMalloc((void**)&gpucamMatrix, sizeof(float)*16);
	cudaMemcpy(gpucamMatrix, camMatrix, sizeof(float)*16, cudaMemcpyHostToDevice);

	
	gpucamLookAt.x = cameraPos.x + cameraLookAt.x;
	gpucamLookAt.y = cameraPos.y + cameraLookAt.y;
	gpucamLookAt.z = cameraPos.z + cameraLookAt.z;


	rightDir = glm::normalize(glm::cross(cameraLookAt, glm::vec3(0, 1, 0)));
	gpuRightDir.x = rightDir.x;
	gpuRightDir.y = rightDir.y;
	gpuRightDir.z = rightDir.z;
}




void handleKeys()
{

	if (wPressed )
	{
		cameraPos -= ( cameraLookAt* cameraSpeed);
		glm::vec3 newPos = (cameraLookAt * cameraSpeed);
		gpucameraPos.x -= newPos.x;
		gpucameraPos.y -= newPos.y;
		gpucameraPos.z -= newPos.z;
	}

	if (sPressed )
	{
		cameraPos += (cameraLookAt * cameraSpeed);
		glm::vec3 newPos =  (cameraLookAt * cameraSpeed);
		gpucameraPos.x += newPos.x;
		gpucameraPos.y += newPos.y;
		gpucameraPos.z += newPos.z;
	}

	if (aPressed )
	{

		glm::vec3 cameraLeftMove = glm::normalize(glm::cross(cameraLookAt, glm::vec3(0, 1, 0)));


		glm::vec3 posDiff = (cameraLeftMove * cameraSpeed);
		cameraPos += posDiff;
		gpucameraPos.x += posDiff.x;
		gpucameraPos.y += posDiff.y;
		gpucameraPos.z += posDiff.z;
	}
	if (dPressed )
	{
		glm::vec3 cameraLeftMove = glm::normalize(glm::cross(cameraLookAt, glm::vec3(0, 1, 0)));
		glm::vec3 posDiff = (cameraLeftMove * cameraSpeed);
		cameraPos -= posDiff;
		gpucameraPos.x -= posDiff.x;
		gpucameraPos.y -= posDiff.y;
		gpucameraPos.z -= posDiff.z;
	}




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
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutPassiveMotionFunc(mouseMove);
	glutIdleFunc(idle);


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	


	//ImGui::StyleColorsDark();
	// Setup Platform/Renderer backends
	ImGui_ImplGLUT_Init();
	ImGui_ImplOpenGL3_Init();
	ImGui_ImplGLUT_InstallFuncs();







	//starts the main loop. Program loops and calls callback functions as appropriate.
	init();
	glutMainLoop();
	



	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();



	return 0;

}
