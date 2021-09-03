#version 330
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

// Solid color input from application
uniform vec3 uColor;

// Color to output to fragment shader
out vec4 vColor;
out vec3 vPosition;
out vec3 vNormal;

// Additional uniform to allow CPU to provide mat3(transpose(inverse(uModel)))
// + Makes a big difference in quality of diffuse shading
// + QMatrix4x4.normalMatrix() returns a mat3 that can be used as a uniform
uniform mat3 uModelInverseTransposed;
// Model View Projection matrix found by P * V * M
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
  mat4 mvp = uProjection * uView * uModel;

  gl_Position = mvp * vec4(aPosition, 1.0);

  vPosition = vec3(uModel * vec4(aPosition, 1.0f));
  vColor = vec4(uColor, 1.0f);

  vNormal = uModelInverseTransposed * aNormal;
  //  vNormal = aNormal; // This is possible without the additional uniform
  // Or you could find this on the GPU using the uModel matrix
  //  vNormal = mat3(transpose(inverse(uModel))) * aNormal;
}
