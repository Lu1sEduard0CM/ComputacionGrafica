/*
	Previo Práctica 12
	Cruz Miranda Luis Eduardo
	Fecha de entrega: 4 de noviembre de 2025
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
void Animation();

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

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

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
//Anim
float rotBall = 0.0f;
float rotDog = 0.0f;
int dogAnim = 0;

// ============== VARIABLES MEJORADAS ==============
// Control individual de patas y nuevas características
float head = 0.0f;
float tail = 0.0f;
float fLeftLeg = 0.0f;      // Pata delantera izquierda
float fRightLeg = 0.0f;     // Pata delantera derecha
float bLeftLeg = 0.0f;      // Pata trasera izquierda
float bRightLeg = 0.0f;     // Pata trasera derecha
float bodyIncline = 0.0f;   // Inclinación del cuerpo
// ================================================



//KeyFrames
float dogPosX, dogPosY, dogPosZ; //Posición en x,y,z

#define MAX_FRAMES 9
int i_max_steps = 100;  // Cambiado de 190 a 100 para animación más rápida
int i_curr_steps = 0;

// ============== ESTRUCTURA FRAME MEJORADA ==============
typedef struct _frame {

	float rotDog;
	float rotDogInc;
	float dogPosX;
	float dogPosY;
	float dogPosZ;
	float incX;
	float incY;
	float incZ;

	// Propiedades expandidas
	float head;
	float headInc;
	float tail;
	float tailInc;

	// Control individual de patas
	float fLeftLeg;
	float fLeftLegInc;
	float fRightLeg;
	float fRightLegInc;
	float bLeftLeg;
	float bLeftLegInc;
	float bRightLeg;
	float bRightLegInc;

	// Inclinación del cuerpo
	float bodyIncline;
	float bodyInclineInc;

}FRAME;
// ======================================================

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].dogPosX = dogPosX;
	KeyFrame[FrameIndex].dogPosY = dogPosY;
	KeyFrame[FrameIndex].dogPosZ = dogPosZ;

	KeyFrame[FrameIndex].rotDog = rotDog;
	KeyFrame[FrameIndex].head = head;
	KeyFrame[FrameIndex].tail = tail;

	// Guardar patas individuales
	KeyFrame[FrameIndex].fLeftLeg = fLeftLeg;
	KeyFrame[FrameIndex].fRightLeg = fRightLeg;
	KeyFrame[FrameIndex].bLeftLeg = bLeftLeg;
	KeyFrame[FrameIndex].bRightLeg = bRightLeg;

	// Guardar inclinación
	KeyFrame[FrameIndex].bodyIncline = bodyIncline;

	FrameIndex++;
}

void resetElements(void)
{
	dogPosX = KeyFrame[0].dogPosX;
	dogPosY = KeyFrame[0].dogPosY;
	dogPosZ = KeyFrame[0].dogPosZ;
	head = KeyFrame[0].head;
	tail = KeyFrame[0].tail;

	// Resetear patas individuales
	fLeftLeg = KeyFrame[0].fLeftLeg;
	fRightLeg = KeyFrame[0].fRightLeg;
	bLeftLeg = KeyFrame[0].bLeftLeg;
	bRightLeg = KeyFrame[0].bRightLeg;

	// Resetear inclinación
	bodyIncline = KeyFrame[0].bodyIncline;

	rotDog = KeyFrame[0].rotDog;

}

// ============== INTERPOLACIÓN MEJORADA ==============
void interpolation(void)
{
	// Interpolación de posición
	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;

	// Interpolación de cabeza y cola
	KeyFrame[playIndex].headInc = (KeyFrame[playIndex + 1].head - KeyFrame[playIndex].head) / i_max_steps;
	KeyFrame[playIndex].tailInc = (KeyFrame[playIndex + 1].tail - KeyFrame[playIndex].tail) / i_max_steps;

	// Interpolación de patas individuales
	KeyFrame[playIndex].fLeftLegInc = (KeyFrame[playIndex + 1].fLeftLeg - KeyFrame[playIndex].fLeftLeg) / i_max_steps;
	KeyFrame[playIndex].fRightLegInc = (KeyFrame[playIndex + 1].fRightLeg - KeyFrame[playIndex].fRightLeg) / i_max_steps;
	KeyFrame[playIndex].bLeftLegInc = (KeyFrame[playIndex + 1].bLeftLeg - KeyFrame[playIndex].bLeftLeg) / i_max_steps;
	KeyFrame[playIndex].bRightLegInc = (KeyFrame[playIndex + 1].bRightLeg - KeyFrame[playIndex].bRightLeg) / i_max_steps;

	// Interpolación de inclinación del cuerpo
	KeyFrame[playIndex].bodyInclineInc = (KeyFrame[playIndex + 1].bodyIncline - KeyFrame[playIndex].bodyIncline) / i_max_steps;

	KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;

}
// ====================================================



// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Previo Practica 11. Animacion por KeyFrames - Luis_Cruz", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}


	// Imprimimos información de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");


	//models
	Model DogBody((char*)"Models/Perro/DogBody.obj");
	Model HeadDog((char*)"Models/Perro/HeadDog.obj");
	Model DogTail((char*)"Models/Perro/TailDog.obj");
	Model F_RightLeg((char*)"Models/Perro/F_RightLegDog.obj");
	Model F_LeftLeg((char*)"Models/Perro/F_LeftLegDog.obj");
	Model B_RightLeg((char*)"Models/Perro/B_RightLegDog.obj");
	Model B_LeftLeg((char*)"Models/Perro/B_LeftLegDog.obj");
	Model Piso((char*)"Models/Piso/piso.obj");
	Model Ball((char*)"Models/Ball/ball.obj");


	// ============== ANIMACIÓN PREDEFINIDA ==============
	// Animación: Perro se sienta y saluda con pata derecha
	printf("\n=== ANIMACIÓN PREDEFINIDA CARGADA ===\n");
	printf("Secuencia: Sentarse suavemente + Saludar con pata derecha\n");
	printf("Presiona 'L' para reproducir\n\n");

	// Keyframe 0: De pie (posición inicial)
	KeyFrame[0].dogPosX = 0.0f;     KeyFrame[0].dogPosY = 0.0f;     KeyFrame[0].dogPosZ = 0.0f;
	KeyFrame[0].rotDog = 0.0f;
	KeyFrame[0].head = 0.0f;        KeyFrame[0].tail = 0.0f;
	KeyFrame[0].fLeftLeg = 0.0f;    KeyFrame[0].fRightLeg = 0.0f;
	KeyFrame[0].bLeftLeg = 0.0f;    KeyFrame[0].bRightLeg = 0.0f;
	KeyFrame[0].bodyIncline = 0.0f;

	// Keyframe 1: Comienza a sentarse
	KeyFrame[1].dogPosX = 0.0f;     KeyFrame[1].dogPosY = -0.05f;   KeyFrame[1].dogPosZ = 0.0f;
	KeyFrame[1].rotDog = 0.0f;
	KeyFrame[1].head = 5.0f;        KeyFrame[1].tail = -10.0f;
	KeyFrame[1].fLeftLeg = 0.0f;    KeyFrame[1].fRightLeg = 0.0f;
	KeyFrame[1].bLeftLeg = 30.0f;   KeyFrame[1].bRightLeg = 30.0f;
	KeyFrame[1].bodyIncline = 15.0f;

	// Keyframe 2: Sentado completo (máximo 28°)
	KeyFrame[2].dogPosX = 0.0f;     KeyFrame[2].dogPosY = -0.08f;   KeyFrame[2].dogPosZ = 0.0f;
	KeyFrame[2].rotDog = 0.0f;
	KeyFrame[2].head = 8.0f;        KeyFrame[2].tail = -15.0f;
	KeyFrame[2].fLeftLeg = 0.0f;    KeyFrame[2].fRightLeg = 0.0f;
	KeyFrame[2].bLeftLeg = 45.0f;   KeyFrame[2].bRightLeg = 45.0f;
	KeyFrame[2].bodyIncline = 28.0f;

	// Keyframe 3: SALUDA - Pata derecha SUBE
	KeyFrame[3].dogPosX = 0.0f;     KeyFrame[3].dogPosY = -0.08f;   KeyFrame[3].dogPosZ = 0.0f;
	KeyFrame[3].rotDog = 0.0f;
	KeyFrame[3].head = 10.0f;       KeyFrame[3].tail = -12.0f;
	KeyFrame[3].fLeftLeg = 0.0f;    KeyFrame[3].fRightLeg = 80.0f;   // POSITIVO = SUBE
	KeyFrame[3].bLeftLeg = 45.0f;   KeyFrame[3].bRightLeg = 45.0f;
	KeyFrame[3].bodyIncline = 28.0f;

	// Keyframe 4: Baja la pata
	KeyFrame[4].dogPosX = 0.0f;     KeyFrame[4].dogPosY = -0.08f;   KeyFrame[4].dogPosZ = 0.0f;
	KeyFrame[4].rotDog = 0.0f;
	KeyFrame[4].head = 8.0f;        KeyFrame[4].tail = -15.0f;
	KeyFrame[4].fLeftLeg = 0.0f;    KeyFrame[4].fRightLeg = 0.0f;
	KeyFrame[4].bLeftLeg = 45.0f;   KeyFrame[4].bRightLeg = 45.0f;
	KeyFrame[4].bodyIncline = 28.0f;

	FrameIndex = 5;  // 5 keyframes predefinidos
	// ===================================================

	// Inicializar incrementos a 0
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotDogInc = 0;
		KeyFrame[i].headInc = 0;
		KeyFrame[i].tailInc = 0;
		KeyFrame[i].fLeftLegInc = 0;
		KeyFrame[i].fRightLegInc = 0;
		KeyFrame[i].bLeftLegInc = 0;
		KeyFrame[i].bRightLegInc = 0;
		KeyFrame[i].bodyInclineInc = 0;
	}


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);


	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);


		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp



		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);



		//Carga de modelo 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		// ============== CUERPO CON INCLINACIÓN ==============
		modelTemp = model = glm::translate(model, glm::vec3(dogPosX, dogPosY, dogPosZ));
		// PRIMERO: Inclinación hacia atrás (eje X negativo para inclinar hacia atrás)
		model = glm::rotate(model, glm::radians(-bodyIncline), glm::vec3(1.0f, 0.0f, 0.0f));
		// LUEGO: Rotación sobre Y (como siempre)
		modelTemp = model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogBody.Draw(lightingShader);
		// ====================================================

		//Head
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
		model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HeadDog.Draw(lightingShader);

		//Tail 
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
		model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogTail.Draw(lightingShader);

		// ============== PATAS CON CONTROL INDIVIDUAL ==============
		// Back Left Leg (Pata trasera izquierda)
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.082f, -0.046f, -0.218f));
		model = glm::rotate(model, glm::radians(bLeftLeg), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_LeftLeg.Draw(lightingShader);

		// Back Right Leg (Pata trasera derecha)
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
		model = glm::rotate(model, glm::radians(bRightLeg), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_RightLeg.Draw(lightingShader);

		// Front Left Leg (Pata delantera izquierda - recta/apoyada)
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
		model = glm::rotate(model, glm::radians(fLeftLeg), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_LeftLeg.Draw(lightingShader);

		// Front Right Leg (Pata delantera derecha - sube para saludar)
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
		model = glm::rotate(model, glm::radians(fRightLeg), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_RightLeg.Draw(lightingShader);
		// ==========================================================


		model = glm::mat4(1);
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ball.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// ============== CONTROLES MEJORADOS ==============
void DoMovement()
{
	// ===== CONTROLES DEL PERRO =====

	// Cabeza (izquierda/derecha)
	if (keys[GLFW_KEY_4])
	{
		head += 0.25f;
	}
	if (keys[GLFW_KEY_5])
	{
		head -= 0.25f;
	}

	// Cola
	if (keys[GLFW_KEY_6])
	{
		tail += 0.25f;
	}
	if (keys[GLFW_KEY_7])
	{
		tail -= 0.25f;
	}

	// Inclinación del cuerpo (sentarse)
	if (keys[GLFW_KEY_0])
	{
		bodyIncline += 0.5f;
		if (bodyIncline > 28.0f) bodyIncline = 28.0f;  // Máximo 28° para evitar deformaciones
	}
	if (keys[GLFW_KEY_9])
	{
		bodyIncline -= 0.5f;
		if (bodyIncline < 0.0f) bodyIncline = 0.0f;
	}

	// Patas traseras (sincronizadas para caminar)
	if (keys[GLFW_KEY_U])
	{
		bLeftLeg += 0.5f;
		bRightLeg += 0.5f;
		if (bLeftLeg >= 60.0f) bLeftLeg = 60.0f;
		if (bRightLeg <= -60.0f) bRightLeg = -60.0f;
	}
	if (keys[GLFW_KEY_J])
	{
		bLeftLeg -= 0.5f;
		bRightLeg -= 0.5f;
		if (bLeftLeg <= -60.0f) bLeftLeg = -60.0f;
		if (bRightLeg >= 60.0f) bRightLeg = 60.0f;
	}

	// Pata delantera izquierda (individual)
	if (keys[GLFW_KEY_C])
	{
		fLeftLeg += 0.25f;
		if (fLeftLeg > 20.0f) fLeftLeg = 20.0f;  // Recta/apoyada
	}
	if (keys[GLFW_KEY_V])
	{
		fLeftLeg -= 0.25f;
		if (fLeftLeg < 0.0f) fLeftLeg = 0.0f;
	}

	// Pata delantera derecha (individual - para saludar)
	if (keys[GLFW_KEY_Z])
	{
		fRightLeg += 0.5f;
		if (fRightLeg > 90.0f) fRightLeg = 90.0f;  // Sube para saludo
	}
	if (keys[GLFW_KEY_X])
	{
		fRightLeg -= 0.5f;
		if (fRightLeg < 0.0f) fRightLeg = 0.0f;
	}

	// Rotación del perro
	if (keys[GLFW_KEY_2])
	{
		rotDog += 0.8f;
	}

	if (keys[GLFW_KEY_3])
	{
		rotDog -= 0.8f;
	}

	// Posición del perro
	if (keys[GLFW_KEY_H])
	{
		dogPosZ += 0.01;
	}

	if (keys[GLFW_KEY_Y])
	{
		dogPosZ -= 0.01;
	}

	if (keys[GLFW_KEY_G])
	{
		dogPosX -= 0.01;
	}

	if (keys[GLFW_KEY_N])
	{
		dogPosX += 0.01;
	}

	// ===== CONTROLES DE CÁMARA =====
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
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// Guardar keyframe actual
	if (keys[GLFW_KEY_K])
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
			printf(" Keyframe %d guardado\n", FrameIndex - 1);
		}
		else
		{
			printf(" Máximo de keyframes alcanzado (%d)\n", MAX_FRAMES);
		}
	}

	// Reproducir/Pausar animación
	if (keys[GLFW_KEY_L])
	{
		if (play == false && (FrameIndex > 1))
		{
			printf("\n Reproduciendo animación...\n");
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			printf(" Animación pausada\n");
		}
	}

	// Toggle de luz puntual
	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
			printf(" Luz activada\n");
		}
		else
		{
			Light1 = glm::vec3(0);
			printf(" Luz desactivada\n");
		}
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
}

// ============== ANIMACIÓN MEJORADA ==============
void Animation() {

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf(" Animación terminada\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation - Actualizar TODAS las propiedades
			dogPosX += KeyFrame[playIndex].incX;
			dogPosY += KeyFrame[playIndex].incY;
			dogPosZ += KeyFrame[playIndex].incZ;

			head += KeyFrame[playIndex].headInc;
			tail += KeyFrame[playIndex].tailInc;

			fLeftLeg += KeyFrame[playIndex].fLeftLegInc;
			fRightLeg += KeyFrame[playIndex].fRightLegInc;
			bLeftLeg += KeyFrame[playIndex].bLeftLegInc;
			bRightLeg += KeyFrame[playIndex].bRightLegInc;

			bodyIncline += KeyFrame[playIndex].bodyInclineInc;

			rotDog += KeyFrame[playIndex].rotDogInc;

			i_curr_steps++;
		}
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
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}