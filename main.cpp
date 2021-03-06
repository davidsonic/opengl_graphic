#define STB_IMAGE_IMPLEMENTATION

#include <cstdio>

#include <cstring>
#include <cmath>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <custom/Mesh.h>
#include <custom/Shader.h>
#include <custom/Window.h>
#include <custom/Camera.h>
#include  <custom/Texture.h>

Texture brickTexture;
Texture dirtTexture;


std::vector<Mesh*>meshList;
std::vector<Shader>shaderList;
Window mainWindow;
Camera camera;

GLfloat deltaTime=0.0f;
GLfloat lastTime=0.0f;

const float toRadians= 3.14159265f/180.0f;

bool direction=true;
float triOffset=0.0f;
float triMaxoffset=0.7f;
float triIncrement=0.005f;

float curAngle=0.0f;

float sizeDirection=true;
float curSize=0.4f;
float maxSize=0.8f;
float minSize=0.1f;



static const char* vShader="shaders/shader.vert";
static const char* fShader= "shaders/shader.frag";


void CreateObjects(){
    unsigned  int indices[]= {
            0, 3, 1,
            1, 3, 2,
            2,3,0,
            0,1,2
    };

	GLfloat vertices[]={
		-1.0f, -1.0f,0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f, 0.5f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.5f, 1.0f
	};


	Mesh * obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 20, 12);
	meshList.push_back(obj1);

	Mesh * obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 20, 12);
	meshList.push_back(obj2);


}

void CreateShaders(){
	Shader* shader1=new Shader();
//	shader1->CreateFromString(vShader, fShader);
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


int main(){

	mainWindow= Window(800,600);
	mainWindow.Initialize();

	CreateObjects();
	CreateShaders();

	camera=Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);
	brickTexture=Texture("Textures/brick.png");
	brickTexture.LoadTexture();

	dirtTexture=Texture("Textures/dirt.png");
	dirtTexture.LoadTexture();

	GLuint uniformProjection=0, uniformModel=0, uniformView=0;
	glm::mat4 projection =glm::perspective(45.0f,(GLfloat)mainWindow.getBufferWidth()/(GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.f);
	
	while(!mainWindow.getShouldClose()){

			GLfloat now= glfwGetTime();
			deltaTime= now - lastTime;
			lastTime= now;

			glfwPollEvents();

			camera.keyControl(mainWindow.getKeys(), deltaTime);
			camera.mouseControl(mainWindow.getXchange(), mainWindow.getYchange());

			if(direction){
			    triOffset += triIncrement;
			}else{
			    triOffset-=triIncrement;
			}


			if(std::abs(triOffset) >= triMaxoffset)
			    direction=!direction;

			curAngle+=0.1f;
			if(curAngle>=360)
			    curAngle-=360;

			if(sizeDirection)
			    curSize+=0.01f;
			else
			    curSize-=0.01f;

			if( curSize >= maxSize || curSize <=minSize)
			    sizeDirection=!sizeDirection;

			glClearColor(0.0f, 0.0f, 0.0f,1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shaderList[0].UseShader();
			uniformModel= shaderList[0].GetModelLocation();
			uniformProjection=shaderList[0].GetProjectionLocation();
			uniformView=shaderList[0].GetViewLocation();


            glm::mat4 model;
            model= glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
            model=glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            brickTexture.UseTexture();
			meshList[0]->RenderMesh();

			model=glm::mat4();
			model=glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
			model=glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			dirtTexture.UseTexture();
			meshList[1]->RenderMesh();

			glUseProgram(0);
			mainWindow.swapBuffers();
	}

	return 0;
}
