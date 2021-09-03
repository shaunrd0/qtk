#version 330
in vec2 vTextureCoord;

uniform sampler2D uTexture;

out vec4 fColor;

void main()
{
  vec4 texCol = texture(uTexture, vTextureCoord);
  fColor = texCol;
}
