#version 330
// Color input from Vertex shader
in vec4 vColor;
in vec3 vNormal;
in vec3 vPosition;

// Final fragment color
out vec4 fColor;

// Light color RGB value
uniform vec3 uLightColor;
// Light object position vector
uniform vec3 uLightPosition;

// Strength ranges 0.0f - 1.0f
uniform float uAmbientStrength;  // 0.2f

void main()
{
  // Ambient lighting
  vec3 ambient = uAmbientStrength * uLightColor;

  // Diffuse lighting
  vec3 norm = normalize(vNormal);
  vec3 lightDir = normalize(uLightPosition - vPosition);
  float diff = max(dot(norm, lightDir), 0.0f);

  vec3 diffuse = diff * uLightColor;

  // Strip vColor to vec3 to drop alpha
  vec3 result = (ambient + diffuse) * vec3(vColor);

  // Final output
  fColor = vec4(result, 1.0f); // Reapply alpha
}
