#version 330
// Color input from Vertex shader
in vec4 vColor;
in vec3 vNormal;
in vec3 vPosition;

// Final fragment fColor
out vec4 fColor;

// Light color RGB value
uniform vec3 uLightColor;
// Light object and camera position vectors
uniform vec3 uLightPosition;
uniform vec3 uCameraPosition;

// Strength ranges 0.0f - 1.0f
uniform float uAmbientStrength;  // 0.2f
uniform float uSpecularStrength; // 0.25f
// 32, 64, 128, 256
uniform int uSpecularShine; // 64

void main()
{
  // Ambient lighting
  vec3 ambient = uAmbientStrength * uLightColor;

  // Diffuse lighting
  vec3 norm = normalize(vNormal);
  vec3 lightDir = normalize(uLightPosition - vPosition);
  float diff = max(dot(norm, lightDir), 0.0f);

  vec3 diffuse = diff * uLightColor;

  // Specular lighting
  vec3 viewDir = normalize(uCameraPosition - vPosition);
  vec3 reflectDir = reflect(-lightDir, norm);

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), uSpecularShine);
  vec3 specular = uSpecularStrength * spec * uLightColor;

  //
  // Final calculation

  // Strip vColor to vec3 to drop alpha
  vec3 result = (ambient + diffuse + specular) * vec3(vColor);

  // Final output
  fColor = vec4(result, 1.0f); // Reapply alpha
}
