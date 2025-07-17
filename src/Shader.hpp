#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <set>
#include "algebra.hpp" // Para usar nossas classes Vec3 e Mat4

class Shader {
public:
    // O ID do programa de shader
    unsigned int ID;

    // Construtor: lê e constrói os shaders a partir dos caminhos dos arquivos
    Shader(const char* vertexPath, const char* fragmentPath);

    // Ativa o shader para uso
    void use() const;

    // Funções para definir uniforms (variáveis globais nos shaders)
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, const alg::Vec3 &value) const;
    void setMat4(const std::string &name, const alg::Mat4 &mat) const;

private:
    // Função utilitária para checar erros de compilação/linkagem
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif //SHADER_HPP