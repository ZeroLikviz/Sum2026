#version 460

layout(location = 0) out vec4 OutColor;

in vec4 DrawColor;
in vec3 DrawNormal;
in vec3 DrawPos;
in vec3 DrawPosOrg;
in vec2 DrawTexCoord;

uniform vec3 CamLoc;
uniform float Time, GlobalTime;
uniform vec3, Ka, Kd, Ks;
uniform float Ph;
uniform int AddonI4;
uniform float FrameW, FrameH;

layout(binding = 0) uniform sampler2D tex;

void main( void )
{
  vec3 Color = texture(tex, DrawTexCoord).rgb;
  if (!bool(AddonI4) && length(Color.r) < 0.01)
    discard;

  vec3 Normal = normalize(DrawNormal);
  vec3 Direction = normalize(DrawPos - CamLoc);
  Direction.x /=  FrameH / FrameW;
  Direction = normalize(Direction);
  float pdist = distance(CamLoc, DrawPos);
  float Angle = 30.0 / 360.0;
  float Factor = 1.0 - (dot(vec3(0, 0, 1), Direction) + 1.0) / 2.0;
  if (Factor < Angle)
    Factor = 1.0;
  else
    Factor = mix(1.0, 0.0, (Factor - Angle) * 16.0);
  
  Factor = clamp(min(1.5, Factor / pdist), 0.0, 1.5);
  Factor = mix(Factor, Factor + 0.3, Factor);
  Factor *= 2.5;
  OutColor = mix(vec4(Color * Factor, 1), vec4(vec3(0.8, 0.8, 0.9) * Factor, 1.0), (dot(vec3(0, 1, 0), Normal) + 1.0) / 4.0) / 2.0;
  OutColor.a *= 2.0;
  OutColor = mix(OutColor, vec4(0, 0, 0, 1), clamp(pdist / 30.0, 0.0, 1.0));
}