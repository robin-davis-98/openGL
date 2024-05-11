#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void mainLoop(GLFWwindow* window, unsigned int& shaderProgram);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mainLoop(GLFWwindow* window, unsigned int& shaderProgram);
void processInput(GLFWwindow* window);
void LoadShader(unsigned int& shader, const std::string& FILENAME, const std::string& TYPE);
void LoadShaderProgram(unsigned int& program, unsigned int shaders[]);

void testShader(std::string type, unsigned int& shader);
void testProgram(std::string type, unsigned int& program);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//VERTEX SHADER
	unsigned int vertexShader;
	LoadShader(vertexShader, "./shaders/vertex.glsl", "VERTEX");


	//FRAGMENT SHADER
	unsigned int fragmentShader;
	LoadShader(fragmentShader, "./shaders/fragment.glsl", "FRAGMENT");

	//SHADER PROGRAM
	unsigned int shaderProgram;

	unsigned int shaders[2] = { vertexShader, fragmentShader };

	LoadShaderProgram(shaderProgram, shaders);

	//SHADER CLEANUP
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//VERTEICE DEFINITION
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	// Vertex Buffer Object is a GL_ARRAY_BUFFER

	unsigned int VertexBufferObject, VertexArrayObject, ElementBufferObject;
	glGenBuffers(1, &VertexBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	mainLoop(window, shaderProgram);

	return 0;
}

void mainLoop(GLFWwindow* window, unsigned int& shaderProgram)
{
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void LoadShader(unsigned int& shader, const std::string& FILENAME, const std::string& TYPE)
{
	try 
	{
		std::string shaderCode = "";

		std::ifstream shaderFile(FILENAME.c_str());

		std::stringstream shaderStream;

		shaderStream << shaderFile.rdbuf();

		shaderFile.close();

		shaderCode = shaderStream.str();

		const char* shaderSource = shaderCode.c_str();

		if (TYPE == "VERTEX")
		{
			shader = glCreateShader(GL_VERTEX_SHADER);
		}

		if (TYPE == "FRAGMENT")
		{
			shader = glCreateShader(GL_FRAGMENT_SHADER);
		}

		glShaderSource(shader, 1, &shaderSource, NULL);
		glCompileShader(shader);

		testShader(TYPE, shader);
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER::" << TYPE << " - Failed to read file" << std::endl;
	}
}

void LoadShaderProgram(unsigned int &program, unsigned int shaders[])
{
	program = glCreateProgram();

	for (int i = 0; i < sizeof(shaders); i++)
	{
		glAttachShader(program, shaders[i]);
	}

	glLinkProgram(program);

	testProgram("PROGRAM", program);
}

void testShader(std::string type, unsigned int& shader)
{
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void testProgram(std::string type, unsigned int& program)
{
	int  success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << type << "::LINKING_FAILED\n" << infoLog << std::endl;
	}
}