/*
	Práctica 4
	Cruz Miranda Luis Eduardo
	Fecha de entrega: 7 de septiembre de 2025
	Número de cuenta: 319148204
*/

#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);

//--------------------------------------------------------
// PASO 1: FUNCIÓN PARA GENERAR VÉRTICES CON COLOR ESPECÍFICO
void generateColoredCubeVertices(float vertices[], float r, float g, float b) {
	// Array temporal con las posiciones de los vértices del cubo
	float cubePositions[] = {
		// Front face
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		// Back face
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		// Right face
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,

		 // Left face
		 -0.5f,  0.5f,  0.5f,
		 -0.5f,  0.5f, -0.5f,
		 -0.5f, -0.5f, -0.5f,
		 -0.5f, -0.5f, -0.5f,
		 -0.5f, -0.5f,  0.5f,
		 -0.5f,  0.5f,  0.5f,

		 // Bottom face
		 -0.5f, -0.5f, -0.5f,
		  0.5f, -0.5f, -0.5f,
		  0.5f, -0.5f,  0.5f,
		  0.5f, -0.5f,  0.5f,
		 -0.5f, -0.5f,  0.5f,
		 -0.5f, -0.5f, -0.5f,

		 // Top face
		 -0.5f,  0.5f, -0.5f,
		  0.5f,  0.5f, -0.5f,
		  0.5f,  0.5f,  0.5f,
		  0.5f,  0.5f,  0.5f,
		 -0.5f,  0.5f,  0.5f,
		 -0.5f,  0.5f, -0.5f
	};

	// Llenar el array de vertices con posiciones y colores
	for (int i = 0; i < 36; i++) {
		// Posición (x, y, z)
		vertices[i * 6 + 0] = cubePositions[i * 3 + 0];
		vertices[i * 6 + 1] = cubePositions[i * 3 + 1];
		vertices[i * 6 + 2] = cubePositions[i * 3 + 2];

		// Color (r, g, b)
		vertices[i * 6 + 3] = r;
		vertices[i * 6 + 4] = g;
		vertices[i * 6 + 5] = b;
	}
}


// PASO 2: FUNCIÓN PARA CREAR UN VAO CON COLOR ESPECÍFICO
GLuint createColoredCubeVAO(float r, float g, float b) {
	// Crear array de vértices con el color especificado
	float vertices[216]; // 36 vértices × 6 valores = 216
	generateColoredCubeVertices(vertices, r, g, b);

	// Crear VAO y VBO
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Configurar VAO
	glBindVertexArray(VAO);

	// Cargar datos al VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Configurar atributos de vértice
	// Posición (location = 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Color (location = 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Desenlazar buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO; // Devolver el ID del VAO creado
}
//-------------------------------------------------------------------------------

const GLint WIDTH = 800, HEIGHT = 600;
float movX = 0.0f;
float movY = 0.0f;      //Variables que permiten manipular la vista directamente 
float movZ = -5.0f;
float rot = 0.0f;

int main() {
	glfwInit();

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Modelado Geometrico Luis_Cruz", nullptr, nullptr);

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificación de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificación de errores de inicialización de glew
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos información de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);
	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// PASO 3: DECLARAR Y CREAR MÚLTIPLES VAOs PARA LOS COLORES DEL OSO
	GLuint vaoMarronOscuro;  // Para nariz y detalles
	GLuint vaoMarronClaro;   // Para cabeza, hocico, patas y cola
	GLuint vaoBlanco;        // Para detalles blancos
	GLuint vaoNegro;         // Para ojos

	// PASO 4: INICIALIZAR TODOS LOS VAOs CON SUS RESPECTIVOS COLORES
	vaoMarronOscuro = createColoredCubeVAO(0.349f, 0.173f, 0.047f); // Café oscuro
	vaoMarronClaro = createColoredCubeVAO(0.529f, 0.314f, 0.161f);  // Café claro
	vaoBlanco = createColoredCubeVAO(1.0f, 1.0f, 1.0f);       // Blanco puro
	vaoNegro = createColoredCubeVAO(0.0f, 0.0f, 0.0f);        // Negro puro

	glm::mat4 projection = glm::mat4(1);
	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(window))
	{
		Inputs(window);
		glfwPollEvents();

		// Render
		glClearColor(0.2f, 0.7f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw our first triangle
		ourShader.Use();
		glm::mat4 model = glm::mat4(1);
		glm::mat4 view = glm::mat4(1);

		view = glm::translate(view, glm::vec3(movX, movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");

		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// PASO 5: RENDERIZADO DEL OSO CON MÚLTIPLES VAOs

		// ==================== CUERPO (Marrón Claro) ====================
		glBindVertexArray(vaoMarronClaro);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.5f, 0.6f, 1.2f)); // Alto -> , ancho | y profundidad /
		model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ==================== CABEZA (Marrón Claro) ====================
		glBindVertexArray(vaoMarronClaro);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5f, 0.6f, 0.8f)); 
		model = glm::translate(model, glm::vec3(3.0f, 0.025f, 0.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ==================== COLA (Marrón Claro) ========================

		glBindVertexArray(vaoMarronClaro);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.3f)); 
		model = glm::translate(model, glm::vec3(-4.75f, 0.0f, 0.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ==================== HOCICO/NARIZ (Marrón Claro) ====================
		glBindVertexArray(vaoMarronClaro);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.25f, 0.3f, 0.4f)); 
		model = glm::translate(model, glm::vec3(7.5f, -0.47f, 0.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ==================== OREJAS (Marrón Oscuro) ====================
		// Oreja izquierda
		glBindVertexArray(vaoMarronClaro);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.15f, 0.3f, 0.3f)); 
		model = glm::translate(model, glm::vec3(11.0f, 1.0f, 1.25f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Oreja derecha
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.15f, 0.3f, 0.3f));
		model = glm::translate(model, glm::vec3(11.0f, 1.0f, -1.25f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ============ Detalles Orejas ===============

		glBindVertexArray(vaoMarronOscuro);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::translate(model, glm::vec3(17.2f, 2.75f, 3.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(vaoMarronOscuro);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::translate(model, glm::vec3(17.2f, 2.75f, -3.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ==================== OJOS (Negro) ====================
		// Ojo derecho
		glBindVertexArray(vaoNegro);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.075f, 0.075f, 0.075f)); 
		model = glm::translate(model, glm::vec3(23.0f, 0.5f, 2.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Ojo izquierdo
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.075f, 0.075f, 0.075f));
		model = glm::translate(model, glm::vec3(23.0f, 0.5f, -2.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ==================== NARIZ (Marrón Oscuro) ====================
		glBindVertexArray(vaoMarronOscuro);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.15f)); 
		model = glm::translate(model, glm::vec3(20.0f, -0.25f, 0.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ==================== PATAS (Marrón Oscuro) ====================
		glBindVertexArray(vaoMarronClaro);

		// Pata trasera derecha (vista lateral al ejecutar)
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.6f, 0.3f));
		model = glm::translate(model, glm::vec3(-3.65f, -1.2f, 1.5f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pata delantera derecha (vista lateral al ejecutar)
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.6f, 0.3f));
		model = glm::translate(model, glm::vec3(3.65f, -1.2f, 1.5f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pata trasera izquierda (vista lateral al ejecutar)
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.6f, 0.3f));
		model = glm::translate(model, glm::vec3(-3.65f, -1.2f, -1.5f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pata trasera derecha
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.6f, 0.3f));
		model = glm::translate(model, glm::vec3(3.65f, -1.2f, -1.5f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ==================== Complemento patas ========================
		glBindVertexArray(vaoBlanco);

		// Pata trasera derecha (vista lateral al ejecutar)
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.15f, 0.3f)); // Patas cilíndricas
		model = glm::translate(model, glm::vec3(-4.2f, -6.3f, 1.5f)); // Frente izquierda
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pata delantera derecha (vista lateral al ejecutar)
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.15f, 0.3f));
		model = glm::translate(model, glm::vec3(6.8f, -6.3f, 1.5f)); // Frente derecha
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pata trasera izquierda (vista lateral al ejecutar)
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.15f, 0.3f));
		model = glm::translate(model, glm::vec3(-4.2f, -6.3f, -1.5f)); // Atrás izquierda
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pata trasera derecha (vista lateral al ejecutar)
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.15f, 0.3f));
		model = glm::translate(model, glm::vec3(6.8f, -6.3f, -1.5f)); // Atrás derecha
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Desvincular VAO
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// PASO 6: CLEANUP - LIMPIAR TODOS LOS VAOs Y VBOs CREADOS
	glDeleteVertexArrays(1, &vaoMarronOscuro);
	glDeleteVertexArrays(1, &vaoMarronClaro);
	glDeleteVertexArrays(1, &vaoBlanco);
	glDeleteVertexArrays(1, &vaoNegro);

	// Nota: Los VBOs se eliminan automáticamente cuando se eliminan los VAOs
	// porque fueron creados dentro de la función createColoredCubeVAO()

	glfwTerminate();
	return EXIT_SUCCESS;
}

// Función para interactuar en tiempo real con el teclado y el ejecutable
void Inputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot += 0.4f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot -= 0.4f;
}

