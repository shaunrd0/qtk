#version 330
layout(location = 0) in vec3 aPosition;
//layout(location = 1) in vec3 vNormal;

// Solid color input from application
uniform vec3 uColor;

// Color to output to fragment shader
out vec4 vColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
  gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);

  vColor = vec4(uColor, 1.0f);
}
