#include "Texture.hpp"
#include <glad/glad.h>
#include <iostream>

// Coloque esta linha aqui! É uma prática melhor colocar a implementação
// da biblioteca stb_image no arquivo .cpp que de fato a utiliza.
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char* path) {
    // Gera e vincula o objeto de textura
    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);

    // Define os parâmetros de wrapping (repetição) e filtering (interpolação)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Inverte a imagem no eixo Y durante o carregamento, pois o OpenGL espera as coordenadas de baixo para cima
    stbi_set_flip_vertically_on_load(true);

    // Carrega a imagem do disco
    unsigned char* data = stbi_load(path, &m_width, &m_height, &m_nrChannels, 0);
    if (data) {
        // Determina o formato da imagem (RGB ou RGBA com transparência)
        GLenum format = GL_RGB;
        if (m_nrChannels == 4) {
            format = GL_RGBA;
        }

        std::cout << "SUCESSO: Textura carregada - " << path << " (" << m_width << "x" << m_height << ", " << m_nrChannels << " canais)" << std::endl;
        
        // Envia os dados da imagem para a GPU
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "ERRO: Falha ao carregar a textura no caminho: " << path << std::endl;
    }

    // Libera a memória da imagem da CPU, pois ela já está na GPU
    stbi_image_free(data);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_ID);
}

void Texture::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}