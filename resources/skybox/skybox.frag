#version 330
uniform samplerCube uTexture;

varying vec3 vTexCoord;

void main()
{
  gl_FragColor = texture(uTexture, vTexCoord);
}