#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chrono>

using namespace std::chrono;

constexpr unsigned short WINDOW_WIDTH = 1366;
constexpr unsigned short WINDOW_HEIGHT = 768;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

inline float getDurationInMilisec(milliseconds start)
{
	milliseconds now = std::chrono::duration_cast<milliseconds>(
		system_clock::now().time_since_epoch());

	return (now - start).count();
}

int main(int argc, char* argv[])
{
	if (glfwInit() == GLFW_FALSE)
	{
		return -1;
		std::cout << "Oops, failed to initializeGLFW!\n";
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Hello World", 
		nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Oops, failed to create the GLFW window!\n";
		glfwTerminate();
		return -2;
	}
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Oops, failed to load the GLAD!\n";
		return -3;
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	milliseconds startTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// RENDERING
		float colorTime = getDurationInMilisec(startTime) / 2000.0f;

		glClearColor(sin(0.2f * colorTime), cos(0.5 * colorTime), sin(0.3f * colorTime), 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
