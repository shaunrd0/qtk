#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

struct Light {
    vec3 position;

// Light colors RGB value
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light uLight;

out VS_OUT {
    vec2 textureCoord;
    vec3 position;
    vec3 normal;
    vec3 tangentLight;
    vec3 tangentView;
    vec3 tangentFrag;
    Light light;
} vOut;

// Model View Projection matrix found by P * V * M
struct MVP {
    mat4 model;
    mat4 view;
    mat4 projection;
// Additional uniform to allow CPU to provide mat3(transpose(inverse(uModel)))
// + Makes a big difference in quality of diffuse shading
// + QMatrix4x4.normalMatrix() returns a mat3 that can be used as a uniform
    mat3 normalMatrix;
};
uniform MVP uMVP;

vec4 VertexPosition(in MVP mvp, in vec3 position)
{
    return mvp.projection * mvp.view * mvp.model * vec4(position, 1.0f);
}

// Light object and camera position vectors
uniform vec3 uCameraPosition;

void main()
{
    // Finx TBN 3x3 matrix values for normals
    vec3 T = normalize(vec3(uMVP.model * vec4(aTangent, 0.0)));
    vec3 B = normalize(vec3(uMVP.model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(uMVP.model * vec4(aNormal, 0.0)));
    mat3 TBN = transpose(mat3(T, B, N));

    // Set tangent postiions for fragment shader
    vOut.tangentLight = TBN * uLight.position;
    vOut.tangentView = TBN * uCameraPosition;
    vOut.tangentFrag = TBN * vec3(uMVP.model * vec4(aPosition, 1.0f));

    // Set normal, position, and texture coordinate for frag shader
    vOut.textureCoord = aTextureCoord;
    vOut.normal = uMVP.normalMatrix * aNormal;
    vOut.position = vec3(uMVP.model * vec4(aPosition, 1.0f));
    vOut.light = uLight;

    gl_Position = VertexPosition(uMVP, aPosition);
}
