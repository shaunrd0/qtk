#version 330 core
struct Light {
    vec3 position;
// Light colors RGB value
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Input from Vertex shader
in VS_OUT {
// Frag position, UV coordinates, normals
    vec2 textureCoord;
    vec3 position;
    vec3 normal;
// Tangent positions
    vec3 tangentLight;
    vec3 tangentView;
    vec3 tangentFrag;
    Light light;
} vOut;

// Final fragment fColor
out vec4 fColor;

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
uniform sampler2D texture_normal1;

void main()
{
    Result result;

    // Diffuse texture for ambient and diffuse lighting
    vec3 diffuseTexture = vec3(texture(texture_diffuse1, vOut.textureCoord));

    // Ambient lighting
    vec3 ambientLighting = vOut.light.ambient * uMaterial.ambient;
    result.ambient = ambientLighting * diffuseTexture;
    result.ambient *= uMaterial.ambientStrength;

    // Diffuse lighting
    vec3 normalTexture = texture(texture_normal1, vOut.textureCoord).rgb;
    normalTexture = normalize(normalTexture * 2.0f - 1.0f);

    vec3 lightDir = normalize(vOut.tangentLight - vOut.tangentFrag);
    float diff = max(dot(lightDir, normalTexture), 0.0f);
    vec3 diffuseLighting = vOut.light.diffuse * diff * uMaterial.diffuse;
    result.diffuse = diffuseLighting * diffuseTexture;
    result.diffuse *= uMaterial.diffuseStrength;

    // Specular lighting
    vec3 specularTexture = vec3(texture(texture_specular1, vOut.textureCoord));

    vec3 viewDir = normalize(vOut.tangentView - vOut.tangentFrag);
    vec3 reflectDir = reflect(-lightDir, normalTexture);
    vec3 halfDir = normalize(lightDir + viewDir);
    float angleAlpha = max(dot(normalTexture, halfDir), 0.0f);
    float spec = pow(angleAlpha, uMaterial.shine);
    vec3 specLighting = vOut.light.specular * spec;
    result.specular = specLighting * specularTexture;
    result.specular *= uMaterial.specularStrength;

    SumResult(result);
    fColor = vec4(result.sum, 1.0f);
}
