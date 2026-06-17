#version 460

layout(location = 0) out vec4 OutColor;

in vec2 DrawTexCoord;

layout(binding = 1) uniform sampler2D tex;

void main( void )
{
  OutColor = texture(tex, DrawTexCoord);
}