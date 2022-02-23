#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "core.h"
#include "shader.h"
#include "math.h"
#include "camera.h"
#include "model.h"
#include "particles.h"

//------------------------------------------------------------------------

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define WINFOW_TITLE "Window Title"

global_var f32 deltaTime = 0.0f;
global_var f32 lastFrame = 0.0f;

global_var camera player;

global_var bool inFreeRoam = true;
global_var vec2 mouseLastPos = { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f };
global_var bool firstMouse = true;

global_var bool keyWasDown[GLFW_KEY_LAST] = {};

global_var u32 dogCurAnim = DOG_ANIMATION_NONE;

//------------------------------------------------------------------------

static_func void OnFramebufferSize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static_func void OnMouseInput(GLFWwindow* window, double xPos, double yPos)
{
	if (!inFreeRoam)
		return;

	if (firstMouse)
	{
		mouseLastPos = { (f32)xPos, (f32)yPos };
		firstMouse = false;
	}

	f32 yOffset = (mouseLastPos.Y - (f32)yPos) * deltaTime; // reversed since y-coordinates range from bottom to top
	f32 xOffset = ((f32)xPos - mouseLastPos.X) * deltaTime;
	mouseLastPos = { (f32)xPos, (f32)yPos };

	player.UpdateDir(xOffset, yOffset);
}

static_func void ProcessKeyboard(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !keyWasDown[GLFW_KEY_ESCAPE])
	{
		keyWasDown[GLFW_KEY_ESCAPE] = true;
		inFreeRoam = !inFreeRoam;
		if (inFreeRoam)
		{
			firstMouse = true;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE && keyWasDown[GLFW_KEY_ESCAPE])
	{
		keyWasDown[GLFW_KEY_ESCAPE] = false;
	}

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

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !keyWasDown[GLFW_KEY_1])
	{
		keyWasDown[GLFW_KEY_1] = true;
		dogCurAnim |= DOG_ANIMATION_NECK;
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE && keyWasDown[GLFW_KEY_1])
	{
		keyWasDown[GLFW_KEY_1] = false;
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
	glfwSetCursorPosCallback(window, OnMouseInput);

	if (inFreeRoam)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//------------------------------------------------------------------------
	// PRINT MENU
	std::cout << "Press Esc to switch between free roam and cursor" << std::endl;
	std::cout << "Press 1 to animate neck" << std::endl;

	//------------------------------------------------------------------------

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	player.Initialize( { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, -1.0f }, VEC3_UP, 5.0f, 150.0f, 60.0f);

	shader_info shaders[] = {
		{ "default.vert", GL_VERTEX_SHADER },
		{ "default.frag", GL_FRAGMENT_SHADER }
	};
	ShaderProgram defaultShader = {};
	defaultShader.Create(shaders, ArrayCount(shaders));

	mesh dog = {};
	dog.MakeDog();

	particles particles = {};
	particles.Initialize();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		f32 curFrame = (f32)glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		ProcessKeyboard(window);

		defaultShader.Use();

		vec3 light[] = { 1.0f, 1.0f, 1.0f };

		vec3 object = { 0.5f + 0.5f * CosF(3.0f * curFrame), 0.6f, 0.5f + 0.5f * SinF(2.0f * curFrame) };
		defaultShader.SetUniform(UNIFORM_TYPE::VEC3, "lightPos", &player.pos);
		defaultShader.SetUniform(UNIFORM_TYPE::VEC3, "lightColor", light);
		defaultShader.SetUniform(UNIFORM_TYPE::VEC3, "objectColor", &object);
		defaultShader.SetUniform(UNIFORM_TYPE::VEC3, "viewPos", &player.pos);

		mat4 view = LookAt(player.pos, player.pos + player.dir, VEC3_UP);
		mat4 proj = Perspective(player.fov, (f32)WINDOW_WIDTH / (f32)WINDOW_HEIGHT, 0.1f, 100.0f);
		defaultShader.SetUniform(UNIFORM_TYPE::MAT4, "view", &view);
		defaultShader.SetUniform(UNIFORM_TYPE::MAT4, "proj", &proj);

		AnimateDog(dogCurAnim, deltaTime, curFrame);
		dog.Draw(defaultShader);

		object = { 1.0f, 1.0f, 1.0f };
		defaultShader.SetUniform(UNIFORM_TYPE::VEC3, "objectColor", &object);
		particles.UpdateAndRender(defaultShader, deltaTime, curFrame);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	dog.Delete();
	particles.Delete();

	glfwTerminate();
	return 0;
}