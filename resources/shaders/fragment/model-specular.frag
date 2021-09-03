#version 330 core
// Input from Vertex shader
in vec2 vTextureCoord;
in vec3 vNormal;
in vec3 vPosition;

// Final fragment fColor
out vec4 fColor;

// Light object and camera position vectors
uniform vec3 uCameraPosition;
struct Light {
    vec3 position;

// Light colors RGB value
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light uLight;

struct Material {
// Strength ranges 0.0f - 1.0f
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
// 32, 64, 128, 256
    float shine;

// Material color values
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Material uMaterial;

struct Result {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 sum;
};

void SumResult(inout Result result)
{
    result.sum = result.ambient + result.diffuse + result.specular;
}

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
    Result result;

    // Diffuse texture for ambient and diffuse lighting
    vec3 diffuseTexture = vec3(texture(texture_diffuse1, vTextureCoord));

    // Ambient lighting
    vec3 ambientLighting = uLight.ambient * uMaterial.ambient;
    result.ambient = ambientLighting * diffuseTexture;
    result.ambient *= uMaterial.ambientStrength;

    // Diffuse lighting
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(uLight.position - vPosition);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuseLighting = uLight.diffuse * diff * uMaterial.diffuse;
    result.diffuse = diffuseLighting * diffuseTexture;
    result.diffuse *= uMaterial.diffuseStrength;

    // Specular lighting
    vec3 specularTexture = vec3(texture(texture_specular1, vTextureCoord));

    vec3 viewDir = normalize(uCameraPosition - vPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float angleAlpha = max(dot(viewDir, reflectDir), 0.0f);
    float spec = pow(angleAlpha, uMaterial.shine);
    vec3 specLighting = uLight.specular * spec;
    result.specular = specLighting * specularTexture;
    result.specular *= uMaterial.specularStrength;

    SumResult(result);
    fColor = vec4(result.sum, 1.0f);
}
