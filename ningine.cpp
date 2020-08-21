#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

constexpr unsigned short WINDOW_WIDTH = 1366;
constexpr unsigned short WINDOW_HEIGHT = 768;

int main(int argc, char* argv[])
{
	if (glfwInit() == GLFW_FALSE)
	{
		return -1;
		std::cout << "Oops, GLFW doesn't initialized!\n";
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Hello World", 
		nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Oops, GLFW window doesn't created!\n";
		glfwTerminate();
		return -2;
	}
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Oops, GLAD doesn't loaded!\n";
		return -3;
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	return 0;
}