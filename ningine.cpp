#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chrono>

using namespace std::chrono;

constexpr unsigned short WINDOW_WIDTH = 1366;
constexpr unsigned short WINDOW_HEIGHT = 768;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

constexpr char* vsCode = R"glsl(
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

constexpr char* fsCode = R"glsl(
	#version 330 core
	
	uniform vec3 colorBlending;

	in vec3 Color;
	out vec4 FragmentColor;
	
	void main()
	{
		FragmentColor = vec4(colorBlending * Color, 1.0);
	}
)glsl";

constexpr char* newfsCode = R"glsl(
	#version 330 core
	
	uniform vec3 colorBlending;

	in vec3 Color;
	out vec4 FragmentColor;
	
	void main()
	{
		FragmentColor = vec4(colorBlending + Color, 1.0);
	}
)glsl";

GLuint createShader(GLenum type, char* source);
GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader);

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
	
	unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vsCode);
	unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fsCode);
	unsigned int newFS = createShader(GL_FRAGMENT_SHADER, newfsCode);
	
	unsigned int shaderProgram = createShaderProgram(vertexShader, fragmentShader);
	unsigned int newShaderProgram = createShaderProgram(vertexShader, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(newFS);

	float smallTriangleVertices[] = {
		// positions			//colors
		-0.4f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
		 0.4f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
		 0.0f,  0.7f, 0.0f,		0.0f, 0.0f, 1.0f
	};

	float bigTriangleVertices[] = {
		// positions			//colors
		-0.6f, -0.7f, 0.0f,		0.0f, 0.0f, 1.0f,
		 0.6f, -0.7f, 0.0f,		0.0f, 1.0f, 0.0f,
		 0.0f,  0.9f, 0.0f,		1.0f, 0.0f, 0.0f
	};

	// setting up buffers for TRIANGLE
	unsigned int VBO[2], VAO[2];
	glGenVertexArrays(1, &VAO[0]);
	glGenBuffers(1, &VBO[0]);

	// binding vertex buffer and array object for TRIANGLE
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(smallTriangleVertices), smallTriangleVertices, GL_STATIC_DRAW);
	
	// setting up vertex attributes for current array object (for TRIANGLE)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 3)  * sizeof(float), static_cast<void*>(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3 + 3) * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// setting up buffers for RECTANGLE
	glGenVertexArrays(1, &VAO[1]);
	glGenBuffers(1, &VBO[1]);

	// binding vertex buffer, array object and index buffer for RECTANGLE
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bigTriangleVertices), bigTriangleVertices, GL_STATIC_DRAW);
	
	// setting up vertex attributes for current array object (for RECTANGLE)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 3) * sizeof(float), static_cast<void*>(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3 + 3) * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	GLint blendingColor = glGetUniformLocation(shaderProgram, "colorBlending");
	GLint newblendingColor = glGetUniformLocation(newShaderProgram, "colorBlending");

	auto t_start = high_resolution_clock::now();

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// RENDERING
		auto t_now = high_resolution_clock::now();
		float time = duration_cast<duration<float>>(t_start - t_now).count();

		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glUniform3f(blendingColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0, cos(time * 2.0f));
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glUseProgram(newShaderProgram);
		glUniform3f(blendingColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0, cos(time * 2.0f));
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteProgram(shaderProgram);
	glDeleteProgram(newShaderProgram);

	glDeleteBuffers(1, &VBO[0]);
	glDeleteVertexArrays(1, &VAO[0]);
	
	glDeleteBuffers(1, &VBO[1]);
	glDeleteVertexArrays(1, &VAO[1]);

	glfwTerminate();
	return 0;
}

GLuint createShader(GLenum type, char* source)
{
	unsigned int shader = glCreateShader(type);

	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	int success{};
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);

		char* shaderType = (type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";
		std::cout << "ERROR! : " << shaderType << " shader compilation failed\n" << infoLog << '\n';

		return 0;
	}

	return shader;
}

GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader)
{
	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success{};
	char infoLog[512];

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR! : Shader program linking failed\n" << infoLog << '\n';
		return 0;
	}

	return shaderProgram;
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
