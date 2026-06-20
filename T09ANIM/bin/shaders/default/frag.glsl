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
uniform int AddonI1;
uniform int AddonI2;
layout(binding = 0) uniform sampler2D tex;

void main( void )
{
  vec3 LightPos = vec3(10 * sin(Time), 400, 10 * cos(Time));
  vec3 Normal = normalize(DrawNormal);
  vec3 Color = texture(tex, DrawTexCoord).rgb;
  vec3 Direction = normalize(DrawPos - CamLoc);
  vec3 Reflected = reflect(Direction, Normal);
  vec3 ToLight = normalize(LightPos - DrawPosOrg);
  float dist = distance(LightPos, DrawPos);
  float pdist = distance(CamLoc, DrawPos);
  
  Color += Ka;
  Color += Kd * max(0, dot(Normal, ToLight));
  Color += Ks * max(0, pow(dot(Reflected, ToLight), Ph + dist)) * 0.0;

  OutColor = mix(vec4(Color, 1), vec4(0, 0, 0, 1), clamp(pdist / 500.0, 0.0, 1.0));
}