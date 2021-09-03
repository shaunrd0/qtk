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
