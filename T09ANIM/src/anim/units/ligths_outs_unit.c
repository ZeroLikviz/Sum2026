/* FILE NAME: lights_out_unit.c
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#include "anim/units/units.h"

/* Lights Out Unit */
typedef struct tagtm5UNIT_GAME tm5UNIT_GAME;
struct tagtm5UNIT_GAME
{
  VOID (*Init)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Close)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Response)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Render)( tm5UNIT *Unit, tm5ANIM *Anim );
  
  BOOL States[9];
  tm5PRIM No;
  tm5PRIM Yes;
};

static VOID Init( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_GAME *rUnit = (tm5UNIT_GAME*)(Unit);
  
  memset(rUnit->States, 0, sizeof(BOOL) * 9);
  TM5_RndPrimCreateSquare(&rUnit->No, 2,  VecSet3(1, 0, 0));
  TM5_RndPrimCreateSquare(&rUnit->Yes, 2, VecSet3(0, 1, 0));
}

static VOID Close( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_GAME *rUnit = (tm5UNIT_GAME*)(Unit);
  
  TM5_RndPrimFree(&(rUnit->Yes));
  TM5_RndPrimFree(&(rUnit->No));
  memset(rUnit, 0, sizeof(tm5UNIT_GAME));
}

static WrapX( INT X )
{
  if (X < 0)
    return 3;
  if (X > 3)
    return 0;
  return X;
}

static WrapY( INT Y )
{
  if (Y < 0)
    return 1;
  if (Y > 1)
    return 0;
  return Y;
}

static GetPos( INT X, INT Y )
{
  if (X < 0 || X > 3)
    return 8;
  X = WrapX(X);
  Y = WrapY(Y);

  return X + Y * 4;
}

static VOID Response( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_GAME *rUnit = (tm5UNIT_GAME*)(Unit);
  DBL X = (DBL)Anim->Mouse.X / Anim->W, Y = (DBL)Anim->Mouse.Y / Anim->H;
  INT tX = (INT)(X * 4), tY = (INT)(Y * 2);

  if (Anim->KeysClick[VK_LBUTTON])
  {
    rUnit->States[GetPos(tX, tY)] = !rUnit->States[GetPos(tX, tY)];
    rUnit->States[GetPos(tX + 1, tY)] = !rUnit->States[GetPos(tX + 1, tY)];
    rUnit->States[GetPos(tX - 1, tY)] = !rUnit->States[GetPos(tX - 1, tY)];
    rUnit->States[GetPos(tX, tY + 1)] = !rUnit->States[GetPos(tX, tY + 1)];
  }
}

static VOID Render( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_GAME *rUnit = (tm5UNIT_GAME*)(Unit);
  INT i, j;

  for (j = 0; j < 2; j++)
    for (i = 0; i < 4; i++)
      if (rUnit->States[GetPos(i, j)])
        TM5_RndPrimDraw(&(rUnit->Yes), MatrTranslate(VecSet3((i * 8 - 16) / 4.0, (-j * 12) / 6.0, 0)));
      else
        TM5_RndPrimDraw(&(rUnit->No), MatrTranslate(VecSet3((i * 8 - 16) / 4.0, (-j * 12) / 6.0, 0)));
}

tm5UNIT* TM5_UnitCreateGame( VOID )
{
  tm5UNIT_GAME *NewUnit = TM5_AnimCreateUnit(sizeof(tm5UNIT_GAME));

  NewUnit->Init = Init;
  NewUnit->Close = Close;
  NewUnit->Response = Response;
  NewUnit->Render = Render;
  NewUnit->Yes.ProgId = TM5_RndShdAdd("default");
  NewUnit->No.ProgId = TM5_RndShdAdd("default");

  return (VOID *)NewUnit;
}

/* End of 'lights_out_unit.c' file */
