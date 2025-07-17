#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include "algebra.hpp"
#include "Shader.hpp"

// Usaremos esta struct para passar os dados de cada vértice de forma organizada
struct Vertex {
    alg::Vec3 Position;
    alg::Vec3 Normal;
    alg::Vec2 TexCoords;
};

class Mesh {
public:
    // Dados da malha
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;

    // Construtor
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

    // Renderiza a malha
    void draw(Shader &shader);

private:
    // IDs dos buffers da GPU
    unsigned int VAO, VBO, EBO;

    // Função de inicialização que cria os buffers
    void setupMesh();
};

#endif //MESH_HPP