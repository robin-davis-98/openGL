#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mainLoop(GLFWwindow* window, unsigned int& shaderProgram, unsigned int& vertexArrayObject, unsigned int& elementBufferObject);
void cleanUp(unsigned int& VertexArrayObject, unsigned int& VertexBufferObject, unsigned int& shaderProgram);
void processInput(GLFWwindow* window);
void LoadShader(unsigned int &shader, const std::string& FILENAME, const std::string& TYPE);
void LoadShaderProgram(unsigned int& program, unsigned int& vertexShader, unsigned int& fragmentShader);

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
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertexShaderFile = "./shaders/vertex.glsl";
	LoadShader(vertexShader, vertexShaderFile, "VERTEX");

	//FRAGMENT SHADER
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragmentShaderFile = "./shaders/fragment.glsl";
	LoadShader(fragmentShader, fragmentShaderFile, "FRAGMENT");

	//SHADER PROGRAM
	unsigned int shaderProgram = glCreateProgram();
	LoadShaderProgram(shaderProgram, vertexShader, fragmentShader);

	//SHADER CLEANUP
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//VERTEX DEFINITION
	float vertices[] = {
		0.5f,  0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
	   -0.5f, -0.5f, 0.0f,
	   -0.5f,  0.5f, 0.0f
	};

	//INDEX DEFINITION
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned int VertexBufferObject, VertexArrayObject, ElementBufferObject;
	glGenVertexArrays(1, &VertexArrayObject);
	glGenBuffers(1, &VertexBufferObject);
	glGenBuffers(1, &ElementBufferObject);

	glBindVertexArray(VertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	std::cout << "Initialization complete" << std::endl;

	mainLoop(window, shaderProgram, VertexArrayObject, ElementBufferObject);

	cleanUp(VertexBufferObject, VertexArrayObject, shaderProgram);

	return 0;
}

void mainLoop(GLFWwindow* window, unsigned int& shaderProgram, unsigned int& vertexArrayObject, unsigned int& elementBufferObject)
{
	//Uncomment this to draw in wireframe mode.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(vertexArrayObject);

		//glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void cleanUp(unsigned int& VertexArrayObject, unsigned int& VertexBufferObject, unsigned int& shaderProgram)
{

	glDeleteVertexArrays(1, &VertexArrayObject);
	glDeleteBuffers(1, &VertexBufferObject);
	//glDeleteBuffers(1, &ElementBufferObject);
	glDeleteProgram(shaderProgram);

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

		glShaderSource(shader, 1, &shaderSource, NULL);
		glCompileShader(shader);

		testShader(TYPE, shader);
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER::" << TYPE << " - Failed to read file" << std::endl;
	}
}

void LoadShaderProgram(unsigned int &program, unsigned int &vertexShader, unsigned int &fragmentShader)
{
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

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