
/*
    Práctica 8
    Cruz Miranda Luis Eduardo
    Fecha de entrega: 12 de octubre de 2025
    Número de cuenta: 319148204
*/

// Std. Includes
#include <string>
#include <vector>
#include <cmath>

// Definir M_PI si no está definido
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void generateSphere(float radius, unsigned int rings, unsigned int sectors,
    std::vector<float>& vertices, std::vector<unsigned int>& indices);

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

// Variables para animación de ciclo día/noche
float sunCycle = 0.0f;           // Ciclo del sol (0 a PI)
float moonCycle = 0.0f;          // Ciclo de la luna (0 a PI)
float cycleSpeed = 0.05f;        // Velocidad de movimiento
bool isDayMode = true;           // true = día, false = noche
float sunMoonRadius = 12.0f;     // Radio de la trayectoria

// Vectores para la esfera
std::vector<float> sphereVertices;
std::vector<unsigned int> sphereIndices;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

void generateSphere(float radius, unsigned int rings, unsigned int sectors,
    std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
    vertices.clear();
    indices.clear();

    float const R = 1.0f / (float)(rings - 1);
    float const S = 1.0f / (float)(sectors - 1);

    for (unsigned int r = 0; r < rings; r++) {
        for (unsigned int s = 0; s < sectors; s++) {
            float const y = sin(-M_PI_2 + M_PI * r * R);
            float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
            float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

            // Posición
            vertices.push_back(x * radius);
            vertices.push_back(y * radius);
            vertices.push_back(z * radius);

            // Normal (mismo que la posición normalizada para una esfera)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    for (unsigned int r = 0; r < rings - 1; r++) {
        for (unsigned int s = 0; s < sectors - 1; s++) {
            indices.push_back(r * sectors + s);
            indices.push_back(r * sectors + (s + 1));
            indices.push_back((r + 1) * sectors + (s + 1));

            indices.push_back(r * sectors + s);
            indices.push_back((r + 1) * sectors + (s + 1));
            indices.push_back((r + 1) * sectors + s);
        }
    }
}

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 8 - Materiales e Illuminacion - Luis_Cruz", nullptr, nullptr);

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

    // GLFW Options
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    std::cout << "\n=== CONTROLES ===" << std::endl;
    std::cout << "T - Cambiar entre DIA y NOCHE" << std::endl;
    std::cout << "Modo DIA o NOCHE: I (avanzar sol o luna) / K (retroceder sol o luna)" << std::endl;
    std::cout << "WASD - Mover camara" << std::endl;
    std::cout << "===================\n" << std::endl;

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");

    // Load models
    Model dog((char*)"Models/Perro/RedDog.obj");
    Model park((char*)"Models/Parque/Parque.obj");
    Model bank((char*)"Models/Banca/Banca.obj");
    Model plant((char*)"Models/Planta/Planta.obj");
    Model bird((char*)"Models/Ave/Ave.obj");
    Model garbage((char*)"Models/BotesBasura/BotesBasura.obj");

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Generar esfera para sol y luna
    generateSphere(1.0f, 30, 30, sphereVertices, sphereIndices);

    // First, set the container's VAO (and VBO and EBO)
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float),
        &sphereVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int),
        &sphereIndices[0], GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
        (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer (el color se establece más abajo según el modo)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Calcular posiciones del sol y luna según el modo
        glm::vec3 sunPosition;
        glm::vec3 moonPosition;

        if (isDayMode) {
            // MODO DÍA - Solo el sol se mueve (de izquierda a derecha por arriba)
            sunPosition.x = sunMoonRadius * cos(sunCycle);
            sunPosition.y = sunMoonRadius * sin(sunCycle);
            sunPosition.z = 0.0f;

            // Luna permanece escondida abajo
            moonPosition.x = -sunMoonRadius;
            moonPosition.y = -2.0f;
            moonPosition.z = 0.0f;
        }
        else {
            // MODO NOCHE - Solo la luna se mueve (de izquierda a derecha por arriba)
            moonPosition.x = sunMoonRadius * cos(moonCycle);
            moonPosition.y = sunMoonRadius * sin(moonCycle);
            moonPosition.z = 0.0f;

            // Sol permanece escondido abajo
            sunPosition.x = sunMoonRadius;
            sunPosition.y = -2.0f;
            sunPosition.z = 0.0f;
        }

        // Manejo de luces - Ciclo día/noche
        lightingShader.Use();

        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(lightPosLoc, sunPosition.x, sunPosition.y, sunPosition.z);
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Configurar iluminación según el MODO activo
        if (isDayMode) {
            // ===== MODO DÍA - SOL activo =====
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.5f, 0.5f, 0.4f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 1.0f, 0.95f, 0.8f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 0.9f);

            // Color de fondo de día (azul cielo)
            glClearColor(0.53f, 0.81f, 0.98f, 1.0f);
        }
        else {
            // ===== SOL apagado en modo noche =====
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.0f, 0.0f, 0.0f);

            // Color de fondo de noche (azul oscuro/negro)
            glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
        }

        // Configurar segunda luz (LUNA)
        GLint lightPos2Loc = glGetUniformLocation(lightingShader.Program, "light2.position");
        glUniform3f(lightPos2Loc, moonPosition.x, moonPosition.y, moonPosition.z);

        if (!isDayMode) {
            // ===== MODO NOCHE - LUNA activa con luz azulada =====
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.08f, 0.10f, 0.22f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.35f, 0.45f, 0.75f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.6f, 0.75f, 1.0f);
        }
        else {
            // ===== LUNA apagada en modo día =====
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.0f, 0.0f, 0.0f);
        }

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Set material properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.3f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.2f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

        // ========== DIBUJADO DE MODELOS CON ILUMINACIÓN ==========

        // Perro
        glm::mat4 modelDog = glm::mat4(1.0f);
        modelDog = glm::translate(modelDog, glm::vec3(0.0f, 0.3f, 0.0f));
        modelDog = glm::rotate(modelDog, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDog));
        dog.Draw(lightingShader);

        // Parque (suelo/base)
        glm::mat4 modelPark = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPark));
        park.Draw(lightingShader);

        // Banca
        glm::mat4 modelBank = glm::mat4(1.0f);
        modelBank = glm::translate(modelBank, glm::vec3(0.5f, -0.15f, 4.0f));
        modelBank = glm::rotate(modelBank, glm::radians(205.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelBank = glm::scale(modelBank, glm::vec3(0.9f, 0.9f, 0.9f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelBank));
        bank.Draw(lightingShader);

        // Ave
        glm::mat4 modelBird = glm::mat4(1.0f);
        modelBird = glm::scale(modelBird, glm::vec3(1.2f, 1.2f, 1.2f));
        modelBird = glm::translate(modelBird, glm::vec3(0.5f, 0.89f, 3.4f));
        modelBird = glm::rotate(modelBird, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelBird));
        bird.Draw(lightingShader);

        // Planta
        glm::mat4 modelPlant = glm::mat4(1.0f);
        modelPlant = glm::translate(modelPlant, glm::vec3(4.0f, -0.1f, 2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPlant));
        plant.Draw(lightingShader);

        // Botes de Basura
        glm::mat4 modelGarbage = glm::mat4(1.0f);
        modelGarbage = glm::translate(modelGarbage, glm::vec3(-3.0f, -0.1f, 2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelGarbage));
        garbage.Draw(lightingShader);

        glBindVertexArray(0);

        // ========== DIBUJADO DE SOL Y LUNA ==========

        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Dibujar SOL (color amarillo-naranja cálido)
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, sunPosition);
        model = glm::scale(model, glm::vec3(0.5f)); // Sol más grande
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(glGetUniformLocation(lampshader.Program, "lightColor"), 1.0f, 0.8f, 0.3f); // Naranja-amarillo
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Dibujar LUNA (color hueso/arenoso)
        model = glm::mat4(1.0f);
        model = glm::translate(model, moonPosition);
        model = glm::scale(model, glm::vec3(0.4f)); // Luna un poco más pequeña
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(glGetUniformLocation(lampshader.Program, "lightColor"), 0.9f, 0.9f, 0.8f); // Color hueso
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
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

    // ========== CONTROL DE MODO DÍA/NOCHE ==========
    if (keys[GLFW_KEY_T] && action == GLFW_PRESS)
    {
        isDayMode = !isDayMode; // Cambiar entre día y noche
        std::cout << (isDayMode ? ">>> MODO DIA <<<" : ">>> MODO NOCHE <<<") << std::endl;
    }

    // ========== CONTROLES UNIFICADOS (I/K para ambos modos) ==========
    if (isDayMode)
    {
        // MODO DÍA - I/K controlan el SOL
        if (keys[GLFW_KEY_I])
        {
            sunCycle += cycleSpeed;
            // Limitar el ciclo del sol entre 0 y PI (semicírculo superior)
            if (sunCycle > 3.14159f)
                sunCycle = 3.14159f;
        }

        if (keys[GLFW_KEY_K])
        {
            sunCycle -= cycleSpeed;
            // No permitir que baje de 0
            if (sunCycle < 0.0f)
                sunCycle = 0.0f;
        }
    }
    else
    {
        // MODO NOCHE - I/K controlan la LUNA
        if (keys[GLFW_KEY_I])
        {
            moonCycle += cycleSpeed;
            // Limitar el ciclo de la luna entre 0 y PI
            if (moonCycle > 3.14159f)
                moonCycle = 3.14159f;
        }

        if (keys[GLFW_KEY_K])
        {
            moonCycle -= cycleSpeed;
            // No permitir que baje de 0
            if (moonCycle < 0.0f)
                moonCycle = 0.0f;
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


