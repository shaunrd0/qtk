#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoord;

out vec2 vTextureCoord;
out vec3 vPosition;
out vec3 vNormal;

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

vec4 VertexPosition(in MVP mvp, in vec3 position)
{
    return mvp.projection * mvp.view * mvp.model * vec4(position, 1.0f);
}

uniform MVP uMVP;

void main()
{
    vTextureCoord = aTextureCoord;
    vNormal = uMVP.normalMatrix * aNormal;
    vPosition = vec3(uMVP.model * vec4(aPosition, 1.0f));
    gl_Position = VertexPosition(uMVP, aPosition);
}
