#include "Shader.hpp"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // 1. Obter o código-fonte dos shaders a partir dos caminhos
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // Abrir arquivos
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    
    // Verificar se os arquivos foram abertos corretamente
    if (!vShaderFile.is_open()) {
        std::cerr << "ERRO::SHADER::VERTEX_SHADER_NAO_ENCONTRADO: " << vertexPath << std::endl;
        return;
    }
    if (!fShaderFile.is_open()) {
        std::cerr << "ERRO::SHADER::FRAGMENT_SHADER_NAO_ENCONTRADO: " << fragmentPath << std::endl;
        return;
    }
    
    std::stringstream vShaderStream, fShaderStream;
    // Ler o conteúdo dos arquivos para os streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    // Fechar os arquivos
    vShaderFile.close();
    fShaderFile.close();
    // Converter o stream em string
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. Compilar os shaders
    unsigned int vertex, fragment;

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // Programa de Shader
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // Deletar os shaders pois eles já estão linkados no nosso programa e não são mais necessários
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const {
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, const alg::Vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value.x);
}

// Em Shader.cpp
void Shader::setMat4(const std::string &name, const alg::Mat4 &mat) const {
    // Pega a localização do uniform
    int location = glGetUniformLocation(ID, name.c_str());

    // --- LINHA DE DIAGNÓSTICO ---
    // Se a localização for -1, significa que o shader não encontrou o uniform.
    if (location == -1) {
        static std::set<std::string> warned_uniforms;
        if (warned_uniforms.find(name) == warned_uniforms.end()) {
            std::cerr << "AVISO: Uniform '" << name << "' nao encontrado no shader!" << std::endl;
            warned_uniforms.insert(name);
        }
    }

    // Envia os dados para a GPU
    glUniformMatrix4fv(location, 1, GL_FALSE, mat.m);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERRO::SHADER_COMPILATION_ERROR de tipo: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        } else {
            std::cout << "SUCESSO: Shader " << type << " compilado com sucesso!" << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERRO::PROGRAM_LINKING_ERROR de tipo: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        } else {
            std::cout << "SUCESSO: Programa shader linkado com sucesso!" << std::endl;
        }
    }
}
