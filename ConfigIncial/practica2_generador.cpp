/*
    Práctica 2
    Cruz Miranda Luis Eduardo
    Fecha de entrega: 24 de agosto de 2025
    Número de cuenta: 319148204
*/

#include <iostream>
#include <vector>
#include <fstream>

// Configuración de la grilla (20x20)
const int GRID_WIDTH = 20;
const int GRID_HEIGHT = 20;
const float TOTAL_WIDTH = 2.0f;
const float TOTAL_HEIGHT = 2.0f;
const float ORIGIN_X = -1.0f;
const float ORIGIN_Y = 1.0f;

const float PIXEL_WIDTH = TOTAL_WIDTH / GRID_WIDTH;   // = 0.1f
const float PIXEL_HEIGHT = TOTAL_HEIGHT / GRID_HEIGHT; // = 0.1f

struct PixelCoord {
    int fila, columna;
    float r, g, b;
};

float gridToOpenGL_X(int columna) {
    return ORIGIN_X + (columna * PIXEL_WIDTH);
}

float gridToOpenGL_Y(int fila) {
    return ORIGIN_Y - (fila * PIXEL_HEIGHT);
}

void generarVerticesPixel(int fila, int columna, float r, float g, float b,
    std::vector<float>& vertices) {

    float x = gridToOpenGL_X(columna);
    float y = gridToOpenGL_Y(fila);

    // 4 vértices del cuadrado (cada uno con 6 componentes: x,y,z,r,g,b) //z no se utiliza
    float pixelVertices[] = {
        // Vértice 0: esquina inferior izquierda
        x,               y - PIXEL_HEIGHT, 0.0f,  r, g, b,
        // Vértice 1: esquina inferior derecha  
        x + PIXEL_WIDTH, y - PIXEL_HEIGHT, 0.0f,  r, g, b,
        // Vértice 2: esquina superior derecha
        x + PIXEL_WIDTH, y,                0.0f,  r, g, b,
        // Vértice 3: esquina superior izquierda
        x,               y,                0.0f,  r, g, b,
    };

    for (int i = 0; i < 24; i++) {
        vertices.push_back(pixelVertices[i]);
    }
}

void generarIndicesPixel(int numeroPixel, std::vector<unsigned int>& indices) {
    int base = numeroPixel * 4;

    indices.push_back(base + 0);
    indices.push_back(base + 1);
    indices.push_back(base + 2);

    indices.push_back(base + 2);
    indices.push_back(base + 3);
    indices.push_back(base + 0);
}

// Función para escribir números float correctamente
void escribirFloat(std::ofstream& archivo, float valor) {
    if (valor == 0.0f) {
        archivo << "0.0f";
    }
    else if (valor == 1.0f) {
        archivo << "1.0f";
    }
    else if (valor == 0.5f) {
        archivo << "0.5f";
    }
    else {
        // Para números calculados, usar un formato específico
        int entero = (int)(valor * 100); // Multiplicar por 100 para obtener 2 decimales
        float resultado = entero / 100.0f;

        if (resultado == (int)resultado) {
            archivo << (int)resultado << ".0f";
        }
        else {
            archivo << resultado << "f";
        }
    }
}

int main() {
    // PATO COMPLETO CON TODOS LOS COLORES
    std::vector<PixelCoord> pato = {

        // CONTORNO (NEGRO)   // Cordenadas x,y RGB r,g,b
        {4,7, 0,0,0},   {4,8, 0,0,0},   {4,9, 0,0,0},   {4,10, 0,0,0},
        {5,6, 0,0,0},   {5,11, 0,0,0},
        {6,5, 0,0,0},   {6,12, 0,0,0},
        {7,5, 0,0,0},   {7,8, 0,0,0},   {7,12, 0,0,0},
        {8,3, 0,0,0},   {8,4, 0,0,0},   {8,5, 0,0,0},   {8,6, 0,0,0},
        {8,8, 0,0,0},   {8,12, 0,0,0},
        {9,3, 0,0,0},   {9,7, 0,0,0},   {9,12, 0,0,0},  {9,16, 0,0,0},  {9,17, 0,0,0},
        {10,4, 0,0,0},  {10,5, 0,0,0},  {10,6, 0,0,0},  {10,11, 0,0,0},
        {10,12, 0,0,0}, {10,13, 0,0,0}, {10,14, 0,0,0}, {10,15, 0,0,0}, {10,17, 0,0,0},
        {11,6, 0,0,0},  {11,17, 0,0,0},
        {12,5, 0,0,0},  {12,17, 0,0,0},
        {13,5, 0,0,0},  {13,10, 0,0,0}, {13,14, 0,0,0}, {13,17, 0,0,0},
        {14,5, 0,0,0},  {14,11, 0,0,0}, {14,12, 0,0,0}, {14,13, 0,0,0}, 
        {15,6, 0,0,0},  {15,15, 0,0,0},
        {16,7, 0,0,0},  {16,8, 0,0,0},  {16,9, 0,0,0},  {16,10, 0,0,0},
        {16,11, 0,0,0}, {16,12, 0,0,0}, {16,13, 0,0,0}, {16,14, 0,0,0},
        {17,8, 0,0,0},  {17,10, 0,0,0}, {17,12, 0,0,0}, {17,14, 0,0,0},
        {18,7, 0,0,0},  {18,10, 0,0,0}, {18,11, 0,0,0}, {18,14, 0,0,0},

        // PICO (NARANJA) 
        {9,4, 1.0f,0.5f,0.0f},  {9,5, 1.0f,0.5f,0.0f},  {9,6, 1.0f,0.5f,0.0f},

        // PATAS (NARANJA)
        {17,9, 1.0f,0.5f,0.0f}, {17,13, 1.0f,0.5f,0.0f},
        {18,8, 1.0f,0.5f,0.0f}, {18,9, 1.0f,0.5f,0.0f},
        {18,12, 1.0f,0.5f,0.0f}, {18,13, 1.0f,0.5f,0.0f},

        // CUERPO (AMARILLO) 
        {5,7, 1.0f,1.0f,0.0f},   {5,8, 1.0f,1.0f,0.0f},   {5,9, 1.0f,1.0f,0.0f},   {5,10, 1.0f,1.0f,0.0f},
        {6,6, 1.0f,1.0f,0.0f},   {6,7, 1.0f,1.0f,0.0f},   {6,8, 1.0f,1.0f,0.0f},   {6,9, 1.0f,1.0f,0.0f},
        {6,10, 1.0f,1.0f,0.0f},  {6,11, 1.0f,1.0f,0.0f},
        {7,6, 1.0f,1.0f,0.0f},   {7,7, 1.0f,1.0f,0.0f},   {7,9, 1.0f,1.0f,0.0f},   {7,10, 1.0f,1.0f,0.0f},  {7,11, 1.0f,1.0f,0.0f},
        {8,7, 1.0f,1.0f,0.0f},   {8,9, 1.0f,1.0f,0.0f},   {8,10, 1.0f,1.0f,0.0f},  {8,11, 1.0f,1.0f,0.0f},
        {9,8, 1.0f,1.0f,0.0f},   {9,9, 1.0f,1.0f,0.0f},   {9,10, 1.0f,1.0f,0.0f},  {9,11, 1.0f,1.0f,0.0f},
        {10,7, 1.0f,1.0f,0.0f},  {10,8, 1.0f,1.0f,0.0f},  {10,9, 1.0f,1.0f,0.0f},  {10,10, 1.0f,1.0f,0.0f}, {10,16, 1.0f,1.0f,0.0f},
        {11,7, 1.0f,1.0f,0.0f},  {11,8, 1.0f,1.0f,0.0f},  {11,9, 1.0f,1.0f,0.0f},  {11,10, 1.0f,1.0f,0.0f},
        {11,11, 1.0f,1.0f,0.0f}, {11,12, 1.0f,1.0f,0.0f}, {11,13, 1.0f,1.0f,0.0f}, {11,14, 1.0f,1.0f,0.0f},
        {11,15, 1.0f,1.0f,0.0f}, {11,16, 1.0f,1.0f,0.0f},
        {12,6, 1.0f,1.0f,0.0f},  {12,7, 1.0f,1.0f,0.0f},  {12,8, 1.0f,1.0f,0.0f},  {12,9, 1.0f,1.0f,0.0f},
        {12,10, 1.0f,1.0f,0.0f}, {12,11, 1.0f,1.0f,0.0f}, {12,12, 1.0f,1.0f,0.0f}, {12,13, 1.0f,1.0f,0.0f},
        {12,14, 1.0f,1.0f,0.0f}, {12,15, 1.0f,1.0f,0.0f}, {12,16, 1.0f,1.0f,0.0f},
        {13,6, 1.0f,1.0f,0.0f},  {13,7, 1.0f,1.0f,0.0f},  {13,8, 1.0f,1.0f,0.0f},  {13,9, 1.0f,1.0f,0.0f},
        {13,11, 1.0f,1.0f,0.0f}, {13,12, 1.0f,1.0f,0.0f}, {13,13, 1.0f,1.0f,0.0f}, {13,15, 1.0f,1.0f,0.0f}, {13,16, 1.0f,1.0f,0.0f},
        {14,6, 1.0f,1.0f,0.0f},  {14,7, 1.0f,1.0f,0.0f},  {14,8, 1.0f,1.0f,0.0f},  {14,9, 1.0f,1.0f,0.0f},
        {14,10, 1.0f,1.0f,0.0f}, {14,14, 1.0f,1.0f,0.0f}, {14,15, 1.0f,1.0f,0.0f},
        {15,7, 1.0f,1.0f,0.0f},  {15,8, 1.0f,1.0f,0.0f},  {15,9, 1.0f,1.0f,0.0f},  {15,10, 1.0f,1.0f,0.0f},
        {15,11, 1.0f,1.0f,0.0f}, {15,12, 1.0f,1.0f,0.0f}, {15,13, 1.0f,1.0f,0.0f}, {15,14, 1.0f,1.0f,0.0f}
    };

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i < pato.size(); i++) {
        PixelCoord pixel = pato[i];
        generarVerticesPixel(pixel.fila, pixel.columna,
            pixel.r, pixel.g, pixel.b, vertices);
        generarIndicesPixel(i, indices);
    }

    std::ofstream archivo("vertices_generados.txt");

    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo crear el archivo" << std::endl;
        return 1;
    }

    archivo << "= ARRAYS GENERADOS =" << std::endl;
    archivo << "Total pixeles: " << pato.size() << std::endl;
    archivo << "Total indices para glDrawElements: " << indices.size() << std::endl;

    // Generar array de vertices
    archivo << "\n// VERTICES:" << std::endl;
    archivo << "float vertices[] = {" << std::endl;

    for (size_t i = 0; i < vertices.size(); i += 6) {
        archivo << "    ";

        // Posición (x, y, z)
        escribirFloat(archivo, vertices[i]);
        archivo << ", ";
        escribirFloat(archivo, vertices[i + 1]);
        archivo << ", ";
        escribirFloat(archivo, vertices[i + 2]);
        archivo << ",   ";

        // Color (r, g, b)  
        escribirFloat(archivo, vertices[i + 3]);
        archivo << ", ";
        escribirFloat(archivo, vertices[i + 4]);
        archivo << ", ";
        escribirFloat(archivo, vertices[i + 5]);

        if (i + 6 < vertices.size()) {
            archivo << ",";
        }

        archivo << std::endl;
    }
    archivo << "};" << std::endl;

    // Generar array de indices
    archivo << "\n// INDICES:" << std::endl;
    archivo << "unsigned int indices[] = {" << std::endl;

    for (size_t i = 0; i < indices.size(); i += 6) {
        archivo << "    " << indices[i] << ", " << indices[i + 1] << ", " << indices[i + 2];
        archivo << ",    " << indices[i + 3] << ", " << indices[i + 4] << ", " << indices[i + 5];

        if (i + 6 < indices.size()) {
            archivo << ",";
        }
        archivo << std::endl;
    }
    archivo << "};" << std::endl;

    archivo << "\n// Ajuste en glDrawElements:" << std::endl;
    archivo << "glDrawElements(GL_TRIANGLES, " << indices.size() << ", GL_UNSIGNED_INT, 0);" << std::endl;

    archivo.close();

    std::cout << "Archivo generado: vertices_generados.txt" << std::endl;
    std::cout << "Pixeles: " << pato.size() << std::endl;
    std::cout << "Indices: " << indices.size() << std::endl;

    return 0;
}