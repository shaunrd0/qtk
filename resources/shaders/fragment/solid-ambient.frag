#version 330
// Color input from Vertex shader
in vec4 vColor;

// Final fragment color
out vec4 fColor;

// Light color RGB value
uniform vec3 uLightColor;
// Strength ranges 0.0f - 1.0f
uniform float uAmbientStrength;  // 0.2f

void main()
{
  vec3 ambient = uAmbientStrength * uLightColor;

  vec3 result = ambient * vec3(vColor); // Strip aColor to vec3 to drop alpha
  fColor = vec4(result, 1.0f); // Reapply alpha
}
