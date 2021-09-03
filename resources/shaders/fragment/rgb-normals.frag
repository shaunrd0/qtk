#version 330
// Color input from Vertex shader
in vec3 vNormal;

// Final fragment color
out vec4 fColor;

void main()
{
  fColor = vec4(abs(vNormal), 1.0f); // Reapply alpha
}
