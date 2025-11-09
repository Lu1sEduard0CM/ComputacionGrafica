/*
    Práctica 12
    Cruz Miranda Luis Eduardo
    Fecha de entrega: 9 de noviembre de 2025
    Número de cuenta: 319148204
*/

#include <iostream>
#include <fstream>
#include <sstream>
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

// Load Models
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
void saveAnimationToFile(const char* filename);
void loadAnimationFromFile(const char* filename);

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
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f, 2.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f)
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

// Animation variables
float rotBall = 0.0f;
float rotDog = 0.0f;
float dogPosX = 0.0f, dogPosY = 0.0f, dogPosZ = 0.0f;
float FLegs = 0.0f, RLegs = 0.0f, RLegsB = 80.0f, FLegsB = -80.0f;
float head = 0.0f, tail = 0.0f;
float fLeftLeg = 0.0f, fRightLeg = 0.0f;
float bLeftLeg = 0.0f, bRightLeg = 0.0f;
float bodyIncline = 0.0f;

// KeyFrames
#define MAX_FRAMES 9
int i_max_steps = 100;
int i_curr_steps = 0;

typedef struct _frame {
    float rotDog, rotDogInc;
    float dogPosX, dogPosY, dogPosZ;
    float incX, incY, incZ;
    float head, headInc;
    float tail, tailInc;
    float FLegs, FLegsInc;
    float RLegs, RLegsInc;
    float fLeftLeg, fLeftLegInc;
    float fRightLeg, fRightLegInc;
    float bLeftLeg, bLeftLegInc;
    float bRightLeg, bRightLegInc;
    float bodyIncline, bodyInclineInc;
} FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;
bool play = false;
int playIndex = 0;

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

void saveFrame()
{
    printf("frameindex %d\n", FrameIndex);

    KeyFrame[FrameIndex].dogPosX = dogPosX;
    KeyFrame[FrameIndex].dogPosY = dogPosY;
    KeyFrame[FrameIndex].dogPosZ = dogPosZ;
    KeyFrame[FrameIndex].rotDog = rotDog;
    KeyFrame[FrameIndex].head = head;
    KeyFrame[FrameIndex].tail = tail;
    KeyFrame[FrameIndex].FLegs = FLegs;
    KeyFrame[FrameIndex].RLegs = RLegs;
    KeyFrame[FrameIndex].fLeftLeg = fLeftLeg;
    KeyFrame[FrameIndex].fRightLeg = fRightLeg;
    KeyFrame[FrameIndex].bLeftLeg = bLeftLeg;
    KeyFrame[FrameIndex].bRightLeg = bRightLeg;
    KeyFrame[FrameIndex].bodyIncline = bodyIncline;

    FrameIndex++;
}

void resetElements()
{
    dogPosX = KeyFrame[0].dogPosX;
    dogPosY = KeyFrame[0].dogPosY;
    dogPosZ = KeyFrame[0].dogPosZ;
    head = KeyFrame[0].head;
    tail = KeyFrame[0].tail;
    FLegs = KeyFrame[0].FLegs;
    RLegs = KeyFrame[0].RLegs;
    fLeftLeg = KeyFrame[0].fLeftLeg;
    fRightLeg = KeyFrame[0].fRightLeg;
    bLeftLeg = KeyFrame[0].bLeftLeg;
    bRightLeg = KeyFrame[0].bRightLeg;
    bodyIncline = KeyFrame[0].bodyIncline;
    rotDog = KeyFrame[0].rotDog;
}

void interpolation()
{
    int pi = playIndex;

    KeyFrame[pi].incX = (KeyFrame[pi + 1].dogPosX - KeyFrame[pi].dogPosX) / i_max_steps;
    KeyFrame[pi].incY = (KeyFrame[pi + 1].dogPosY - KeyFrame[pi].dogPosY) / i_max_steps;
    KeyFrame[pi].incZ = (KeyFrame[pi + 1].dogPosZ - KeyFrame[pi].dogPosZ) / i_max_steps;
    KeyFrame[pi].headInc = (KeyFrame[pi + 1].head - KeyFrame[pi].head) / i_max_steps;
    KeyFrame[pi].tailInc = (KeyFrame[pi + 1].tail - KeyFrame[pi].tail) / i_max_steps;
    KeyFrame[pi].FLegsInc = (KeyFrame[pi + 1].FLegs - KeyFrame[pi].FLegs) / i_max_steps;
    KeyFrame[pi].RLegsInc = (KeyFrame[pi + 1].RLegs - KeyFrame[pi].RLegs) / i_max_steps;
    KeyFrame[pi].fLeftLegInc = (KeyFrame[pi + 1].fLeftLeg - KeyFrame[pi].fLeftLeg) / i_max_steps;
    KeyFrame[pi].fRightLegInc = (KeyFrame[pi + 1].fRightLeg - KeyFrame[pi].fRightLeg) / i_max_steps;
    KeyFrame[pi].bLeftLegInc = (KeyFrame[pi + 1].bLeftLeg - KeyFrame[pi].bLeftLeg) / i_max_steps;
    KeyFrame[pi].bRightLegInc = (KeyFrame[pi + 1].bRightLeg - KeyFrame[pi].bRightLeg) / i_max_steps;
    KeyFrame[pi].bodyInclineInc = (KeyFrame[pi + 1].bodyIncline - KeyFrame[pi].bodyIncline) / i_max_steps;
    KeyFrame[pi].rotDogInc = (KeyFrame[pi + 1].rotDog - KeyFrame[pi].rotDog) / i_max_steps;
}

void saveAnimationToFile(const char* filename)
{
    if (FrameIndex == 0)
    {
        std::cout << " ERROR: No hay keyframes para guardar. Presiona K para crear keyframes primero." << std::endl;
        return;
    }

    std::ofstream outFile(filename);
    if (!outFile)
    {
        std::cout << " ERROR: No se pudo crear el archivo para guardar." << std::endl;
        std::cout << "  Verifica permisos de escritura en la carpeta del programa." << std::endl;
        return;
    }

    // Guardar cada keyframe en una línea
    for (int i = 0; i < FrameIndex; i++)
    {
        outFile << KeyFrame[i].dogPosX << " " << KeyFrame[i].dogPosY << " " << KeyFrame[i].dogPosZ << " "
            << KeyFrame[i].rotDog << " " << KeyFrame[i].head << " " << KeyFrame[i].tail << " "
            << KeyFrame[i].FLegs << " " << KeyFrame[i].RLegs << " " << KeyFrame[i].fLeftLeg << " "
            << KeyFrame[i].fRightLeg << " " << KeyFrame[i].bLeftLeg << " " << KeyFrame[i].bRightLeg << " "
            << KeyFrame[i].bodyIncline << "\n";
    }

    outFile.close();

    std::cout << "\n========================================" << std::endl;
    std::cout << "  ANIMACION GUARDADA EXITOSAMENTE" << std::endl;
    std::cout << "  Archivo: " << filename << std::endl;
    std::cout << "  Keyframes: " << FrameIndex << std::endl;
    std::cout << "  Ubicacion: Carpeta del ejecutable" << std::endl;
    std::cout << "========================================\n" << std::endl;
}

void loadAnimationFromFile(const char* filename)
{
    std::ifstream inFile(filename);
    if (!inFile)
    {
        std::cout << "No se pudo abrir el archivo de animacion." << std::endl;
        return;
    }

    FrameIndex = 0;
    std::string line;
    while (std::getline(inFile, line) && FrameIndex < MAX_FRAMES)
    {
        std::istringstream iss(line);
        iss >> KeyFrame[FrameIndex].dogPosX >> KeyFrame[FrameIndex].dogPosY >> KeyFrame[FrameIndex].dogPosZ
            >> KeyFrame[FrameIndex].rotDog >> KeyFrame[FrameIndex].head >> KeyFrame[FrameIndex].tail
            >> KeyFrame[FrameIndex].FLegs >> KeyFrame[FrameIndex].RLegs >> KeyFrame[FrameIndex].fLeftLeg
            >> KeyFrame[FrameIndex].fRightLeg >> KeyFrame[FrameIndex].bLeftLeg >> KeyFrame[FrameIndex].bRightLeg
            >> KeyFrame[FrameIndex].bodyIncline;
        FrameIndex++;
    }
    inFile.close();
    std::cout << "Animacion cargada desde: " << filename << " (" << FrameIndex << " keyframes)" << std::endl;
}

void Animation()
{
    if (play)
    {
        if (i_curr_steps >= i_max_steps)
        {
            playIndex++;
            if (playIndex > FrameIndex - 2)
            {
                playIndex = 0;
                play = false;
            }
            else
            {
                i_curr_steps = 0;
                interpolation();
            }
        }
        else
        {
            dogPosX += KeyFrame[playIndex].incX;
            dogPosY += KeyFrame[playIndex].incY;
            dogPosZ += KeyFrame[playIndex].incZ;
            head += KeyFrame[playIndex].headInc;
            tail += KeyFrame[playIndex].tailInc;
            RLegs += KeyFrame[playIndex].RLegsInc;
            FLegs += KeyFrame[playIndex].FLegsInc;
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

void DoMovement()
{
    // Camera movement
    if (keys[GLFW_KEY_W])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (keys[GLFW_KEY_S])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (keys[GLFW_KEY_A])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (keys[GLFW_KEY_D])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    // Head
    if (keys[GLFW_KEY_4])
    {
        head += 0.25f;
    }
    if (keys[GLFW_KEY_5])
    {
        head -= 0.25f;
    }

    // Back legs
    if (keys[GLFW_KEY_U])
    {
        bLeftLeg += 0.25f;
        bRightLeg += 0.25f;
        if (bLeftLeg >= RLegsB && bRightLeg <= FLegsB)
        {
            bLeftLeg = RLegsB;
            bRightLeg = FLegsB;
        }
    }
    if (keys[GLFW_KEY_J])
    {
        bLeftLeg -= 0.25f;
        bRightLeg -= 0.25f;
        if (bLeftLeg <= FLegsB && bRightLeg >= RLegsB)
        {
            bLeftLeg = FLegsB;
            bRightLeg = RLegsB;
        }
    }

    // Front left leg
    if (keys[GLFW_KEY_C])
    {
        fLeftLeg += 0.25f;
        if (fLeftLeg > 20.0f) fLeftLeg = 20.0f;
    }
    if (keys[GLFW_KEY_V])
    {
        fLeftLeg -= 0.25f;
        if (fLeftLeg < 0.0f) fLeftLeg = 0.0f;
    }

    // Front right leg
    if (keys[GLFW_KEY_Z])
    {
        fRightLeg += 0.5f;
        if (fRightLeg > 90.0f) fRightLeg = 90.0f;
    }
    if (keys[GLFW_KEY_X])
    {
        fRightLeg -= 0.5f;
        if (fRightLeg < 0.0f) fRightLeg = 0.0f;
    }

    // Body incline
    if (keys[GLFW_KEY_0])
    {
        bodyIncline += 0.5f;
        if (bodyIncline > 25.0f) bodyIncline = 25.0f;
    }
    if (keys[GLFW_KEY_9])
    {
        bodyIncline -= 0.5f;
        if (bodyIncline < 0.0f) bodyIncline = 0.0f;
    }
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // Save keyframe
    if (key == GLFW_KEY_K && action == GLFW_PRESS)
    {
        if (FrameIndex < MAX_FRAMES)
        {
            saveFrame();
        }
    }

    // Play/Pause animation
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        if (!play && FrameIndex > 1)
        {
            resetElements();
            interpolation();
            play = true;
            playIndex = 0;
            i_curr_steps = 0;
        }
        else
        {
            play = !play;
        }
    }

    // Load animation from file
    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        loadAnimationFromFile("animacion_sentarse_saludar.txt");
    }

    // Save animation to file
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        std::cout << "\n>>> Guardando animacion..." << std::endl;
        std::cout << "    Keyframes en memoria: " << FrameIndex << std::endl;
        saveAnimationToFile("mi_animacion.txt");
    }

    // Toggle light
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        active = !active;
        Light1 = active ? glm::vec3(0.2f, 0.8f, 1.0f) : glm::vec3(0.0f);
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

int main()
{
    // Init GLFW
    glfwInit();

    // Create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 12. Animacion por KeyFrames - Luis_Cruz", nullptr, nullptr);

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

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Imprimimos información de OpenGL del sistema
    std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // Define viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Setup shaders
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    // Load models
    Model DogBody((char*)"Models/Perro/DogBody.obj");
    Model HeadDog((char*)"Models/Perro/HeadDog.obj");
    Model DogTail((char*)"Models/Perro/TailDog.obj");
    Model F_RightLeg((char*)"Models/Perro/F_RightLegDog.obj");
    Model F_LeftLeg((char*)"Models/Perro/F_LeftLegDog.obj");
    Model B_RightLeg((char*)"Models/Perro/B_RightLegDog.obj");
    Model B_LeftLeg((char*)"Models/Perro/B_LeftLegDog.obj");
    Model Piso((char*)"Models/Piso/piso.obj");
    Model Ball((char*)"Models/Ball/ball.obj");

    // Setup buffers
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Set texture units
    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Initialize KeyFrames
    for (int i = 0; i < MAX_FRAMES; ++i)
    {
        KeyFrame[i] = FRAME();
    }

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Calculate deltatime
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check events
        glfwPollEvents();
        DoMovement();
        Animation();

        // Clear buffers
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // Use shader
        lightingShader.Use();
        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Directional light
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);

        // Point light
        glm::vec3 lightColor;
        lightColor.x = std::abs(std::sin(glfwGetTime() * Light1.x));
        lightColor.y = std::abs(std::sin(glfwGetTime() * Light1.y));
        lightColor.z = std::sin(glfwGetTime() * Light1.z);

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

        // Material properties
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

        // Camera transformations
        glm::mat4 view = camera.GetViewMatrix();

        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model(1);

        // Draw floor
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Piso.Draw(lightingShader);

        // Draw dog body with incline
        glm::mat4 modelTemp = glm::translate(model, glm::vec3(dogPosX, dogPosY, dogPosZ));
        model = glm::rotate(modelTemp, glm::radians(-bodyIncline), glm::vec3(1.0f, 0.0f, 0.0f));
        modelTemp = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogBody.Draw(lightingShader);

        // Draw head
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
        model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        HeadDog.Draw(lightingShader);

        // Draw tail
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
        model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogTail.Draw(lightingShader);

        // Back Left Leg
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.082f, -0.046f, -0.218f));
        model = glm::rotate(model, glm::radians(bLeftLeg), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_LeftLeg.Draw(lightingShader);

        // Back Right Leg
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
        model = glm::rotate(model, glm::radians(bRightLeg), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_RightLeg.Draw(lightingShader);

        // Front Left Leg
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
        model = glm::rotate(model, glm::radians(fLeftLeg), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_LeftLeg.Draw(lightingShader);

        // Front Right Leg
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
        model = glm::rotate(model, glm::radians(fRightLeg), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_RightLeg.Draw(lightingShader);

        // Draw ball with transparency
        model = glm::mat4(1);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
        model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Ball.Draw(lightingShader);
        glDisable(GL_BLEND);
        glBindVertexArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}