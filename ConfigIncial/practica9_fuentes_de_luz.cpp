/*
	Práctica 9
	Cruz Miranda Luis Eduardo
	Fecha de entrega: 19 de octubre de 2025
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

//Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Variable para modo noche
bool isNightMode = false;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

// ========== VARIABLES PARA LUCES DE MODELOS DECORATIVOS ==========

// Posición del perro (objetivo de las luces LED)
glm::vec3 perroPos = glm::vec3(0.0f, 0.3f, 0.0f);

// CUBO 1: LED DERECHO - TIPO REFLECTOR (SpotLight 0)
glm::vec3 ledDerechoPos = glm::vec3(-1.53f, 0.1f, 0.0f);
glm::vec3 ledDerechoDir = glm::normalize(perroPos - ledDerechoPos);
float ledDerechoCutOff = 15.0f;
float ledDerechoOuterCutOff = 25.0f;
glm::vec3 ledDerechoColor = glm::vec3(1.0f, 1.0f, 0.9f);

// CUBO 2: LED IZQUIERDO - TIPO REFLECTOR (SpotLight 1)
glm::vec3 ledIzquierdoPos = glm::vec3(1.52f, -0.047f, 0.0f);
glm::vec3 ledIzquierdoDir = glm::normalize(perroPos - ledIzquierdoPos);
float ledIzquierdoCutOff = 15.0f;
float ledIzquierdoOuterCutOff = 25.0f;
glm::vec3 ledIzquierdoColor = glm::vec3(1.0f, 1.0f, 0.9f);

// CUBO 3: LAMPARA DE CALLE - LUZ 1 - TIPO PUNTUAL (Point Light 1)
glm::vec3 lamparaLuz1Pos = glm::vec3(3.24f, 4.1f, 3.6f);
glm::vec3 lamparaLuz1Color = glm::vec3(1.0f, 1.0f, 1.0f); // Luz BLANCA

// CUBO 4: LAMPARA DE CALLE - LUZ 2 - TIPO PUNTUAL (Point Light 2)
glm::vec3 lamparaLuz2Pos = glm::vec3(2.15f, 4.1f, 3.65f);
glm::vec3 lamparaLuz2Color = glm::vec3(1.0f, 1.0f, 1.0f); // Luz BLANCA

// CUBO 5: FAROLA - TIPO PUNTUAL (Point Light 3)
glm::vec3 farolaPos = glm::vec3(-0.77f, 1.15f, 4.4f);
glm::vec3 farolaColor = glm::vec3(0.6f, 0.8f, 1.0f); // Luz AZULADA
float farolaConstant = 1.0f;
float farolaLinear = 0.09f;
float farolaQuadratic = 0.032f;

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

glm::vec3 Light1 = glm::vec3(0);

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 9 - Fuentes de Luz - Luis_Cruz", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Cargar shaders
	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	// ========== CARGA DE MODELOS ==========
	// Modelos de la práctica anterior
	Model Dog((char*)"Models/Perro/RedDog.obj");
	Model Park((char*)"Models/Parque/Parque.obj");
	Model Bank((char*)"Models/Banca/Banca.obj");
	Model Plant((char*)"Models/Planta/Planta.obj");
	Model Bird((char*)"Models/Ave/Ave.obj");
	Model Garbage((char*)"Models/BotesBasura/BotesBasura.obj");

	// Modelos nuevos para fuentes de luz
	Model Led((char*)"Models/Led/Led.obj");           // Reflectores LED
	Model Lamp((char*)"Models/Lampara/lampara.obj");  // Lámpara de calle
	Model WallLight((char*)"Models/Farola/farola.obj"); // Farola de pared

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		DoMovement();

		// Color de fondo nocturno
		glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		lightingShader.Use();
		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		// ========================================================================
		// CONFIGURACIÓN DE LUCES
		// ========================================================================

		// ========== LUZ DIRECCIONAL (DESACTIVADA PARA AMBIENTE NOCTURNO) ==========
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.0f, 0.0f, 0.0f);//0.3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.0f, 0.0f, 0.0f);//0.7 Para dar un iluminación completa que simula la luna
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.0f, 0.0f, 0.0f); //0.5

		// ========== POINT LIGHT 0: LUZ ANIMADA  ==========
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"),
			pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.0f, 0.0f, 0.0f); 
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.0f, 0.0f, 0.0f); 
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.0f, 0.0f, 0.0f); 
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

		// ========== POINT LIGHT 1: LÁMPARA DE CALLE - LUZ 1 (CUBO 3) ==========
		// Tipo: Luz Puntual | Color: Blanco | Posición: (3.24, 4.1, 3.6)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"),
			lamparaLuz1Pos.x, lamparaLuz1Pos.y, lamparaLuz1Pos.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"),
			lamparaLuz1Color.x * 0.3f, lamparaLuz1Color.y * 0.3f, lamparaLuz1Color.z * 0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"),
			lamparaLuz1Color.x * 1.5f, lamparaLuz1Color.y * 1.5f, lamparaLuz1Color.z * 1.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"),
			lamparaLuz1Color.x * 1.0f, lamparaLuz1Color.y * 1.0f, lamparaLuz1Color.z * 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.075f);

		// ========== POINT LIGHT 2: LÁMPARA DE CALLE - LUZ 2 (CUBO 4) ==========
		// Tipo: Luz Puntual | Color: Blanco | Posición: (2.15, 4.1, 3.65)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"),
			lamparaLuz2Pos.x, lamparaLuz2Pos.y, lamparaLuz2Pos.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"),
			lamparaLuz2Color.x * 0.3f, lamparaLuz2Color.y * 0.3f, lamparaLuz2Color.z * 0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"),
			lamparaLuz2Color.x * 1.5f, lamparaLuz2Color.y * 1.5f, lamparaLuz2Color.z * 1.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"),
			lamparaLuz2Color.x * 1.0f, lamparaLuz2Color.y * 1.0f, lamparaLuz2Color.z * 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.075f);


		// ========== POINT LIGHT 3: FAROLA (CUBO 5) ==========
		// Tipo: Luz Puntual | Color: Azulado | Posición: (-0.77, 1.15, 4.4)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"),
			farolaPos.x, farolaPos.y, farolaPos.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"),
			farolaColor.x * 0.3f, farolaColor.y * 0.3f, farolaColor.z * 0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"),
			farolaColor.x * 1.5f, farolaColor.y * 1.5f, farolaColor.z * 1.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"),
			farolaColor.x * 1.0f, farolaColor.y * 1.0f, farolaColor.z * 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), farolaConstant);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), farolaLinear);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), farolaQuadratic);

		// ========== SPOTLIGHT 0: LED DERECHO (CUBO 1) ==========
		// Tipo: Reflector | Color: Blanco cálido | Dirección: Hacia el perro
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].position"),
			ledDerechoPos.x, ledDerechoPos.y, ledDerechoPos.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].direction"),
			ledDerechoDir.x, ledDerechoDir.y, ledDerechoDir.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].ambient"),
			ledDerechoColor.x * 0.2f, ledDerechoColor.y * 0.2f, ledDerechoColor.z * 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].diffuse"),
			ledDerechoColor.x * 1.0f, ledDerechoColor.y * 1.0f, ledDerechoColor.z * 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].specular"),
			ledDerechoColor.x * 0.8f, ledDerechoColor.y * 0.8f, ledDerechoColor.z * 0.8f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].cutOff"),
			glm::cos(glm::radians(ledDerechoCutOff)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].outerCutOff"),
			glm::cos(glm::radians(ledDerechoOuterCutOff)));

		// ========== SPOTLIGHT 1: LED IZQUIERDO (CUBO 2) ==========
		// Tipo: Reflector | Color: Blanco cálido | Dirección: Hacia el perro
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].position"),
			ledIzquierdoPos.x, ledIzquierdoPos.y, ledIzquierdoPos.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].direction"),
			ledIzquierdoDir.x, ledIzquierdoDir.y, ledIzquierdoDir.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].ambient"),
			ledIzquierdoColor.x * 0.2f, ledIzquierdoColor.y * 0.2f, ledIzquierdoColor.z * 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].diffuse"),
			ledIzquierdoColor.x * 1.0f, ledIzquierdoColor.y * 1.0f, ledIzquierdoColor.z * 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].specular"),
			ledIzquierdoColor.x * 0.8f, ledIzquierdoColor.y * 0.8f, ledIzquierdoColor.z * 0.8f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].cutOff"),
			glm::cos(glm::radians(ledIzquierdoCutOff)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].outerCutOff"),
			glm::cos(glm::radians(ledIzquierdoOuterCutOff)));

		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

		// Camera transformations
		glm::mat4 view = camera.GetViewMatrix();

		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model(1);

		// ========================================================================
		// RENDERIZADO DE MODELOS
		// ========================================================================

		// Parque (base del entorno)
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Park.Draw(lightingShader);

		// Perro (protagonista - iluminado por LEDs)
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.3f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Dog.Draw(lightingShader);

		// Banca
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.5f, -0.15f, 4.0f));
		model = glm::rotate(model, glm::radians(205.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Bank.Draw(lightingShader);

		// Ave
		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::translate(model, glm::vec3(0.5f, 0.89f, 3.4f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Bird.Draw(lightingShader);

		// Planta
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.0f, -0.1f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Plant.Draw(lightingShader);

		// Botes de Basura
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.0f, -0.1f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Garbage.Draw(lightingShader);

		// ========== MODELOS DE FUENTES DE LUZ ==========

		// LED Derecho
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-1.5f, 0.03f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Led.Draw(lightingShader);

		// LED Izquierdo
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.5f, -0.09f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Led.Draw(lightingShader);

		// Lámpara de calle
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(3.1f, 0.5f, 3.3f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Lamp.Draw(lightingShader);

		// Farola (luz de pared)
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.0f, 1.5f, 6.42f));
		model = glm::rotate(model, glm::radians(168.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		WallLight.Draw(lightingShader);

		glBindVertexArray(0);

		// ========================================================================
		// CUBOS VISUALES DE LUZ (lampShader)
		// ========================================================================
		lampShader.Use();
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// CUBO 1: LED DERECHO - SPOTLIGHT 0
		// Posición: (-1.53, 0.1, 0.0) | Tipo: Reflector hacia el perro
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-1.53f, 0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// CUBO 2: LED IZQUIERDO - SPOTLIGHT 1
		// Posición: (1.52, -0.047, 0.0) | Tipo: Reflector hacia el perro
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.52f, -0.047f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// CUBO 3: LAMPARA LUZ 1 - POINT LIGHT 1
		// Posición: (3.24, 4.1, 3.6) | Tipo: Luz Puntual Blanca
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(3.24f, 4.1f, 3.6f));
		model = glm::scale(model, glm::vec3(0.15f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// CUBO 4: LAMPARA LUZ 2 - POINT LIGHT 2
		// Posición: (2.15, 4.1, 3.65) | Tipo: Luz Puntual Blanca
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.15f, 4.1f, 3.65f));
		model = glm::scale(model, glm::vec3(0.15f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// CUBO 5: FAROLA - POINT LIGHT 3
		// Posición: (-0.77, 1.15, 4.4) | Tipo: Luz Puntual Azulada
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-0.77f, 1.15f, 4.4f));
		model = glm::scale(model, glm::vec3(0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
		}
		else
		{
			Light1 = glm::vec3(0);
		}
	}

	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		isNightMode = !isNightMode;
		std::cout << (isNightMode ? ">>> MODO NOCHE ACTIVADO <<<" : ">>> MODO NORMAL <<<") << std::endl;
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