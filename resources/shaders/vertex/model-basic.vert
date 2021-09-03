#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoord;

out vec2 vTextureCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    vTextureCoord = aTextureCoord;
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}
