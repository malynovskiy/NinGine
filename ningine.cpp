#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chrono>

using namespace std::chrono;

constexpr unsigned short WINDOW_WIDTH = 1366;
constexpr unsigned short WINDOW_HEIGHT = 768;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

constexpr char* vertexShaderCode = R"glsl(
	#version 330 core

	layout(location = 0) in vec3 inPos;
	layout(location = 1) in vec3 inColor;
	
	out vec3 Color;

	void main()
	{
		Color = inColor;
		gl_Position = vec4(inPos.x, inPos.y, inPos.z, 1.0);
	}
)glsl";

constexpr char* fragmentShaderCode = R"glsl(
	#version 330 core
	
	uniform vec3 colorBlending;

	in vec3 Color;
	out vec4 FragmentColor;
	
	void main()
	{
		FragmentColor = vec4(colorBlending * Color, 1.0);
	}
)glsl";

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

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE;
#endif

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
	
	unsigned int vertexShader{};
	unsigned int fragmentShader{};

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
	glCompileShader(vertexShader);

	int success{};
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
	}

	glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << '\n';
	}
	
	unsigned int shaderProgram{};
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float smallTriangleVertices[] = {
		-0.4f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
		 0.4f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
		 0.0f,  0.7f, 0.0f,		0.0f, 0.0f, 1.0f
	};

	float bigTriangleVertices[] = {
		// positions			//colors
		-0.5f, -0.6f, 0.0f,		0.0f, 0.0f, 1.0f,
		 0.5f, -0.6f, 0.0f,		0.0f, 1.0f, 0.0f,
		 0.0f,  0.8f, 0.0f,		1.0f, 0.0f, 0.0f
	};

	// setting up buffers for TRIANGLE
	unsigned int smallTriangleVBO, smallTriangleVAO;
	glGenVertexArrays(1, &smallTriangleVAO);
	glGenBuffers(1, &smallTriangleVBO);

	// binding vertex buffer and array object for TRIANGLE
	glBindVertexArray(smallTriangleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, smallTriangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(smallTriangleVertices), smallTriangleVertices, GL_STATIC_DRAW);
	
	// setting up vertex attributes for current array object (for TRIANGLE)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 3)  * sizeof(float), static_cast<void*>(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3 + 3) * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// setting up buffers for RECTANGLE
	unsigned int bigTriangleVBO, bigTriangleVAO;
	glGenVertexArrays(1, &bigTriangleVAO);
	glGenBuffers(1, &bigTriangleVBO);

	// binding vertex buffer, array object and index buffer for RECTANGLE
	glBindVertexArray(bigTriangleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, bigTriangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bigTriangleVertices), bigTriangleVertices, GL_STATIC_DRAW);
	
	// setting up vertex attributes for current array object (for RECTANGLE)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 3) * sizeof(float), static_cast<void*>(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3 + 3) * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glUseProgram(shaderProgram);

	GLint blendingColor = glGetUniformLocation(shaderProgram, "colorBlending");

	auto t_start = high_resolution_clock::now();

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// RENDERING
		auto t_now = high_resolution_clock::now();
		float time = duration_cast<duration<float>>(t_start - t_now).count();

		glClearColor(sin(0.2f * time), cos(0.5 * time), sin(0.3f * time), 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUniform3f(blendingColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0, cos(time * 2.0f));
		glBindVertexArray(bigTriangleVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(smallTriangleVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &smallTriangleVBO);
	glDeleteVertexArrays(1, &smallTriangleVAO);
	
	glDeleteBuffers(1, &bigTriangleVBO);
	glDeleteVertexArrays(1, &bigTriangleVAO);

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
