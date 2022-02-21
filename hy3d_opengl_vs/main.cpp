#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "core.h"
#include "shader.h"
#include "math.h"
#include "camera.h"

//------------------------------------------------------------------------

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define WINFOW_TITLE "Window Title"

global_var f32 deltaTime = 0.0f;
global_var f32 lastFrame = 0.0f;

global_var camera player;

global_var vec2 mouseLastPos = { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f };
global_var bool firstMouse = true;

//------------------------------------------------------------------------

static_func void OnFramebufferSize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static_func void OnMouseInput(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		mouseLastPos = { (f32)xPos, (f32)yPos };
		firstMouse = false;
	}

	f32 yOffset = (mouseLastPos.Y - (f32)yPos) * player.sens * deltaTime; // reversed since y-coordinates range from bottom to top
	f32 xOffset = ((f32)xPos - mouseLastPos.X) * player.sens * deltaTime;
	mouseLastPos = { (f32)xPos, (f32)yPos };

	player.UpdateDir(xOffset, yOffset);
}

static_func void ProcessKeyboard(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	f32 camSpeed = deltaTime * player.speed;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		player.pos += camSpeed * player.dir;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		player.pos -= camSpeed * player.dir;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		player.pos -= camSpeed * Normalize(Cross(player.dir, VEC3_UP));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		player.pos += camSpeed * Normalize(Cross(player.dir, VEC3_UP));
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		player.pos.Y += camSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		player.pos.Y -= camSpeed;
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINFOW_TITLE, 0, 0);
	if (!window)
	{
		DebugPrint("Could not create window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, OnFramebufferSize);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, OnMouseInput);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	player.Initialize( { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, -1.0f }, VEC3_UP, 5.0f, 150.0f, 90.0f);

	shader_info shaders[] = {
		{ "default.vert", GL_VERTEX_SHADER },
		{ "default.frag", GL_FRAGMENT_SHADER }
	};
	ShaderProgram defaultShader = {};
	defaultShader.Create(shaders, ArrayCount(shaders));

	// Create Vertex Buffer
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	u32 vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		f32 curFrame = (f32)glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		ProcessKeyboard(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		f32 green = (sin(curFrame) / 2.0f) + 1.0f;

		defaultShader.Use();

		vec3 light[] = { 1.0f, 1.0f, 1.0f };
		vec3 object[] = { 0.2f, 0.7f, 0.0f};
		defaultShader.SetUniform(UNIFORM_TYPE::VEC3, "lightPos", &player.pos);
		defaultShader.SetUniform(UNIFORM_TYPE::VEC3, "lightColor", light);
		defaultShader.SetUniform(UNIFORM_TYPE::VEC3, "objectColor", object);

		mat4 model = Translate( { 0.0f, 0.0f, 0.0f });

		mat4 view = LookAt(player.pos, player.pos + player.dir, VEC3_UP);
		mat4 proj = Perspective(player.fov, (f32)WINDOW_WIDTH / (f32)WINDOW_HEIGHT, 0.1f, 100.0f);
		defaultShader.SetUniform(UNIFORM_TYPE::MAT4, "model", &model);
		defaultShader.SetUniform(UNIFORM_TYPE::MAT4, "view", &view);
		defaultShader.SetUniform(UNIFORM_TYPE::MAT4, "proj", &proj);


		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	glfwTerminate();
	return 0;
}