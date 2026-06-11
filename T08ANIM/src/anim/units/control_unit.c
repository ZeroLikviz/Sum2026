/* FILE NAME: control_unit.c
 * PROGRAMMER: TM5
 * DATE: 10.06.2026
 * PURPOSE: Implement animation system.
 */

#include <math.h>
#include <stdio.h>

#include "anim/units/units.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm")

#define VG4_GET_JOYSTIC_AXIS(A) 2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ## min) - 1

static VEC
    CamPos = {0, 0,  4},
    CamDir = {0, 0, -1},
    VecUp  = {0, 1,  0};
static DBL CamSpeed = 0.1;
static DBL Sensitivity = 0.1;

static VOID UpdateKeyboard( tm5ANIM *Anim )
{
  INT i;

  memcpy(Anim->KeysOld, Anim->Keys, 256);
  GetKeyboardState(Anim->Keys);
  for (i = 0; i < 256; i++)
  {
    Anim->Keys[i] >>= 7;
    Anim->KeysClick[i] = Anim->Keys[i] && !Anim->KeysOld[i];
  }
}

static VOID UpdateMouse( tm5ANIM *Anim )
{
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(Anim->hWnd, &pt);

  Anim->Mdx = pt.x - Anim->Mx;
  Anim->Mdy = pt.y - Anim->My;

  Anim->Mx = pt.x;
  Anim->My = pt.y;
}

static VOID UpdateJoystick( tm5ANIM *Anim )
{
  INT i;

  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;


      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Buttons */
        for (i = 0; i < 32; i++)
        {
          Anim->JBut[i] = (ji.dwButtons >> i) & 1;
          Anim->JButClick[i] = Anim->JBut[i] && !Anim->JButOld[i];
          Anim->JButOld[i] = Anim->JBut[i];
        }
        /* Axes */
        Anim->JX = VG4_GET_JOYSTIC_AXIS(X);
        Anim->JY = VG4_GET_JOYSTIC_AXIS(Y);
        Anim->JZ = VG4_GET_JOYSTIC_AXIS(Z);
        Anim->JR = VG4_GET_JOYSTIC_AXIS(R);
        Anim->JPov = ji.dwPOV == 0xFFFF ? -1 : ji.dwPOV / 4500;
      }
    }
  }
}

static VOID ProcessKeyboard( tm5ANIM *Anim )
{
  /* General Keys */
  if (Anim->KeysClick[VK_ESCAPE])
    TM5_AnimExit();
  if (Anim->KeysClick[' '])
    Anim->IsPause = !Anim->IsPause;
  if (Anim->KeysClick[VK_F11])
    TM5_AnimFlipFullScreen();

  /* Movement Keys */
  if (Anim->Keys['W'])
    CamPos = VecAddVec(CamPos, VecScale(CamDir,  CamSpeed));
  if (Anim->Keys['S'])
    CamPos = VecAddVec(CamPos, VecScale(CamDir, -CamSpeed));
  if (Anim->Keys['D'])
    CamPos = VecAddVec(CamPos, VecScale(VecCross(CamDir, VecUp),  CamSpeed));
  if (Anim->Keys['A'])
    CamPos = VecAddVec(CamPos, VecScale(VecCross(CamDir, VecUp), -CamSpeed));
  if (Anim->Keys[VK_SHIFT])
    CamSpeed = 0.01;
  else
    CamSpeed = 0.1;

  TM5_RndCamSet(CamPos, VecAddVec(CamPos, CamDir), VecUp);
}

static VOID ProcessMouse( tm5ANIM *Anim )
{
  static POINT MousePos;

  if (Anim->KeysClick[VK_RBUTTON])
    GetCursorPos(&MousePos);
  if (Anim->Keys[VK_RBUTTON])
  {
    static POINT NullMousePos;
    VEC Right;

    SetCursor(CreateCursor(NULL, 0, 0, 0, 0, NULL, NULL));
    GetCursorPos(&NullMousePos);
    CamDir = VecMulMatr(CamDir, MatrRotateY(-(NullMousePos.x - MousePos.x) * Sensitivity));
    Right = VecCross(CamDir, VecUp);
    CamDir = VecRotateVec(Right, CamDir, -(NullMousePos.y - MousePos.y) * Sensitivity);
    SetCursorPos(MousePos.x, MousePos.y);
  }
  if (!Anim->Keys[VK_RBUTTON] && Anim->KeysOld[VK_RBUTTON])
    SetCursorPos(MousePos.x, MousePos.y);
}

static VOID ProcessJoystick( tm5ANIM *Anim )
{
  VEC Right = VecCross(CamDir, VecUp);

  CamPos = VecAddVec(CamPos, VecScale(CamDir,  -CamSpeed * Anim->JY));
  CamPos = VecAddVec(CamPos, VecScale(VecCross(CamDir, VecUp), CamSpeed * Anim->JX));
  
  CamDir = VecMulMatr(CamDir, MatrRotateY(-Anim->JR * 20 * Sensitivity));
  CamDir = VecRotateVec(Right, CamDir, -Anim->JZ * 20 * Sensitivity);
  TM5_RndCamSet(CamPos, VecAddVec(CamPos, CamDir), VecUp);
}

static VOID Response( tm5UNIT *Unit, tm5ANIM *Anim )
{
  if (!Anim->IsActive)
    return;

  UpdateKeyboard(Anim);
  UpdateMouse(Anim);
  UpdateJoystick(Anim);
  if (Anim->ProcessKeyboard)
  {
    ProcessKeyboard(Anim);
    ProcessMouse(Anim);
    ProcessJoystick(Anim);
  }
}

tm5UNIT* TM5_UnitCreateControl( VOID )
{
  tm5UNIT *NewUnit = TM5_AnimCreateUnit(sizeof(tm5UNIT));

  NewUnit->Response = Response;

  return (VOID *)NewUnit;
}

/* End of 'control_unit.c' file */