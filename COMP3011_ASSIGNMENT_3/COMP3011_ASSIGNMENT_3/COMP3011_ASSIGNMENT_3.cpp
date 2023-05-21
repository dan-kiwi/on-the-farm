#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "error.h"
#include "file.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "objParser.h"
#include "point.h"
#include "cylinder.h"


#define NUM_BUFFERS 4
#define NUM_VAOS 4
#define WIDTH 640
#define HEIGHT 480
#define CAMERA_START_X -45.f
#define CAMERA_START_Y 0.f
#define CAMERA_TARGET glm::vec3(0.f, -0.5f, 0.f)
#define TROUGH_CENTER point(0.f, 0.f, 0.f)
#define TROUGH_RADIUS 0.22f
#define TROUGH_HEIGHT 0.1f
#define TROUGH_SLICES 18

GLuint Buffers[NUM_BUFFERS];
GLuint VAOs[NUM_VAOS];
SCamera Camera;
int move_sheep;

glm::vec3 lightDirection = glm::vec3(-0.8f, -1.f, -0.8f);
glm::vec3 lightPos = glm::vec3(1.5f, 1.5f, 1.5f);

float background[] = 
{
	//positions			//texture		//normals
	//Ground
	-1.f, -1.f, -1.f,	0.f, 0.f,		0.f, 1.f, 0.f,
	1.f, -1.f, -1.f,	0.f, 1.f,		0.f, 1.f, 0.f,
	1.f, -1.f, 1.f,		0.39f, 1.f,		0.f, 1.f, 0.f,

	-1.f, -1.f, 1.f,	0.39f, 0.f,		0.f, 1.f, 0.f,
	-1.f, -1.f, -1.f,	0.f, 0.f,		0.f, 1.f, 0.f,
	1.f, -1.f, 1.f,		0.39f, 1.f,		0.f, 1.f, 0.f,

	//Sky
	-1.f, -1.f, -1.f,	0.7f, 0.f,		1.f, 0.f, 0.f,
	-1.f, 1.f, -1.f,	0.7f, 1.f,		1.f, 0.f, 0.f,
	-1.f, 1.f, 1.f,		0.4f, 1.f,		1.f, 0.f, 0.f,

	-1.f, -1.f, 1.f,	0.4f, 0.f,		1.f, 0.f, 0.f,
	-1.f, -1.f, -1.f,	0.7f, 0.f,		1.f, 0.f, 0.f,
	-1.f, 1.f, 1.f,		0.4f, 1.f,		1.f, 0.f, 0.f,

	-1.f, -1.f, -1.f,	0.7f, 0.f,		0.f, 0.f, 1.f,
	1.f, -1.f, -1.f,	1.f, 0.f,		0.f, 0.f, 1.f,
	1.f, 1.f, -1.f,		1.f, 1.f,		0.f, 0.f, 1.f,

	-1.f, -1.f, -1.f,	0.7f, 0.f,		0.f, 0.f, 1.f,
	-1.f, 1.f, -1.f,	0.7f, 1.f,		0.f, 0.f, 1.f,
	1.f, 1.f, -1.f,		1.f, 1.f,		0.f, 0.f, 1.f,
};

float hay[] =
{
	//Right
	0.f, 0.f, 0.f,		0.f, 0.f,		-1.f, 0.f, 0.f,
	0.4f, 0.f, 0.f,		0.f, 1.f,		-1.f, 0.f, 0.f,
	0.4f, 0.2f, 0.f,	1.f, 1.f,		-1.f, 0.f, 0.f,

	0.f, 0.f, 0.f,		0.f, 0.f,		-1.f, 0.f, 0.f,
	0.f, 0.2f, 0.f,		1.f, 0.f,		-1.f, 0.f, 0.f,
	0.4f, 0.2f, 0.f,	1.f, 1.f,		-1.f, 0.f, 0.f,
	
	//Bottom
	0.f, 0.f, 0.f,		0.f, 0.f,		0.f, -1.f, 0.f,
	0.4f, 0.f, 0.f,		0.f, 1.f,		0.f, -1.f, 0.f,
	0.4f, 0.f, 0.2f,	1.f, 1.f,		0.f, -1.f, 0.f,

	0.f, 0.f, 0.f,		0.f, 0.f,		0.f, -1.f, 0.f,
	0.f, 0.f, 0.2f,		1.f, 0.f,		0.f, -1.f, 0.f,
	0.4f, 0.f, 0.2f,	1.f, 1.f,		0.f, -1.f, 0.f,

	//Top
	0.f, 0.2f, 0.f,		0.f, 0.f,		0.f, 1.f, 0.f,
	0.4f, 0.2f, 0.f,	0.f, 1.f,		0.f, 1.f, 0.f,
	0.4f, 0.2f, 0.2f,	1.f, 1.f,		0.f, 1.f, 0.f,

	0.f, 0.2f, 0.f,		0.f, 0.f,		0.f, 1.f, 0.f,
	0.f, 0.2f, 0.2f,	1.f, 0.f,		0.f, 1.f, 0.f,
	0.4f, 0.2f, 0.2f,	1.f, 1.f,		0.f, 1.f, 0.f,

	//Left
	0.f, 0.f, 0.2f,		0.f, 0.f,		1.f, 0.f, 0.f,
	0.4f, 0.f, 0.2f,	0.f, 1.f,		1.f, 0.f, 0.f,
	0.4f, 0.2f, 0.2f,	1.f, 1.f,		1.f, 0.f, 0.f,

	0.f, 0.f, 0.2f,		0.f, 0.f,		1.f, 0.f, 0.f,
	0.f, 0.2f, 0.2f,	1.f, 0.f,		1.f, 0.f, 0.f,
	0.4f, 0.2f, 0.2f,	1.f, 1.f,		1.f, 0.f, 0.f,

	//Back
	0.f, 0.f, 0.f,		0.f, 0.f,		0.f, 0.f, -1.f,
	0.f, 0.2f, 0.f,		0.f, 1.f,		0.f, 0.f, -1.f,
	0.f, 0.2f, 0.2f,	1.f, 1.f,		0.f, 0.f, -1.f,

	0.f, 0.f, 0.f,		0.f, 0.f,		0.f, 0.f, -1.f,
	0.f, 0.f, 0.2f,		1.f, 0.f,		0.f, 0.f, -1.f,
	0.f, 0.2f, 0.2f,	1.f, 1.f,		0.f, 0.f, -1.f,

	//Front
	0.4f, 0.f, 0.f,		0.f, 0.f,		0.f, 0.f, 1.f,
	0.4f, 0.2f, 0.f,	0.f, 1.f,		0.f, 0.f, 1.f,
	0.4f, 0.2f, 0.2f,	1.f, 1.f,		0.f, 0.f, 1.f,

	0.4f, 0.f, 0.f,		0.f, 0.f,		0.f, 0.f, 1.f,
	0.4f, 0.f, 0.2f,	1.f, 0.f,		0.f, 0.f, 1.f,
	0.4f, 0.2f, 0.2f,	1.f, 1.f,		0.f, 0.f, 1.f,
};


void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float x_offset = 0.f;
	float y_offset = 0.f;
	bool cam_changed = false;
	if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT)
	{
		x_offset = 1.f;
		y_offset = 0.f;
		cam_changed = true;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT)
	{
		x_offset = -1.f;
		y_offset = 0.f;
		cam_changed = true;
	}
	if (key == GLFW_KEY_UP && action == GLFW_REPEAT)
	{
		x_offset = 0.f;
		y_offset = -1.f;
		cam_changed = true;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT)
	{
		x_offset = 0.f;
		y_offset = 1.f;
		cam_changed = true;
	}
	if (key == GLFW_KEY_R && action == GLFW_REPEAT)
	{
		cam_dist -= 0.1f;
		cam_changed = true;
	}
	if (key == GLFW_KEY_F && action == GLFW_REPEAT)
	{
		cam_dist += 0.1f;
		cam_changed = true;
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_REPEAT)
	{
		move_sheep = 1;
	}
	else 
	{
		move_sheep = 0;
	}

	if (cam_changed)
	{
		MoveAndOrientCamera(Camera, CAMERA_TARGET, cam_dist, x_offset, y_offset);
	}
}

void ResizeCallback(GLFWwindow*, int w, int h)
{
	glViewport(0, 0, w, h);
}


int main()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "On The Farm", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetWindowSizeCallback(window, ResizeCallback);

	gl3wInit();

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(DebugCallback, 0);

	unsigned int shaderProgram = CompileShader("triangle.vert", "triangle.frag");

	InitCamera(Camera);
    MoveAndOrientCamera(Camera, CAMERA_TARGET, cam_dist, CAMERA_START_X, CAMERA_START_Y);

	GLuint sheepTexture = setup_texture("sheep_texture.bmp");
	GLuint backgroundTexture = setup_texture("background_texture.bmp");
    GLuint troughTexture = setup_texture("trough_texture.bmp");
	GLuint hayTexture = setup_texture("hay_texture.bmp");


	//Water trough
	point center = point(0.f, 0.f, 0.f);
	std::vector<point> trough_points = createCircle(TROUGH_CENTER, TROUGH_RADIUS, TROUGH_SLICES);
	int num_curve_verts = 0;
	int num_curve_floats = 0;
	float* curve_vertices = MakeCyclinder(trough_points, TROUGH_HEIGHT, num_curve_verts, num_curve_floats);

	glCreateBuffers(NUM_BUFFERS, Buffers);
	glGenVertexArrays(NUM_VAOS, VAOs);

	//Sheep buffer
	float* sheep = objParser("sheep1.obj");
	glNamedBufferStorage(Buffers[0], 46432, sheep, 0);
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Background buffer
	glNamedBufferStorage(Buffers[1], 576, background, 0);
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Water trough buffer
	glNamedBufferStorage(Buffers[2], sizeof(float) * num_curve_floats, curve_vertices, 0);
	glBindVertexArray(VAOs[2]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Hay buffer
	glNamedBufferStorage(Buffers[3], 1152, hay, 0);
	glBindVertexArray(VAOs[3]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[3]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glEnable(GL_DEPTH_TEST);

	float sheep_angle = 0.f;
	move_sheep = 0;
	float hay_position_x = 0.5f;
	int move_hay_right = 1;
	while (!glfwWindowShouldClose(window))
	{
		static const GLfloat bgd[] = { 1.f, 1.f, 1.f, 1.f };
		glClearBufferfv(GL_COLOR, 0, bgd);
		glClear(GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glUseProgram(shaderProgram);

		glm::mat4 model = glm::mat4(1.f);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glm::mat4 view = glm::mat4(1.f);
		view = glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection = glm::mat4(1.f);
		projection = glm::perspective(glm::radians(45.f), (float)WIDTH / (float)HEIGHT, .1f, 10.f);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glUniform3f(glGetUniformLocation(shaderProgram, "lightDirection"), lightDirection.x, lightDirection.y, lightDirection.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "lightColour"), 0.f, 0.f, 1.f);
		glUniform3f(glGetUniformLocation(shaderProgram, "camPos"), Camera.Position.x, Camera.Position.y, Camera.Position.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

		//Background
		glBindTexture(GL_TEXTURE_2D, backgroundTexture);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 18);

		//Water trough
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(0.f, -0.92f, 0.f));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, troughTexture);
		glBindVertexArray(VAOs[2]);
		glDrawArrays(GL_TRIANGLES, 0, num_curve_verts);

		//Sheep
		model = glm::mat4(1.f);
		model = glm::rotate(model, glm::radians(sheep_angle), glm::vec3(0.f, 1.f, 0.f));
		model = glm::translate(model, glm::vec3(0.6f, -0.96f, 0.f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, sheepTexture);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 1451);
		if (move_sheep)
			sheep_angle += 0.5f;

		//Hay
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(hay_position_x, -0.99f, -0.85f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, hayTexture);
		glBindVertexArray(VAOs[3]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		if (move_hay_right)
		{
			hay_position_x += 0.002f;
			if (hay_position_x > 0.7f)
				move_hay_right = 0;
		}
		else
		{
			hay_position_x -= 0.002f;
			if (hay_position_x < 0.f)
				move_hay_right = 1;
		}

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}