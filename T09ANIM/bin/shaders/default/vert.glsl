#version 330

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;

uniform mat4 MatrWVP;
uniform mat4 MatrInv;

out vec4 DrawColor;
out vec3 DrawNormal;
out vec3 DrawPos;
out vec3 DrawPosOrg;

void main( void )
{
  vec4 NewPos = MatrWVP * vec4(InPosition, 1);
  gl_Position = NewPos;
  DrawColor = InColor;
  DrawNormal = (MatrInv * vec4(InNormal, 1)).xyz;
  DrawPos = NewPos.xyz;
  DrawPosOrg = InPosition;
}