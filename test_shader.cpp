#include <iostream>
#include <fstream>
#include <sstream>

int main() {
    std::cout << "Testing shader file loading..." << std::endl;
    
    // Test vertex shader
    std::ifstream vShaderFile("shaders/basic.vert");
    if (!vShaderFile.is_open()) {
        std::cerr << "ERROR: Cannot open vertex shader file" << std::endl;
        return -1;
    }
    
    std::stringstream vShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    std::string vertexCode = vShaderStream.str();
    vShaderFile.close();
    
    std::cout << "Vertex shader loaded successfully. Size: " << vertexCode.size() << std::endl;
    
    // Test fragment shader
    std::ifstream fShaderFile("shaders/basic.frag");
    if (!fShaderFile.is_open()) {
        std::cerr << "ERROR: Cannot open fragment shader file" << std::endl;
        return -1;
    }
    
    std::stringstream fShaderStream;
    fShaderStream << fShaderFile.rdbuf();
    std::string fragmentCode = fShaderStream.str();
    fShaderFile.close();
    
    std::cout << "Fragment shader loaded successfully. Size: " << fragmentCode.size() << std::endl;
    
    return 0;
}
