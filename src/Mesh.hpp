#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <functional> // Necessário para std::hash
#include "algebra.hpp"
#include "Shader.hpp"

// Usaremos esta struct para passar os dados de cada vértice de forma organizada
struct Vertex {
    alg::Vec3 Position;
    alg::Vec3 Normal;
    alg::Vec2 TexCoords;

    // Operador de igualdade para a struct Vertex
    bool operator==(const Vertex& other) const {
        return Position == other.Position &&
               Normal == other.Normal &&
               TexCoords == other.TexCoords;
    }
};

// --- ESPECIALIZAÇÃO DO std::hash ---
// Precisamos colocar este bloco no namespace std para ensinar o C++
// a fazer o hash das nossas structs personalizadas.
namespace std {
    // 1. Primeiro, ensinamos a fazer o hash das partes menores (Vec2 e Vec3)
    template<> struct hash<alg::Vec2> {
        size_t operator()(const alg::Vec2& vec) const {
            auto h1 = hash<float>{}(vec.x);
            auto h2 = hash<float>{}(vec.y);
            return h1 ^ (h2 << 1);
        }
    };

    template<> struct hash<alg::Vec3> {
        size_t operator()(const alg::Vec3& vec) const {
            auto h1 = hash<float>{}(vec.x);
            auto h2 = hash<float>{}(vec.y);
            auto h3 = hash<float>{}(vec.z);
            return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
        }
    };

    // 2. Agora, ensinamos a fazer o hash da struct Vertex, que usa os de cima
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            size_t h1 = hash<alg::Vec3>()(vertex.Position);
            size_t h2 = hash<alg::Vec3>()(vertex.Normal);
            size_t h3 = hash<alg::Vec2>()(vertex.TexCoords);

            return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
        }
    };
} // fim do namespace std

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