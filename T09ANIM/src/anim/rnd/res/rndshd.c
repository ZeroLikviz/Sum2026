/* FILE NAME  : rndshd.c
 * PROGRAMMER : TM5
 * LAST UPDATE: 13.06.2026
 * PURPOSE    : 3D animation project.
 *              Rendering resources declaration module.
 */
 
#include <stdio.h>
#include <time.h>
 
#include "anim/rnd/res/rndres.h"
#include "anim/rnd/rnd.h"

tm5SHADER TM5_RndShaders[TM5_MAX_SHADERS];
INT TM5_RndShadersSize;
 
static VOID TM5_RndShdLog( CHAR *FileNamePrefix, CHAR *ShaderName, CHAR *Text )
{
  FILE *F;
 
  if ((F = fopen("bin/shaders/LOG", "a")) == NULL)
    return;
  fprintf(F, "%s : %s\n%s\n\n", FileNamePrefix, ShaderName, Text);
  fclose(F);
}
 
static CHAR * TM5_RndLoadTextFromFile( CHAR *FileName )
{
  FILE *F;
  INT flen;
  CHAR *txt;
 
  if ((F = fopen(FileName, "rb")) == NULL)
    return NULL;
 
  /* Measure file length */
  fseek(F, 0, SEEK_END);
  flen = ftell(F);
 
  /* Allocate memory */
  if ((txt = malloc(flen + 1)) == NULL)
  {
    fclose(F);
    return NULL;
  }
  memset(txt, 0, flen + 1);
 
  /* Load text */
  rewind(F);
  fread(txt, 1, flen, F);
  fclose(F);
  return txt;
}
 
static UINT TM5_RndShdLoad( CHAR *FileNamePrefix )
{
  CHAR *txt;
  struct
  {
    CHAR *Name;  /* Shader name (e.g. "VERT") */
    INT Type;    /* Shader type (e.g. GL_VERTEX_SHADER) */
    INT Id;      /* Created shader Id */
  } shd[] =
  {
    {"VERT", GL_VERTEX_SHADER, 0},
    {"FRAG", GL_FRAGMENT_SHADER, 0},
  };
  INT i, res, NoofS = sizeof(shd) / sizeof(shd[0]);
  UINT prg;
  BOOL is_ok = TRUE;
  static CHAR Buf[10000];
 
  for (i = 0; i < NoofS; i++)
  {
    /* Build shader file name */
    sprintf(Buf, "bin/shaders/%s/%s.glsl", FileNamePrefix, shd[i].Name);
 
    /* Load shader text from file */
    txt = TM5_RndLoadTextFromFile(Buf);
    if (txt == NULL)
    {
      TM5_RndShdLog(FileNamePrefix, shd[i].Name, "Error load file");
      is_ok = FALSE;
      break;
    }
    /* Create shader */
    shd[i].Id = glCreateShader(shd[i].Type);
    if (shd[i].Id == 0)
    {
      TM5_RndShdLog(FileNamePrefix, shd[i].Name, "Error create shader");
      is_ok = FALSE;
      break;
    }
 
    /* Send shader source text to OpenGL */
    glShaderSource(shd[i].Id, 1, &txt, NULL);
    free(txt);
 
    /* Compile shader */
    glCompileShader(shd[i].Id);
 
    /* Errors handle */
    glGetShaderiv(shd[i].Id, GL_COMPILE_STATUS, &res);
    if (res != 1)
    {
      glGetShaderInfoLog(shd[i].Id, sizeof(Buf), &res, Buf);
      TM5_RndShdLog(FileNamePrefix, shd[i].Name, Buf);
      is_ok = FALSE;
      break;
    }
  }
 
  /* Create program */
  if (is_ok)
    if ((prg = glCreateProgram()) == 0)
    {
      TM5_RndShdLog(FileNamePrefix, "PROG", "Error create program");
      is_ok = FALSE;
    }
    else
    {
      /* Attach shader programs */
      for (i = 0; i < NoofS; i++)
        if (shd[i].Id != 0)
          glAttachShader(prg, shd[i].Id);
      /* Link program */
      glLinkProgram(prg);
      /* Errors handle */
      glGetProgramiv(prg, GL_LINK_STATUS, &res);
      if (res != 1)
      {
        glGetProgramInfoLog(shd[i].Id, sizeof(Buf), &res, Buf);
        TM5_RndShdLog(FileNamePrefix, "PROG", Buf);
        is_ok = FALSE;
      }
    }
 
 
  /* Error handle */
  if (!is_ok)
  {
    /* Delete all shaders */
    for (i = 0; i < NoofS; i++)
      if (shd[i].Id != 0)
      {
        if (prg != 0)
          glDetachShader(prg, shd[i].Id);
        glDeleteShader(shd[i].Id);
      }
    /* Delete program */
    if (prg != 0)
      glDeleteProgram(prg);
    prg = 0;
  }
  return prg;
} /* End of 'TM5_RndShdLoad' function */
 
static VOID TM5_RndShdFree( UINT ProgId )
{
  INT shds[5], n, i;
 
  if (ProgId == 0 || !glIsProgram(ProgId))
    return;
 
  glGetAttachedShaders(ProgId, 5, &n, shds);
  for (i = 0; i < n; i++)
    if (glIsShader(shds[i]))
    {
      glDetachShader(ProgId, shds[i]);
      glDeleteShader(shds[i]);
    }
  glDeleteProgram(ProgId);
} /* End of 'TM5_RndShdFree' function */
 
 
VOID TM5_RndShdInit( VOID )
{
  INT TM5_RndShadersSize = 0;
  TM5_RndShdAdd("default");
} /* End of 'TM5_RndResInit' function */
 
VOID TM5_RndShdClose( VOID )
{
  INT i;

  for (i = 0; i < TM5_RndShadersSize; i++)
    TM5_RndShdFree(TM5_RndShaders[i].ProgId);
  TM5_RndShadersSize = 0;
} /* End of 'TM5_RndResInit' function */
 
VOID TM5_RndShdUpdate( VOID )
{
  INT i;

  for (i = 0; i < TM5_RndShadersSize; i++)
  {
    TM5_RndShdFree(TM5_RndShaders[i].ProgId);
    TM5_RndShaders[i].ProgId = TM5_RndShdLoad(TM5_RndShaders[i].Name);
  }
} /* End of 'TM5_RndShdUpdate' function */

INT TM5_RndShdAdd( CHAR *ShaderFileNamePrefix )
{
  INT i;

  for (i = 0; i < TM5_RndShadersSize; i++)
    if (strcmp(ShaderFileNamePrefix, TM5_RndShaders[i].Name) == 0)
      return i;
  if (TM5_RndShadersSize >= TM5_MAX_SHADERS)
    return 0;
  strncpy(TM5_RndShaders[TM5_RndShadersSize].Name, ShaderFileNamePrefix, TM5_STR_MAX - 1);
  TM5_RndShaders[TM5_RndShadersSize].ProgId = TM5_RndShdLoad(ShaderFileNamePrefix);
  return TM5_RndShadersSize++;
} /* End of 'TM5_RndShdAdd' function */
 
/* END OF 'rndshd.c' FILE */