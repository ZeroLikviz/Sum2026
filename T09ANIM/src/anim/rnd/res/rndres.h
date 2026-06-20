/* FILE NAME: rndres.h
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: Implement renderer.
 */
#ifndef __rndres_h_
#define __rndres_h_

#include "def.h"

#define TM5_MAX_SHADERS 32
#define TM5_MAX_TEXTURES 256
#define TM5_MAX_MATERIALS 256
#define TM5_STR_MAX 512

typedef struct tagtm5SHADER
{
  CHAR Name[TM5_STR_MAX];
  INT ProgId;
} tm5SHADER;

typedef struct tagtm5TEXTURE
{
  CHAR Name[TM5_STR_MAX]; /* Texture name */
  INT W, H;               /* Texture size in pixels */
  UINT TexId;             /* OpenGL texture Id */ 
} tm5TEXTURE;

typedef struct tagtm5MATERIAL
{
  CHAR Name[TM5_STR_MAX]; /* Material name */

  /* Illumination coefficients */    
  VEC Ka, Kd, Ks;           /* Ambient, diffuse, specular coefficients */
  FLT Ph;                   /* Phong power coefficient */

  FLT Transparency;                /* Transparency factor */

  INT Textures[8];               /* Texture references from texture table (or -1) */

  INT ShaderNumber;                /* Shader number in shader table */
} tm5MATERIAL;

/* Font description structure */
typedef struct tagtm5FONT
{
  DWORD LineH, BaseH; /* Font line height and base line height in pixels */
  FLT AdvanceX[256];  /* Every letter shift right value (0 if no letter present) */
} tm5FONT;

VOID TM5_RndResInit( VOID );
VOID TM5_RndResClose( VOID );
VOID TM5_RndResBindTexture( INT MtlNum, INT TexNum );

/* Shader Functions */
VOID TM5_RndShdInit( VOID ); 
VOID TM5_RndShdClose( VOID ); 
VOID TM5_RndShdUpdate( VOID );
INT TM5_RndShdAdd( CHAR *ShaderFileNamePrefix );

VOID TM5_RndTexInit( VOID );
VOID TM5_RndTexClose( VOID );
INT TM5_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits );
INT TM5_RndTexAdd( CHAR *Filename );

/* Material Functions */
tm5MATERIAL TM5_RndMtlGetDef( VOID );
tm5MATERIAL TM5_RndMtlGetStock( CHAR *Name );
VOID TM5_RndMtlInit( VOID );
VOID TM5_RndMtlClose( VOID );
INT TM5_RndMtlAdd( tm5MATERIAL *Mtl );
UINT TM5_RndMtlApply( INT MtlNo );
tm5MATERIAL *TM5_RndMtlGet( INT mtlNo );

extern tm5SHADER TM5_RndShaders[TM5_MAX_SHADERS];
extern tm5TEXTURE TM5_RndTextures[TM5_MAX_TEXTURES];
extern tm5MATERIAL TM5_RndMaterials[TM5_MAX_MATERIALS];
extern INT TM5_RndShadersSize;
extern INT TM5_RndMaterialsSize;
extern INT TM5_RndTexturesSize;

BOOL TM5_RndFntLoad( CHAR *FileName );
VOID TM5_RndFntInit( VOID );
VOID TM5_RndFntClose( VOID );
VOID TM5_RndFntDraw( CHAR *Str, VEC Pos, FLT Size );
/* END OF 'rndshd.c' FILE */

#endif

/* End of 'rndres.h' file */