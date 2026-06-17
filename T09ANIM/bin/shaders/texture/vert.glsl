#version 460

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;

uniform mat4 MatrWVP;

out vec2 DrawTexCoord;

void main( void )
{
  vec4 NewPos = MatrWVP * vec4(InPosition, 1);
  gl_Position = NewPos;
  DrawTexCoord = InTexCoord;
}