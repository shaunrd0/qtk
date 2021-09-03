#version 330
// Color input from Vertex shader
in vec3 vNormal;
in vec3 vPosition;

// Final fragment fColor
out vec4 fColor;

// Light object and camera position vectors
uniform vec3 uCameraPosition;
struct Light {
  vec3 position;

  // Light colors RGB value
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform Light uLight;

struct Material {
  // Strength ranges 0.0f - 1.0f
  float ambientStrength;
  float diffuseStrength;
  float specularStrength;
  // 32, 64, 128, 256
  float shine;

  // Material color values
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform Material uMaterial;

struct Result {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  vec3 sum;
};

void SumResult(inout Result result)
{
  result.sum = result.ambient + result.diffuse + result.specular;
}

void main()
{
  // A struct to store lighting results as we finish calculating them
  // Valuse stored here will be applied to the final output of our shader
  Result result;

  // Ambient lighting
  result.ambient =  (uLight.ambient * uMaterial.ambient) * uMaterial.ambientStrength;

  //
  // Diffuse lighting

  // Normalize the provided normal vector
  // + Creates a vector with length of 1 in the same direction as vNormal
  vec3 norm = normalize(vNormal);
  // Get a vector from this frag to the lightSoruce
  vec3 lightDir = normalize(uLight.position - vPosition);
  // As the normal vector approaches an angle looking at lightDir vector
  // + If this is negative, the frag is on a surface opposite of the lightSource
  float diff = max(dot(norm, lightDir), 0.0f);
  result.diffuse = (uLight.diffuse * diff * uMaterial.diffuse) * uMaterial.diffuseStrength;

  //
  // Specular lighting

  // Get a vector from the camera to the fragment as our viewDir
  vec3 viewDir = normalize(uCameraPosition - vPosition);
  // Since lightDir is already a vector from this frag to the light source
  // reflectionDir is the opposite; A vector from the light to the frag
  vec3 reflectDir = reflect(-lightDir, norm);

  // Use dot product to check if viewDir and reflectDir angles are intersecting
  // -1.0f if they are opposite; 1.0 if they are looking at each other directly
  // + If this is negative, the lightSource is behind the player; Ignore it
  float angleAlpha = max(dot(viewDir, reflectDir), 0.0f);
  // As the angleAlpha approaches 1, raise to the power of uMaterial.shine
  float spec = pow(angleAlpha, uMaterial.shine);
  // Apply specular result to the
  result.specular = (uLight.specular * uMaterial.specular * spec) * uMaterial.specularStrength;

  //
  // Final calculation
  SumResult(result);

  // Final output
  fColor = vec4(result.sum, 1.0f); // Reapply alpha for opacity
}
