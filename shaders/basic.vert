#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord; // Assumindo que você ainda tem texturas

out vec2 TexCoord;

// --- A CORREÇÃO ESTÁ AQUI ---
// Declaramos as 3 matrizes que o C++ está enviando
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Usamos as 3 matrizes na ordem correta (Projeção * Visão * Modelo)
    // para calcular a posição final do vértice.
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}