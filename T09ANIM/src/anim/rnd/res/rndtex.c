/* FILE NAME: rndtex.c
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: 3D Animation Project.
 */

#include <stdio.h>
#include "anim/rnd/rnd.h"

tm5TEXTURE TM5_RndTextures[TM5_MAX_TEXTURES]; /* Array of textures */
INT TM5_RndTexturesSize; 

VOID TM5_RndTexInit( VOID )
{
  memset(TM5_RndTextures, 0, sizeof(TM5_RndTextures));
  TM5_RndTexturesSize = 0;
}

VOID TM5_RndTexClose( VOID )
{
}

INT TM5_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits )
{
  INT mips;

  if (TM5_RndTexturesSize >= TM5_MAX_TEXTURES)
    return -1;

  /* Setup OpenGL texture */
  glGenTextures(1, &TM5_RndTextures[TM5_RndTexturesSize].TexId);
  glBindTexture(GL_TEXTURE_2D, TM5_RndTextures[TM5_RndTexturesSize].TexId);


  mips = log(W > H ? W : H) / log(2);
  mips = mips < 1 ? 1 : mips;

  glTexStorage2D(GL_TEXTURE_2D, mips, C == 4 ? GL_RGBA8 : C == 3 ? GL_RGB8 : GL_R8, W, H);

  if (Bits != NULL)
  {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, W, H,
                    C == 4 ? GL_BGRA : C == 3 ? GL_BGR : GL_RED, GL_UNSIGNED_BYTE, Bits);
  }
  /*glGenerateMipmap(GL_TEXTURE_2D); */ /* NO MIPMAPS! >:( */

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  strcpy(TM5_RndTextures[TM5_RndTexturesSize].Name, Name);
  TM5_RndTextures[TM5_RndTexturesSize].W = W;
  TM5_RndTextures[TM5_RndTexturesSize].H = H;

  return TM5_RndTexturesSize++;
}

INT TM5_RndTexAdd( CHAR *Filename )
{
  INT i, ret = -1;
  HBITMAP hBm;
  BITMAP Bm;
  CHAR Buffer[512];

  for (i = 0; i < TM5_RndTexturesSize; i++)
    if (strcmp(Filename, TM5_RndTextures[i].Name) == 0)
      return i;
  
  sprintf(Buffer, "bin/textures/%s", Filename);
  if ((hBm = LoadImage(NULL, Buffer, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
  {
    GetObject(hBm, sizeof(BITMAP), &Bm);
    if (Bm.bmBitsPixel == 24 || Bm.bmBitsPixel == 32 || Bm.bmBitsPixel == 8)
      ret = TM5_RndTexAddImg(Filename, Bm.bmWidth, Bm.bmHeight, Bm.bmBitsPixel >> 3, Bm.bmBits); 
    DeleteObject(hBm);
  }

  return ret;
}

/* End of 'rndtex.c' file */