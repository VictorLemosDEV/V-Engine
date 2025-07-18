#version 330 core
#define MAX_LIGHTS 8

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// Estrutura do Material
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    vec3 ambient;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};
uniform Material material;

// Estrutura da Luz
struct Light {
    int type;           // 0 = point, 1 = directional, 2 = spot
    bool enabled;
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float range;
    float innerAngle;   // cos(innerAngle)
    float outerAngle;   // cos(outerAngle)
};

uniform Light lights[MAX_LIGHTS];
uniform int activeLightCount;
uniform vec3 viewPos;

// Função para cálculo da contribuição de luz
vec3 calculateLightContribution(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    if (!light.enabled) return vec3(0.0);

    vec3 lightDir;
    float attenuation = 1.0;

    // Cálculos específicos por tipo de luz
    if (light.type == 1) { // Directional
        lightDir = normalize(-light.direction);
    } else { // Point or Spot
        lightDir = normalize(light.position - fragPos);
        float distance = length(light.position - fragPos);
        attenuation = 1.0 / (1.0 + 0.1*distance + 0.01*(distance*distance));
        attenuation *= smoothstep(light.range, light.range*0.9, distance);
    }

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.color * diff * light.intensity;

    // Specular (Blinn-Phong)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.color * spec * light.intensity;

    // Spot light factor
    if (light.type == 2) { // Spot
        float theta = dot(lightDir, normalize(-light.direction));
        float epsilon = light.innerAngle - light.outerAngle;
        float spotIntensity = clamp((theta - light.outerAngle) / epsilon, 0.0, 1.0);
        diffuse *= spotIntensity;
        specular *= spotIntensity;
    }

    return (diffuse + specular) * attenuation;
}

void main() {
    // Normaliza a normal (pode ser distorcida por escala não-uniforme)
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Iluminação ambiente
    vec3 result = material.ambient;

    // Aplica textura difusa
    vec4 texColor = texture(material.diffuse, TexCoords);
    vec3 effectiveDiffuse = material.diffuseColor * texColor.rgb;

    // Calcula contribuição de todas as luzes
    for (int i = 0; i < activeLightCount; i++) {
        vec3 lightContrib = calculateLightContribution(lights[i], norm, FragPos, viewDir);
        result += lightContrib * effectiveDiffuse;

        // Aplica textura especular (usa o canal vermelho como máscara)
        float specularStrength = texture(material.specular, TexCoords).r;
        result += lightContrib * material.specularColor * specularStrength;
    }

    // Saída final
    FragColor = vec4(result, texColor.a);
}