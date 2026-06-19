/**/
#version 330

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;

uniform mat4 MatrWVP, MatrW, MatrWInv;
uniform float Time;

uniform int AddonI0, AddonI1;

uniform float FrameW, FrameH;

out vec4 DrawColor;
out vec2 DrawTexCoord;

/* Orthographics projection matrix setup function.
 * ARGUMENTS:
 *   - orthographic box side facets coordinates:
 *       float Left, Right, Bottom, Top, Near, Far;
 * RETURNS:
 *   (mat4) result matrix.
 */
mat4 MatrOrtho( float Left, float Right, float Bottom, float Top, float Near, float Far )
{
  return
    mat4(vec4(              2 / (Right - Left),                                0,                            0, 0),
         vec4(                               0,               2 / (Top - Bottom),                            0, 0),
         vec4(                               0,                                0,            -2 / (Far - Near), 0),
         vec4(-(Right + Left) / (Right - Left), -(Top + Bottom) / (Top - Bottom), -(Far + Near) / (Far - Near), 1));
} /* End of 'MatrOrtho' function */

void main( void )
{
  //gl_Position = MatrOrtho(0, FrameW, -FrameH, 0, -100, 100) * MatrW * vec4(InPosition, 1);
  gl_Position = MatrWVP * vec4(InPosition, 1);
  DrawColor = vec4(1, 1, 1, 1);
  DrawTexCoord = InTexCoord;
}
