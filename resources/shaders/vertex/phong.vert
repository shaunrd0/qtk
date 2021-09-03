#version 330
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

out vec3 vPosition;
out vec3 vNormal;

// Model View Projection matrix found by P * V * M
struct MVP {
  mat4 model;
  mat4 view;
  mat4 projection;
  // Additional uniform to allow CPU to provide mat3(transpose(inverse(uModel)))
  // + Makes a big difference in quality of diffuse shading
  // + QMatrix4x4.normalMatrix() returns a mat3 that can be used as a uniform
  mat3 normalMatrix;
};
uniform MVP uMVP;

vec4 VertexPosition(in MVP mvp, in vec3 position)
{
  return mvp.projection * mvp.view * mvp.model * vec4(position, 1.0f);
}

void main()
{
  // Find normals, pass to fragment shader
  vNormal = uMVP.normalMatrix * aNormal;
  //  vNormal = aNormal; // This is possible without the additional uniform
  // Or you could find this on the GPU using the uModel matrix
  //  vNormal = mat3(transpose(inverse(uModel))) * aNormal;

  // Set the vertex position to pass to fragment shader using only model matrix
  vPosition = vec3(uMVP.model * vec4(aPosition, 1.0f));

  // Set vertex gl_Position using MVP matrix
  gl_Position = VertexPosition(uMVP, aPosition);
}
