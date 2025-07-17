#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

class Texture {
public:
    // Construtor: carrega a imagem e cria o objeto de textura
    Texture(const char* path);
    Texture(unsigned int existingID);
    // Destrutor
    ~Texture();

    // Vincula (ativa) a textura para uso em renderização
    void bind(unsigned int slot = 0) const;

private:
    unsigned int m_ID;
    int m_width, m_height, m_nrChannels;
};

#endif //TEXTURE_HPP