#version 330
layout(location = 0) in vec3 aPosition;

out vec3 vTexCoord;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

void main()
{
  // Strip translation column from camera's 4x4 matrix
  mat4 view = mat4(mat3(uViewMatrix));
  gl_Position = uProjectionMatrix * view * vec4(aPosition, 1.0);
  vTexCoord = aPosition;
}