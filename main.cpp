#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "dependente\glew\glew.h"
#include "dependente\glfw\glfw3.h"
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"
#include "Sphere.h"
#include "Cylinder.h"
#include "shader.hpp"
#include "Camera/camera.h"



bool temp = false;
bool aux = false;
bool tiltCyls = false;
float counter = 0.0f;
void mouse_button_callback(GLFWwindow* window, int button, int action, int models);
void proccessedInput(GLFWwindow* window);

GLFWwindow* window;
const int width = 1024, height = 1024;

glm::vec3 lightColor(1.0f);

//Time
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//Add the camera parameters here and create a camera object

glm::vec3 cameraPos = glm::vec3(0.0f, 0.3f, -50.0f);
glm::vec3 cameraDir = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 0.1f, 0.0f);

Camera camera(cameraPos, cameraDir, cameraUp);
const float radius = 200.0f;



//Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraViewDirection, glm::vec3 cameraUp)

void window_callback(GLFWwindow* window, int new_width, int new_height)
{
	glViewport(0, 0, new_width, new_height);
}



int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "3D demo", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}


	glViewport(0, 0, width, height);

	// Pink Panther background
	glClearColor(1.0f, 0.7f, 0.8f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	glm::vec3 positions[] =
	{
		glm::vec3(-0.2f, 0.04f, 210.0f),		// top right
		glm::vec3(-0.07f, 0.04f, 210.0f),		// left of top right
		glm::vec3(0.07f, 0.04f, 210.0f),		// right of top left
		glm::vec3(0.2f, 0.04f, 210.0f),			// top right
		glm::vec3(-0.13f, 0.04f, 205.0f),		// under top right
		glm::vec3(0.0f, 0.04f, 205.0f),			// center pin
		glm::vec3(0.13f, 0.04f, 205.0f),		//under top left
		glm::vec3(-0.07f, 0.04f, 200.0f),		// right of head pin
		glm::vec3(0.07f, 0.04f, 200.0f),		// left of head pin
		glm::vec3(0.0f, 0.04f, 195.0f)			// head pin
	};


	GLfloat vertices[] =
	{
		 0.3f, 0.0f, 220.0f,
		 0.3f, 0.0f, -5.0f,
		-0.3f, 0.0f, 220.0f,
		-0.3f, 0.0f, -5.0f,
	};


	GLuint indices[] =
	{
		0, 1, 2,
		1 , 2, 3

	};


	Sphere sphere = Sphere(0.05f, 40, 40);
	Cylinder cylinder = Cylinder(0.015f, 0.015f, 0.23f, 10, 12, true);
	GLuint vboSphere, vaoSphere, iboSphere, vboCyl, vaoCyl, iboCyl, vboAlley, vaoAlley, iboAlley;

	//sphere
	glGenVertexArrays(1, &vaoSphere);
	glGenBuffers(1, &vboSphere);
	glGenBuffers(1, &iboSphere);

	glBindVertexArray(vaoSphere);

	glBindBuffer(GL_ARRAY_BUFFER, vboSphere);
	glBufferData(GL_ARRAY_BUFFER, sphere.getInterleavedVertexSize(), sphere.getInterleavedVertices(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboSphere);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.getIndexSize(), sphere.getIndices(), GL_STATIC_DRAW);

	int sphStride = sphere.getInterleavedStride();


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sphStride, (void*)0);
	glEnableVertexAttribArray(0);

	//Unbin
	glBindVertexArray(0);

	//---------------------------------------------------------------------------------------------------

	//cylinder
	glGenVertexArrays(1, &vaoCyl);
	glGenBuffers(1, &vboCyl);
	glGenBuffers(1, &iboCyl);

	glBindVertexArray(vaoCyl);

	glBindBuffer(GL_ARRAY_BUFFER, vboCyl);
	glBufferData(GL_ARRAY_BUFFER, cylinder.getInterleavedVertexSize(), cylinder.getInterleavedVertices(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboCyl);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cylinder.getIndexSize(), cylinder.getIndices(), GL_STATIC_DRAW);

	int cylStride = cylinder.getInterleavedStride();


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, cylStride, (void*)0);
	glEnableVertexAttribArray(0);

	//Unbind
	glBindVertexArray(0);

	//---------------------------------------------------------------------------------------------------

	//alley
	glGenVertexArrays(1, &vaoAlley);
	glGenBuffers(1, &vboAlley);
	glGenBuffers(1, &iboAlley);

	glBindVertexArray(vaoAlley);

	glBindBuffer(GL_ARRAY_BUFFER, vboAlley);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboAlley);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Unbind
	glBindVertexArray(0);

	//---------------------------------------------------------------------------------------------------

	glfwSetFramebufferSizeCallback(window, window_callback);

	//Change accordingly for camera
	glm::mat4 view;

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);



	//glPolygonmodele(GL_FRONT_AND_BACK, GL_LINE); //wireframe 
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		proccessedInput(window);
		glfwSetMouseButtonCallback(window, mouse_button_callback);

		glm::mat4 view = glm::lookAt(camera.getCameraPosition(), camera.getCameraPosition() + camera.getCameraViewDirection(), camera.getCameraUp());

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwSwapBuffers(window);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);


		// render alley
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 matrix = projection * view * model;

		unsigned int transformLoc = glGetUniformLocation(programID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));

		glm::vec4 color = glm::vec4(0.6, 0.28, 0.0f, 1.0f);
		unsigned int transformLoc2 = glGetUniformLocation(programID, "color");
		glUniform4fv(transformLoc2, 1, glm::value_ptr(color));


		glBindVertexArray(vaoAlley);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//---------------------------------------------------------------------

		//cylinder stuff


		for (int i = 0; i < 10; i++)
		{

			//checks for "collision"
			if (counter > 195.0f)
			{
				counter = 0.0f;
				tiltCyls = true;
				aux = false;

			}

			glm::mat4 matrix;
			//keeps cylinders upright
			if (tiltCyls == false)
			{
				matrix = glm::translate(matrix, positions[i]);
				matrix = glm::rotate(matrix, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			}

			//tips cilinders over
			else
			{
				matrix = glm::translate(matrix, positions[i]);
				matrix = glm::rotate(matrix, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			}

			//render cylinders

			glm::mat4 mpv = projection * view * matrix;
			unsigned int transformLoc = glGetUniformLocation(programID, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mpv));

			unsigned int transformLoc2 = glGetUniformLocation(programID, "color");
			glUniform4fv(transformLoc2, 1, glm::value_ptr(lightColor));

			//glBindVertexArray(0);
			glBindVertexArray(vaoCyl);
			glDrawElements(GL_TRIANGLES, cylinder.getIndexSize(), GL_UNSIGNED_INT, 0);
		}

		//---------------------------------------------------------------------

		//sphere stuff

		// Ball not moving / in initial positon
		if (aux == false && tiltCyls == false)
		{
			model = glm::translate(model, glm::vec3(0.0f, 0.05f, 0.0f));
		}


		// Ball moving towards cylinders
		else if (aux == true && tiltCyls == false)
		{
			model = glm::translate(model, glm::vec3(0.0f, 0.05f, counter));
			model = glm::rotate(model, (float)glfwGetTime() * 100, glm::vec3(0.0f, 0.0f, 1.0f));
			counter += 0.7;
		}


		// ball stops at end position so cyilnders tilt
		else if (aux == false && tiltCyls == true)
		{
			model = glm::translate(model, glm::vec3(0.0f, 0.05f, 190.0f));
		}


		//render sphere

		glm::mat4 matrix3 = projection * view * model;

		unsigned int transformLoc3 = glGetUniformLocation(programID, "transform");
		glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(matrix3));

		color = glm::vec4(0.5f + 1 / 10.0f, 1 - 0 / 10.0f, 0.5f, 1.0);
		unsigned int transformLoc4 = glGetUniformLocation(programID, "color");
		glUniform4fv(transformLoc4, 1, glm::value_ptr(color));
		glBindVertexArray(vaoSphere);

		glDrawElements(GL_TRIANGLES, sphere.getIndexSize(), GL_UNSIGNED_INT, 0);
	}

	// Janitor

	glDeleteBuffers(1, &vboSphere);
	glDeleteBuffers(1, &iboSphere);
	glDeleteVertexArrays(1, &vaoSphere);

	glDeleteBuffers(1, &vboCyl);
	glDeleteBuffers(1, &iboCyl);
	glDeleteVertexArrays(1, &vaoCyl);

	glDeleteBuffers(1, &vboAlley);
	glDeleteBuffers(1, &iboAlley);
	glDeleteVertexArrays(1, &vaoAlley);

	glDeleteProgram(programID);

	glfwTerminate();
	return 0;
}




// CALL BACK AND KEYBOARD INPUT STUFF:

void mouse_button_callback(GLFWwindow* window, int button, int action, int models)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		aux = true;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		aux = false;
		tiltCyls = false;
		counter = 0.0f;
	}
}


void proccessedInput(GLFWwindow* window)
{
	float cameraSpeed = deltaTime * 5;

	if (glfwGetKey(window, GLFW_KEY_W))
	{
		camera.keyboardMoveFront(cameraSpeed / 8);
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		camera.keyboardMoveBack(cameraSpeed / 8);
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE))
	{
		camera.keyboardMoveUp(cameraSpeed / 10);
	}

	if (glfwGetKey(window, GLFW_KEY_C))
	{
		camera.keyboardMoveDown(cameraSpeed / 10);
	}

	if (glfwGetKey(window, GLFW_KEY_A))
	{
		camera.rotateOy(cameraSpeed / 2);
	}

	if (glfwGetKey(window, GLFW_KEY_D))
	{
		camera.rotateOy(-cameraSpeed / 2);
	}

	if (glfwGetKey(window, GLFW_KEY_R))
	{
		camera.rotateOx(cameraSpeed / 5);
	}

	if (glfwGetKey(window, GLFW_KEY_F))
	{
		camera.rotateOx(-cameraSpeed / 5);
	}
}