#version 330
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

// Solid color input from application
uniform vec3 uColor;

// Color to output to fragment shader
out vec4 vColor;
out vec3 vPosition;
out vec3 vNormal;

// Model View Projection matrix found by P * V * M
uniform mat3 uModelInverseTransposed;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
  gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);

  vPosition = vec3(uModel * vec4(aPosition, 1.0f));
  vColor = vec4(uColor, 1.0f);

  vNormal = uModelInverseTransposed * aNormal;
  //  vNormal = aNormal; // This is possible without the additional uniform
  // Or you could find this on the GPU using the uModel matrix
  //  vNormal = mat3(transpose(inverse(uModel))) * aNormal;
}
