#version 330
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

// Solid color input from application

// Color to output to fragment shader
out vec3 vNormal;

// Model View Projection matrix found by P * V * M
//uniform mat3 uModelInverseTransposed;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
  gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);

  // This isn't a shader we plan to use heavily, it's useful for testing normals
  // + We can see what the value of normals are on the object based on color
  vNormal = mat3(transpose(inverse(uModel))) * aNormal;
  //  vNormal = uModelInverseTransposed * aNormal;
  //  vNormal = aNormal; // This is possible without the additional uniform
  // Or you could find this on the GPU using the uModel matrix
}
