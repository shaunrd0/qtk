#version 330 core
out vec4 fColor;

in vec2 vTextureCoord;

uniform sampler2D texture_diffuse1;

void main()
{
    fColor = texture(texture_diffuse1, vTextureCoord);
}
