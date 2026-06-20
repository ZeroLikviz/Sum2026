/* FILE NAME: tinky_unit.c
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#include <stdio.h>
#include "anim/units/units.h"

/*  Unit */
typedef struct tagtm5UNIT_TINKY tm5UNIT_TINKY;
struct tagtm5UNIT_TINKY
{
  VOID (*Init)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Close)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Response)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Render)( tm5UNIT *Unit, tm5ANIM *Anim );
  
  tm5PRIMS Model;
  VEC Pos;
  VEC ShowPos;
};

/* SHhhh */
typedef struct tagtm5UNIT_HEIGHT_MAP tm5UNIT_HEIGHT_MAP;
struct tagtm5UNIT_HEIGHT_MAP
{
  VOID (*Init)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Close)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Response)( tm5UNIT *Unit, tm5ANIM *Anim );
  VOID (*Render)( tm5UNIT *Unit, tm5ANIM *Anim );
  
  tm5GRID Grid;
  CHAR Filename[512];
  FLT Size;
  tm5PRIM Model;
  VEC Pos;
};

static VOID Init( tm5UNIT *Unit, tm5ANIM *Anim )
{
  INT i;
  tm5UNIT_TINKY *rUnit = (tm5UNIT_TINKY*)(Unit);
  
  rUnit->Pos = VecSet3(0, 0.2, 0);
  TM5_RndPrimsLoad(&rUnit->Model, "bin/models/tinky.g3dm", FALSE);
  for (i = 0; i < rUnit->Model.NumOfPrims; i++)
    TM5_RndMaterials[rUnit->Model.Primitives[i].MtlNumber].ShaderNumber = TM5_RndShdAdd("snow");
}

static VOID Close( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_TINKY *rUnit = (tm5UNIT_TINKY*)(Unit);
  
  TM5_RndPrimsFree(&rUnit->Model);
}

static VOID Render( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_TINKY *rUnit = (tm5UNIT_TINKY*)(Unit);

  TM5_RndPrimsDraw(&rUnit->Model, MatrTranslate(rUnit->ShowPos));
}

static VOID CorrectY( tm5UNIT *Unit, tm5ANIM *Anim )
{
  tm5UNIT_TINKY *rUnit = (tm5UNIT_TINKY*)(Unit);
  tm5UNIT_HEIGHT_MAP *HMap = (tm5UNIT_HEIGHT_MAP*)(Anim->Units[Anim->HeightMapId]);
  INT X = (INT)((rUnit->Pos.X + HMap->Size / 2) / HMap->Size * HMap->Grid.W);
  INT Y = (INT)((rUnit->Pos.Z + HMap->Size / 2) / HMap->Size * HMap->Grid.H);
  X = Clamp(X, 0, HMap->Grid.W - 1);
  Y = Clamp(Y, 0, HMap->Grid.H - 1);

  rUnit->Pos.Y = HMap->Grid.Vertices[X + Y * HMap->Grid.W].Vec.Y;
}

static VOID Response( tm5UNIT *Unit, tm5ANIM *Anim )
{
  static FLT Angle = 0;
  static FLT ShowAngle = 0;
  INT i;
  MATR Matr;
  tm5UNIT_TINKY *rUnit = (tm5UNIT_TINKY*)(Unit);
  VEC Between = VecSubVec(Anim->Camera.Pos, rUnit->Pos);
  FLT Dist = VecLen(Between);

  if (Dist > 0.01)
  {
    FLT Speed = 1.8;
    Angle = atan2(Between.X / Dist, Between.Z / Dist);
    if (fabs(Angle - ShowAngle) > PI)
      ShowAngle += Sign(Angle - ShowAngle) * DPI;
    ShowAngle = Mix(ShowAngle, Angle, Anim->DeltaTime * 4);
    Matr = MatrRotateY(R2D(ShowAngle));

    if (Dist > 6)
      Speed = 8;
    Between.Y = 0;
    Between = VecNormalize(Between);
    rUnit->Pos = VecAddVec(rUnit->Pos, VecScale(Between, Anim->DeltaTime * Speed));
    for (i = 0; i < rUnit->Model.NumOfPrims; i++)
      rUnit->Model.Primitives[i].Transform = Matr;
  }
  CorrectY(Unit, Anim);
  rUnit->ShowPos = VecMix(rUnit->ShowPos, rUnit->Pos, Anim->DeltaTime * 4);
}

tm5UNIT* TM5_UnitCreateTinky( VOID )
{
  tm5UNIT_TINKY *NewUnit = TM5_AnimCreateUnit(sizeof(tm5UNIT_TINKY));
  
  NewUnit->Pos = VecSet3(0, 0, 0);
  NewUnit->ShowPos = VecSet3(0, 0, 0);
  NewUnit->Init = Init;
  NewUnit->Close = Close;
  NewUnit->Render = Render;
  NewUnit->Response = Response;

  return (VOID *)NewUnit;
}

/* End of 'tinky_unit.c' file */
