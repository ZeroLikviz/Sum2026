/**/
#version 460

layout(location = 0) out vec4 OutColor;

in vec4 DrawColor;
in vec2 DrawTexCoord;

uniform vec3 CamLoc;
uniform float Time, GlobalTime;

uniform int AddonI0, AddonI1;

uniform bool IsTexture0;
layout(binding = 0) uniform sampler2D Tex;

void main( void )
{
  if (!IsTexture0)
    discard;

  vec4 tc = texture(Tex, DrawTexCoord);
  if (tc.a < 0.030)
    discard;

  OutColor = vec4(tc.rgb, tc.a);
  //OutColor = vec4(1, 1, 1, 1);
}
