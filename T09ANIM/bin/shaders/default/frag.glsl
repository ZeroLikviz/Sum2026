#version 330

layout(location = 0) out vec4 OutColor;

in vec4 DrawColor;
in vec3 DrawNormal;
in vec3 DrawPos;
in vec3 DrawPosOrg;

uniform vec3 CamLoc;
uniform float Time, GlobalTime;
uniform vec3, Ka, Kd, Ks;
uniform float Ph;

void main( void )
{
  vec3 LightPos = vec3(10 * sin(Time), 40, 10 * cos(Time));
  vec3 Normal = normalize(DrawNormal);
  vec3 Color = DrawColor.rgb;
  vec3 Direction = normalize(DrawPos - CamLoc);
  vec3 Reflected = reflect(Direction, Normal);
  vec3 ToLight = normalize(LightPos - DrawPos);
  float dist = distance(LightPos, DrawPos);
  
  Color += Ka;
  Color += Kd * max(0, (dot(Normal, ToLight) + 1.0) / 2.0);
  Color += Ks * max(0, pow((dot(Reflected, ToLight) + 1.0) / 2.0, Ph + dist));

  OutColor = vec4(Color, 1);
}