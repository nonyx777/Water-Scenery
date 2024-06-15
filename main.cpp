#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/Shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "include/stb_image.h"
#include "include/Model.hpp"
#include "include/VAO.hpp"
#include "include/VBO.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_cursor_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
float clamp(double min, double max, double value);
float convertAngle(float offset);
unsigned int loadTexture(char const *path);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
double lastX, lastY;
bool firstMouse = true;
float angle_x{0.f};
float angle_y{0.f};
const float sensetivity_x = 7.f;
const float sensetivity_y = 5.f;
bool can_rotate = false;

// function declerations
void renderWater(Shader &waterShader, VAO waterVAO, VBO waterVBO, glm::vec3 view_pos);
void renderPool(Shader &poolShader, VAO poolVAO, VBO poolVBO, glm::vec3 view_pos, glm::vec3 light_dir, uint poolDiffuseMap, uint poolNormalMap);
void setupReflectionBuffer(uint &reflectionFramebuffer, uint &reflectionColorbuffer, uint &reflectionRenderbuffer);
void setupRefractionBuffer(uint &refractionFramebuffer, uint &refractionColorbuffer, uint &refractionRenderbuffer);

// positions
glm::vec3 pos1(-1.f, 1.f, 0.f);
glm::vec3 pos2(-1.f, -1.f, 0.f);
glm::vec3 pos3(1.f, -1.f, 0.f);
glm::vec3 pos4(1.f, 1.f, 0.f);
// texture coords
glm::vec2 uv1(0.f, 1.f);
glm::vec2 uv2(0.f, 0.f);
glm::vec2 uv3(1.f, 0.f);
glm::vec2 uv4(1.f, 1.f);

GLfloat waterVertices[] = {
	pos1.x, pos1.y, pos1.z, uv1.x, uv1.y,
	pos2.x, pos2.y, pos2.z, uv2.x, uv2.y,
	pos3.x, pos3.y, pos3.z, uv3.x, uv2.y,

	pos1.x, pos1.y, pos1.z, uv1.x, uv1.y,
	pos3.x, pos3.y, pos3.z, uv3.x, uv3.y,
	pos4.x, pos4.y, pos4.z, uv4.x, uv4.y};

float poolVertices[] = {
	// Back face
	-1.f, -1.f, -1.f, 0.0f, 0.0f, // Bottom-left
	1.f, 1.f, -1.f, 1.0f, 1.0f,	  // top-right
	1.f, -1.f, -1.f, 1.0f, 0.0f,  // bottom-right
	1.f, 1.f, -1.f, 1.0f, 1.0f,	  // top-right
	-1.f, -1.f, -1.f, 0.0f, 0.0f, // bottom-left
	-1.f, 1.f, -1.f, 0.0f, 1.0f,  // top-left
	// Front face
	-1.f, -1.f, 1.f, 0.0f, 0.0f, // bottom-left
	1.f, -1.f, 1.f, 1.0f, 0.0f,	 // bottom-right
	1.f, 1.f, 1.f, 1.0f, 1.0f,	 // top-right
	1.f, 1.f, 1.f, 1.0f, 1.0f,	 // top-right
	-1.f, 1.f, 1.f, 0.0f, 1.0f,	 // top-left
	-1.f, -1.f, 1.f, 0.0f, 0.0f, // bottom-left
	// Left face
	-1.f, 1.f, 1.f, 1.0f, 0.0f,	  // top-right
	-1.f, 1.f, -1.f, 1.0f, 1.0f,  // top-left
	-1.f, -1.f, -1.f, 0.0f, 1.0f, // bottom-left
	-1.f, -1.f, -1.f, 0.0f, 1.0f, // bottom-left
	-1.f, -1.f, 1.f, 0.0f, 0.0f,  // bottom-right
	-1.f, 1.f, 1.f, 1.0f, 0.0f,	  // top-right
								  // Right face
	1.f, 1.f, 1.f, 1.0f, 0.0f,	  // top-left
	1.f, -1.f, -1.f, 0.0f, 1.0f,  // bottom-right
	1.f, 1.f, -1.f, 1.0f, 1.0f,	  // top-right
	1.f, -1.f, -1.f, 0.0f, 1.0f,  // bottom-right
	1.f, 1.f, 1.f, 1.0f, 0.0f,	  // top-left
	1.f, -1.f, 1.f, 0.0f, 0.0f,	  // bottom-left
	// Bottom face
	-1.f, -1.f, -1.f, 0.0f, 1.0f, // top-right
	1.f, -1.f, -1.f, 1.0f, 1.0f,  // top-left
	1.f, -1.f, 1.f, 1.0f, 0.0f,	  // bottom-left
	1.f, -1.f, 1.f, 1.0f, 0.0f,	  // bottom-left
	-1.f, -1.f, 1.f, 0.0f, 0.0f,  // bottom-right
	-1.f, -1.f, -1.f, 0.0f, 1.0f  // top-right
};

struct ForShader
{
	glm::vec3 view_position = glm::vec3(0.f, -0.5f, -5.f);
	glm::vec3 light_position = glm::vec3(0.f, 0.f, 2.f);
	glm::vec3 light_direction = glm::vec3(0.f, 2.f, 3.f);
};

const float radius = 10.f;

unsigned int reflectionFramebuffer, reflectionColorbuffer, reflectionRenderbuffer;
unsigned int refractionFramebuffer, refractionColorbuffer, refractionRenderbuffer;

int main()
{
	//....
	ForShader forShader;

	// initialize glfw
	glfwInit();
	if (glfwInit() == GL_FALSE)
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// window creation
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// registering callback functions
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_cursor_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// handling mouse movement variables
	lastX = SCR_WIDTH / 2.0;
	lastY = SCR_HEIGHT / 2.0;

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model)
	stbi_set_flip_vertically_on_load(true);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//...
	Shader waterShader = Shader("./resource/water.vert", "./resource/water.frag");
	Shader poolShader = Shader("./resource/pool.vert", "./resource/pool.frag");

	// water
	VAO waterVAO = VAO();
	VBO waterVBO = VBO(waterVertices, sizeof(waterVertices));
	waterVAO.bind();
	waterVAO.linkVBO(waterVBO, 0, 1);
	waterVAO.unbind();

	// pool
	VAO poolVAO = VAO();
	VBO poolVBO = VBO(poolVertices, sizeof(poolVertices));
	poolVAO.bind();
	poolVAO.linkVBO(poolVBO, 0, 1);
	poolVAO.unbind();

	// textures
	unsigned int poolDiffuseMap = loadTexture("./resource/textures/painted_plaster_wall_diff_2k.jpg");
	unsigned int poolNormalMap = loadTexture("./resource/textures/painted_plaster_wall_nor_gl_2k.jpg");

	// framebuffers
	setupReflectionBuffer(reflectionFramebuffer, reflectionColorbuffer, reflectionRenderbuffer);
	setupRefractionBuffer(refractionFramebuffer, refractionColorbuffer, refractionRenderbuffer);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// reflection framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, reflectionFramebuffer);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDisable(GL_CULL_FACE);
		renderWater(waterShader, waterVAO, waterVBO, forShader.view_position);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		renderPool(poolShader, poolVAO, poolVBO, forShader.view_position, forShader.light_direction, poolDiffuseMap, poolNormalMap);

		// refraction framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, refractionFramebuffer);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_CULL_FACE);
		renderWater(waterShader, waterVAO, waterVBO, forShader.view_position);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		renderPool(poolShader, poolVAO, poolVBO, forShader.view_position, forShader.light_direction, poolDiffuseMap, poolNormalMap);

		// default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_CULL_FACE);
		renderWater(waterShader, waterVAO, waterVBO, forShader.view_position);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		renderPool(poolShader, poolVAO, poolVBO, forShader.view_position, forShader.light_direction, poolDiffuseMap, poolNormalMap);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	poolShader.Delete();
	waterShader.Delete();
	waterVAO.Delete();
	waterVBO.Delete();
	poolVAO.Delete();
	poolVBO.Delete();
	glDeleteBuffers(1, &reflectionFramebuffer);
	glDeleteBuffers(1, &reflectionColorbuffer);
	glDeleteBuffers(1, &reflectionRenderbuffer);
	glDeleteBuffers(1, &refractionFramebuffer);
	glDeleteBuffers(1, &refractionColorbuffer);
	glDeleteBuffers(1, &refractionRenderbuffer);

	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_cursor_callback(GLFWwindow *window, double xpos, double ypos)
{
	lastY = SCR_HEIGHT / 2.0;
	lastX = SCR_WIDTH / 2.0;

	if (!can_rotate)
		return;

	if (firstMouse)
	{
		firstMouse = false;
		return;
	}

	angle_x = convertAngle(xpos - lastX);
	angle_y = convertAngle(lastY - ypos);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		can_rotate = true;
	else
		can_rotate = false;
}

float clamp(double min, double max, double value)
{
	if (value < min)
		return min;
	if (value > max)
		return max;

	return value;
}

float convertAngle(float offset)
{
	// SCR_WIDTH = 360 degrees
	float angle_deg = 360.f * offset / 800.f;
	return glm::radians(angle_deg);
}

// function definitions
void renderWater(Shader &waterShader, VAO waterVAO, VBO waterVBO, glm::vec3 view_pos)
{
	waterShader.use();

	glm::mat4 model = glm::mat4(1.f);
	glm::mat4 view = glm::mat4(1.f);
	glm::mat4 projection = glm::mat4(1.f);

	model = glm::translate(model, glm::vec3(0.f, -0.9f, 0.f));
	model = glm::rotate(model, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
	view = glm::translate(view, view_pos);
	view = glm::rotate(view, glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f));
	view = glm::rotate(view, glm::radians(angle_x) * sensetivity_x, glm::vec3(0.f, 1.f, 0.f));
	projection = glm::perspective(glm::radians(45.f), float(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.f);

	glm::mat4 transform = projection * view * model;
	glUniformMatrix4fv(glGetUniformLocation(waterShader.id, "transform"), 1, GL_FALSE, glm::value_ptr(transform));

	waterVAO.bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflectionColorbuffer);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refractionColorbuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	waterVAO.unbind();
}

void renderPool(Shader &poolShader, VAO poolVAO, VBO poolVBO, glm::vec3 view_pos, glm::vec3 light_dir, uint poolDiffuseMap, uint poolNormalMap)
{
	poolShader.use();

	// transormation
	glm::mat4 model = glm::mat4(1.f);
	glm::mat4 view = glm::mat4(1.f);
	glm::mat4 projection = glm::mat4(1.f);

	view = glm::translate(view, view_pos);
	view = glm::rotate(view, glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f));
	//// view = glm::rotate(view, glm::radians(angle_y) * sensetivity_y, glm::vec3(1.f, 0.f, 0.f));
	//// view = glm::rotate(view, glm::radians(angle_x) * sensetivity_x, glm::vec3(0.f, 1.f, 0.f));
	projection = glm::perspective(glm::radians(45.f), float(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.f);

	glm::mat4 transform = projection * view * model;
	glUniformMatrix4fv(glGetUniformLocation(poolShader.id, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(poolShader.id, "transform"), 1, GL_FALSE, glm::value_ptr(transform));

	// light
	glUniform3fv(glGetUniformLocation(poolShader.id, "light.direction"), 1, glm::value_ptr(light_dir));
	glUniform3fv(glGetUniformLocation(poolShader.id, "light.ambient"), 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));
	glUniform3fv(glGetUniformLocation(poolShader.id, "light.diffuse"), 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
	glUniform3fv(glGetUniformLocation(poolShader.id, "light.specular"), 1, glm::value_ptr(glm::vec3(1.f, 1.f, 1.f)));
	glUniform1f(glGetUniformLocation(poolShader.id, "material.shininess"), 6.f);

	// camera
	glUniform3fv(glGetUniformLocation(poolShader.id, "viewPos"), 1, glm::value_ptr(view_pos));

	poolVAO.bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, poolDiffuseMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, poolNormalMap);

	glDrawArrays(GL_TRIANGLES, 0, 30);
	poolVAO.unbind();
}

unsigned int loadTexture(char const *path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void setupReflectionBuffer(uint &reflectionFramebuffer, uint &reflectionColorbuffer, uint &reflectionRenderbuffer)
{
	// framebuffer
	glGenFramebuffers(1, &reflectionFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, reflectionFramebuffer);
	// generate and attach a color texture buffer to framebuffer
	glGenTextures(1, &reflectionColorbuffer);
	glBindTexture(GL_TEXTURE_2D, reflectionColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflectionColorbuffer, 0);
	// generate and attach a renderbuffer to the framebuffer
	glGenRenderbuffers(1, &reflectionRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, reflectionRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, reflectionRenderbuffer);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void setupRefractionBuffer(uint &refractionFramebuffer, uint &refractionColorbuffer, uint &refractionRenderbuffer)
{
	// framebuffer
	glGenFramebuffers(1, &refractionFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, refractionFramebuffer);
	// generate and attach a color texture buffer to framebuffer
	glGenTextures(1, &refractionColorbuffer);
	glBindTexture(GL_TEXTURE_2D, refractionColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, refractionColorbuffer, 0);
	// generate and attach a renderbuffer to the framebuffer
	glGenRenderbuffers(1, &refractionRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, refractionRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, refractionRenderbuffer);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}