
//#include<GL/gl.h>
//#include<GL/glu.h>
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/string_cast.hpp>
#include<glm/ext.hpp>
#include "shader.hpp"
#include <shader.h>
#include <window.h>
#include <mesh.h>

#include<iostream>

#include <stdlib.h>
static bool autoRotate=1;

#include<stdio.h>
#include<SDL3/SDL.h>

#include<glad/glad.h>

#include <memory>

Window window;

#ifdef __EMSCRIPTEN__
#include<emscripten.h>
#endif

#define XLEN 512
#define YLEN 512
using namespace glm;

static float reqAngleX=0.0;
static float reqAngleY=0.0;
static float reqAngleZ=0.0;

static float currentAngleX=45.0;
static float currentAngleY=45.0;
static float currentAngleZ=45.0;

static float autodelay=1.0;

extern "C"{
	void setAngleX(const double ucx){
		reqAngleX=ucx;
	}
	void setAngleY(const double ucy){
		reqAngleY=ucy;
	}
	void setAngleZ(const double ucz){
		reqAngleZ=ucz;
	}

	void setRotationSpeed(const float delay){
		autodelay=delay;
	}

	void checkbox(){
		autoRotate = !autoRotate;
	}
}

static const GLfloat vertexData[]={
	-1,-1,-1,1,
	+1,-1,-1,1,
	+1,+1,-1,1,
	-1,+1,-1,1,
	-1,-1,+1,1,
	-1,+1,+1,1,
	+1,+1,+1,1,
	+1,-1,+1,1,
};

static GLuint elements[]={
	0,1,2,
	2,3,0,
	0,3,4,
	4,3,5,
	5,6,4,
	6,7,4,
	4,7,0,
	0,7,1,
	1,7,6,
	6,2,1,
	3,2,6,
	6,5,3,
};

static const GLfloat colorData[]={
	1,0,0,1,
	0,1,0,1,
	0,0,1,1,
	0,1,1,1,
	1,1,0,1,
	1,0,1,1,
	1,0,0,1,
	0,1,1,1,
};

GLuint elementsBuf;
GLuint vertexBuf;
GLuint colorBuffer;
GLuint program;
int width=XLEN;
int height=YLEN;
glm::mat4 viewPortMat;
glm::mat4 Projection;
glm::mat4 View;
mat4 identityMat;
mat4 cameraRotMat;
vec3 cameraPos;
GLuint viewPortLoc;
GLuint cameraRotLoc;
void initScene(){
	//FOV,aspect ratio,znear,zfar
	Projection = glm::perspective(glm::radians(60.0f),(float)width/(float)height,1.0f,1024.0f);
	//ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f);
	identityMat=mat4(1.0f);
	cameraRotMat=identityMat;
	//cameraPos=vec3(4,3,3); 
	cameraPos=vec3(4,4,0); 
	 
	// Camera matrix
	View = glm::lookAt(
		cameraPos,				//camera location
		glm::vec3(0,0,0), //location of origin
		glm::vec3(0,0,1)  //up vector
	);
		
	viewPortMat = Projection * View;
}
void updateScene(){

	if(reqAngleX!=currentAngleX){
		cameraRotMat=rotate(cameraRotMat,radians(reqAngleX-currentAngleX),vec3(0,1,0));
		currentAngleX=reqAngleX;
	}
	if(reqAngleY!=currentAngleY){
		cameraRotMat=rotate(cameraRotMat,radians(reqAngleY-currentAngleY),vec3(0,1,0));
		currentAngleY=reqAngleY;
	}
	if(reqAngleZ!=currentAngleZ){
		cameraRotMat=rotate(cameraRotMat,radians(reqAngleZ-currentAngleZ),vec3(0,0,1));
		currentAngleZ=reqAngleZ;
	}
	if(autoRotate){
		cameraRotMat=rotate(cameraRotMat,radians(autodelay),vec3(1,0,0));
//		std::cout<<glm::to_string(cameraRotMat)<<std::endl;

	}
	glUniformMatrix4fv(cameraRotLoc,1,GL_FALSE,value_ptr(cameraRotMat));

	glUniformMatrix4fv(viewPortLoc,1,GL_FALSE,value_ptr(viewPortMat));
}

Geometry geometry;
Material material;
void initGL(){
	glGenBuffers(1,&elementsBuf);
	glGenBuffers(1,&vertexBuf);
	glGenBuffers(1,&colorBuffer);

	initScene();

	Shader vertexShader, fragmentShader;
	vertexShader.loadFromFile("./shader/test.vs", Shader::vertex);
	fragmentShader.loadFromFile("./shader/test.fs", Shader::fragment);

	printf("Linking program\n");
	material.setShaders(vertexShader, fragmentShader);
	printf("Linked program\n");


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsBuf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements,GL_STATIC_DRAW);

	glEnableVertexAttribArray(material.getAttributeLocation("position"));

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuf);
	//glBindAttribLocation(program,0,"vertexBuf");
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glVertexAttribPointer(
		material.getAttributeLocation("position"),
		4,						//coordinates per vertex
		GL_FLOAT,			//type
		GL_FALSE,			//normalized?
		0,						//stride
		0							//buffer offset
	);

	glEnableVertexAttribArray(material.getAttributeLocation("colorBuffer"));

	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	//glBindAttribLocation(program,1,"colorBuffer");
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);
	glVertexAttribPointer(
		glGetAttribLocation(material._handle,"colorBuffer"),
		4,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);

	viewPortLoc=material.getUniformLocation("viewPort");
	cameraRotLoc=material.getUniformLocation("cameraRot");
//	glDisableVertexAttribArray(0);
}

void drawScreen(){
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glUseProgram(material._handle);
	updateScene();
	//glDrawArrays(GL_TRIANGLES,0,3);
	glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);
	SDL_GL_SwapWindow(window._window);
	// SDL_GL_SwapBuffers();
}

static bool handle_key_down(SDL_KeyboardEvent* keyEvent){
	switch( keyEvent -> keysym.sym  ) {
		case SDLK_ESCAPE:
			SDL_Quit( );
			return true;
		break;
		case SDLK_SPACE:
			autoRotate = !autoRotate;
		break;
		case SDLK_UP:
			reqAngleZ++;
		break;
		case SDLK_DOWN:
			reqAngleZ--;
		break;
		case SDLK_RIGHT:
			reqAngleY++;
		break;
		case SDLK_LEFT:
			reqAngleY--;
		break;
		default:
		break;
	}
	return false;
}

static bool processEvents(){
    SDL_Event event;

    /* Grab all the events off the queue. */
    while( SDL_PollEvent( &event ) ) {

        switch( event.type ) {
        case SDL_EVENT_KEY_DOWN:
            /* Handle key presses. */
            return handle_key_down( &event.key );
            break;
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        case SDL_EVENT_QUIT:
            SDL_Quit();
            std::exit(0);
			return true;
        }
    }

	return false;
}

void main_loop(){
	bool running = true;
	while(running) {
		running = !processEvents();
		drawScreen();
	}
}

int main(){
	int width;
	int height;
	setvbuf(stdout, NULL, _IONBF, 0);
	width = XLEN;
	height = YLEN;

	window.ccw = false;
	window.init("Open GL", width, height);
	printf("Initialized window\n");
	initGL();
	printf("Initialized GL stuff\n");

	printf("Starting main loop\n");
	main_loop();
	// emscripten_set_main_loop(main_loop,0,0);
//	emscripten_set_main_loop_timing(EM_TIMING_RAF,10);
	printf("Rotating 3D cube written in C++ using SDL and OpenGL.\n");
	printf("Press spacebar to pause rotation.\n");
	printf("Press up/down to rotate up/down.\n");
	printf("Press left/right to rotate left/right.\n");
	return 0;
}