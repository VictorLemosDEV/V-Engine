#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <string>
#include "Mesh.hpp"
#include "Shader.hpp"

class Model {
public:
    // Construtor que carrega o modelo a partir de um arquivo .obj
    Model(const std::string& path);

    // Desenha o modelo (e todas as suas malhas)
    void draw(Shader& shader);

private:
    // Lista de malhas que compõem o modelo
    std::vector<Mesh> meshes;
    // Diretório do arquivo do modelo, para carregar texturas
    std::string directory;

    // Função principal que carrega os dados do arquivo .obj
    void loadModel(const std::string& path);
};

#endif //MODEL_HPP