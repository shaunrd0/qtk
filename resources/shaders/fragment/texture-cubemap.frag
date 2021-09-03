#version 330
in vec3 vPosition;

uniform samplerCube uTexture;

out vec4 fColor;

void main()
{
  vec4 texCol = texture(uTexture, vPosition);
  fColor = texCol;
}
