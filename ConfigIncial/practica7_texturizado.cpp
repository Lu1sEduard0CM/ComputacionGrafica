/*
	Práctica 7 - Texturizado de Dado
	Cruz Miranda Luis Eduardo
	Fecha de entrega: 28 de septiembre de 2025
	Número de cuenta: 319148204
*/

#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include "Camera.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void RotateDice();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Dice rotation variables
float diceRotationX = 0.0f;
float diceRotationY = 0.0f;
float diceRotationZ = 0.0f;

int main()
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 7 Dado Texturizado - Luis_Cruz", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set callbacks
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Print OpenGL info
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_TEST);

	// Build shaders
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	// Vertices del cubo con coordenadas UV del archivo OBJ
	// Extraídos y ordenados del archivo texturaDado.obj
	GLfloat vertices[] = {
		// Cara superior (face 1: 1/1/1 5/2/1 7/3/1 3/4/1)
		// Positions          // Colors           // Texture Coords
		 1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 1.0f,  0.249480f, 0.000062f, // v1
		-1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 1.0f,  0.497422f, 0.000055f, // v5
		-1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 1.0f,  0.499019f, 0.246119f, // v7
		 1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 1.0f,  0.250927f, 0.248007f, // v3

		 // Cara frontal (face 2: 4/5/2 3/6/2 7/7/2 8/8/2)
		  1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 1.0f,  0.250043f, 0.749957f, // v4
		  1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 1.0f,  0.500000f, 0.749957f, // v3
		 -1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 1.0f,  0.500000f, 0.999913f, // v7
		 -1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 1.0f,  0.250043f, 0.999913f, // v8

		 // Cara izquierda (face 3: 8/9/3 7/10/3 5/11/3 6/12/3)
		 -1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 1.0f,  0.000005f, 0.000000f, // v8
		 -1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 1.0f,  0.250955f, 0.000505f, // v7
		 -1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 1.0f,  0.251001f, 0.247985f, // v5
		 -1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 1.0f,  0.000001f, 0.248041f, // v6

		 // Cara inferior (face 4: 6/13/4 2/14/4 4/15/4 8/16/4)
		 -1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 1.0f,  0.250182f, 0.498990f, // v6
		  1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 1.0f,  0.500041f, 0.499666f, // v2
		  1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 1.0f,  0.499853f, 0.753694f, // v4
		 -1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 1.0f,  0.249942f, 0.753755f, // v8

		 // Cara derecha (face 5: 2/17/5 1/18/5 3/19/5 4/20/5)
		  1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 1.0f,  0.498369f, 0.000189f, // v2
		  1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 1.0f,  0.749018f, 0.000031f, // v1
		  1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 1.0f,  0.749024f, 0.246085f, // v3
		  1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 1.0f,  0.499042f, 0.246076f, // v4

		  // Cara posterior (face 6: 6/21/6 5/22/6 1/23/6 2/24/6)
		  -1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 1.0f,  0.250043f, 0.250043f, // v6
		  -1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 1.0f,  0.500000f, 0.250043f, // v5
		   1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 1.0f,  0.500000f, 0.500000f, // v1
		   1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 1.0f,  0.250043f, 0.500000f  // v2
	};

	// Índices para formar los triángulos de cada cara
	GLuint indices[] = {
		// Cara superior
		0, 1, 2,    0, 2, 3,
		// Cara frontal  
		4, 5, 6,    4, 6, 7,
		// Cara izquierda
		8, 9, 10,   8, 10, 11,
		// Cara inferior
		12, 13, 14, 12, 14, 15,
		// Cara derecha
		16, 17, 18, 16, 18, 19,
		// Cara posterior
		20, 21, 22, 20, 22, 23
	};

	// Setup buffers
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Load dice texture
	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	int textureWidth, textureHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load dice texture (asume que tienes dado.png en la carpeta images/)
	unsigned char* image = stbi_load("images/dado.png", &textureWidth, &textureHeight, &nrChannels, 0);
	if (image)
	{
		// Determinar el formato basado en los canales
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Dice texture loaded successfully!" << std::endl;
	}
	else
	{
		std::cout << "Failed to load dice texture" << std::endl;
	}
	stbi_image_free(image);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		DoMovement();
		RotateDice();

		// Clear buffers
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lampShader.Use();

		// Create transformations
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 model = glm::mat4(1.0f);

		/*
		// Opcional: rotar el dado para verlo mejor
		model = glm::rotate(model, (float)glfwGetTime() * 0.5f, glm::vec3(0.5f, 1.0f, 0.0f));
		*/


		// Aplicar rotaciones del dado
		model = glm::rotate(model, glm::radians(diceRotationX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(diceRotationY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(diceRotationZ), glm::vec3(0.0f, 0.0f, 1.0f));

		// Get uniform locations
		GLint modelLoc = glGetUniformLocation(lampShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lampShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// Draw cube
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // 6 caras * 2 triángulos * 3 vértices = 36
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	// Cleanup
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();

	return 0;
}

void DoMovement()
{
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void RotateDice()
{
	// Rotaciones del dado con teclas numéricas
	if (keys[GLFW_KEY_1]) // Rotación en X positiva
		diceRotationX += 90.0f * deltaTime;
	if (keys[GLFW_KEY_2]) // Rotación en X negativa
		diceRotationX -= 90.0f * deltaTime;
	if (keys[GLFW_KEY_3]) // Rotación en Y positiva
		diceRotationY += 90.0f * deltaTime;
	if (keys[GLFW_KEY_4]) // Rotación en Y negativa
		diceRotationY -= 90.0f * deltaTime;
	if (keys[GLFW_KEY_5]) // Rotación en Z positiva
		diceRotationZ += 90.0f * deltaTime;
	if (keys[GLFW_KEY_6]) // Rotación en Z negativa
		diceRotationZ -= 90.0f * deltaTime;

	// Reset rotación con R
	if (keys[GLFW_KEY_R])
	{
		diceRotationX = 0.0f;
		diceRotationY = 0.0f;
		diceRotationZ = 0.0f;
	}
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}