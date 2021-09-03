#version 330
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexture;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec2 vTextureCoord;

void main()
{
  vTextureCoord = aTexture;

  gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}
