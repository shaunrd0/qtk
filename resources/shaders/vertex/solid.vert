#version 330
layout(location = 0) in vec3 aPosition;

out vec4 vColor;

uniform vec3 uColor;

void main()
{
  gl_Position = vec4(aPosition, 1.0);

  vColor = vec4(uColor, 1.0f);
}
