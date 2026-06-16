#version 330

layout(location = 0) out vec4 OutColor;

uniform float Time;

in vec4 DrawColor;
in vec3 DrawNormal;

void main( void )
{
  vec3 LightDir = vec3(1, -1, 1);
  float Strength = dot(LightDir, DrawNormal) * 2 - 1;
  
  if (Strength < 0.5)
    Strength = 0.5;

  OutColor = DrawColor * Strength;
}