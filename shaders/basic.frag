#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// A textura que vamos amostrar
uniform sampler2D ourTexture;

void main()
{
    // A cor do fragmento agora é a cor da textura na coordenada recebida
    FragColor = texture(ourTexture, TexCoord);
}