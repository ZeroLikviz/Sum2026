#version 330

layout(location = 0) out vec4 OutColor;

uniform float Time;

in vec4 DrawColor;
in vec3 DrawPos;

void main( void )
{
  if (sin(DrawPos.x) * cos(DrawPos.y + 25 * Time) < cos(Time) - 0.3)
    discard;
  OutColor = DrawColor;
}