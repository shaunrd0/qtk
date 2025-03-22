/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2025 Shaun Reed, all rights reserved                ##
## About: Default GLSL shaders to use for objects if no shader if provided.   ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/
#ifndef QTK_SHADERS_H
#define QTK_SHADERS_H

//
// Model

#define QTK_SHADER_VERTEX_MODEL \
  R"(
#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoord;

out vec2 vTextureCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    vTextureCoord = aTextureCoord;
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}
)"

#define QTK_SHADER_FRAGMENT_MODEL \
  R"(
#version 330 core
out vec4 fColor;

in vec2 vTextureCoord;

uniform sampler2D texture_diffuse1;

void main()
{
    fColor = texture(texture_diffuse1, vTextureCoord);
}
)"

//
// MeshRenderer

#define QTK_SHADER_VERTEX_MESH \
  R"(
#version 330
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;

out vec4 vColor;

uniform mat4 uModel;  // Model
uniform mat4 uView; // View
uniform mat4 uProjection;  // Projection

void main()
{
  gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);

  vColor = vec4(aColor, 1.0f);
}
)"

#define QTK_SHADER_FRAGMENT_MESH \
  R"(
#version 330
in vec4 vColor;

out vec4 fColor;

void main()
{
  fColor = vColor;
}
)"

//
// Skybox

#define QTK_SHADER_VERTEX_SKYBOX \
  R"(
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
)"

#define QTK_SHADER_FRAGMENT_SKYBOX \
  R"(
#version 330
uniform samplerCube uTexture;

varying vec3 vTexCoord;

void main()
{
  gl_FragColor = texture(uTexture, vTexCoord);
}
)"

#endif  // QTK_SHADERS_H
