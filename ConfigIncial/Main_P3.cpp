/*
	Previo Práctica 3
	Cruz Miranda Luis Eduardo
	Fecha de entrega: 31 de agosto de 2025
	Número de cuenta: 319148204
*/

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  //Se integran librerias glm
#include <glm/gtc/type_ptr.hpp>



// Shaders
#include "Shader.h"

const GLint WIDTH = 800, HEIGHT = 600;


int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecciones y transformaciones basicas", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificaci�n de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}


	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers
	// use with Orthographic Projection

	GLfloat vertices[] = {  //Vertices adaptados para una proyección ortogonal
       -0.5f*500, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,//Front
		0.5f * 500, -0.5f * 500, 0.5f * 500,  1.0f, 0.0f,0.0f,
		0.5f * 500,  0.5f * 500, 0.5f * 500,  1.0f, 0.0f,0.0f,
		0.5f * 500,  0.5f * 500, 0.5f * 500,  1.0f, 0.0f,0.0f,
		-0.5f * 500,  0.5f * 500, 0.5f * 500, 1.0f, 0.0f,0.0f,
		-0.5f * 500, -0.5f * 500, 0.5f * 500, 1.0f, 0.0f,0.0f,
		
	    -0.5f * 500, -0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,//Back
		 0.5f * 500, -0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,
		 0.5f * 500,  0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,
		 0.5f * 500,  0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,
	    -0.5f * 500,  0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,
	    -0.5f * 500, -0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,
		
		 0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,1.0f,
		 0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 0.0f,1.0f,
		 0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 0.0f,1.0f,
		 0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 0.0f,1.0f,
		 0.5f * 500,  0.5f * 500,  0.5f * 500,  0.0f, 0.0f,1.0f,
		 0.5f * 500,  -0.5f * 500, 0.5f * 500, 0.0f, 0.0f,1.0f,
      
		-0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 1.0f,0.0f,
		-0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,0.0f,
		-0.5f * 500, -0.5f * 500, -0.5f * 500,  1.0f, 1.0f,0.0f,
		-0.5f * 500, -0.5f * 500, -0.5f * 500,  1.0f, 1.0f,0.0f,
		-0.5f * 500, -0.5f * 500,  0.5f * 500,  1.0f, 1.0f,0.0f,
		-0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 1.0f,0.0f,
		
		-0.5f * 500, -0.5f * 500, -0.5f * 500, 0.0f, 1.0f,1.0f,
		0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,1.0f,
		0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 1.0f,1.0f,
		0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 1.0f,1.0f,
		-0.5f * 500, -0.5f * 500,  0.5f * 500, 0.0f, 1.0f,1.0f,
		-0.5f * 500, -0.5f * 500, -0.5f * 500, 0.0f, 1.0f,1.0f,
		
		-0.5f * 500,  0.5f * 500, -0.5f * 500, 1.0f, 0.2f,0.5f,
		0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 0.2f,0.5f,
		0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.2f,0.5f,
		0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.2f,0.5f,
		-0.5f * 500,  0.5f * 500,  0.5f * 500, 1.0f, 0.2f,0.5f,
		-0.5f * 500,  0.5f * 500, -0.5f * 500, 1.0f, 0.2f,0.5f,
	};
	

	//use with Perspective Projection  //vertices adapatados para una proyección en perspectiva 
	//float vertices[] = {
	//	-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,//Front
	//	0.5f, -0.5f, 0.5f,  1.0f, 0.0f,0.0f,
	//	0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,
	//	0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,
	//	-0.5f,  0.5f, 0.5f, 1.0f, 0.0f,0.0f,
	//	-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,
	//	
	//    -0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,//Back
	//	 0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	//	 0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	//	 0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	//    -0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	//    -0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	//	
	//	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
	//	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,1.0f,
	//	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
	//	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
	//	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,
	//	 0.5f,  -0.5f, 0.5f, 0.0f, 0.0f,1.0f,
 //     
	//	-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
	//	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
	//	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
	//	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
	//	-0.5f, -0.5f,  0.5f,  1.0f, 1.0f,0.0f,
	//	-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
	//	
	//	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,
	//	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,1.0f,
	//	0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
	//	0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
	//	-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,1.0f,
	//	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,
	//	
	//	-0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
	//	0.5f,  0.5f, -0.5f,  1.0f, 0.2f,0.5f,
	//	0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
	//	0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
	//	-0.5f,  0.5f,  0.5f, 1.0f, 0.2f,0.5f,
	//	-0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
	//};




	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	
	glm::mat4 projection=glm::mat4(1); //Libreria glm que permite generar variables de tipo matrices de cuatro elmentos

	//projection = glm::perspective(45.0f, (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	
	projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);//Izq,Der,Fondo,Alto,Cercania,Lejania //Límites
	
	// 0.1f, 1000.0f son distancias; 0.1 es la distancia donde se comienza a visualizar la proyeción y 1000.0 es el límite máx que se alcanza a ver

	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glm::mat4 model = glm::mat4(1); //Matriz model
		glm::mat4 view = glm::mat4(1); //Matriz view

		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));
		//model = glm::rotate(model, 0.5f, glm::vec3(1.0f, 2.0f, 2.0f)); // use to compare orthographic and perspective projection
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		view = glm::translate( view, glm::vec3( 
			screenWidth / 2, screenHeight / 5,-700.0f ) ); // use with orthographic projection

		//Comunicar la información con los shaders
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");

		//Los Uniforms permiten enviar la información, son variables, del codigo base a la información de los shaders 

		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Nuevos elementos

		model = glm::mat4(1); //	Inicializar matriz

		// Acciones / Operaciones
		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, -4.0f));
		model = glm::rotate(model, 0.5f, glm::vec3(1.0f, -2.0f, -2.0f)); // use to compare orthographic and perspective projection
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

		//Mandar info al shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//Dibujamos caja 
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glBindVertexArray(0);



		
		
		

		// Swap the screen buffers
		glfwSwapBuffers(window);
	
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;

  

}




