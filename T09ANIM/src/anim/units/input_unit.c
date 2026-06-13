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

#define TM5_GET_JOYSTIC_AXIS(A) 2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ## min) - 1

static VEC
    CamPos = {0, 0,  4},
    CamDir = {0, 0, -1},
    VecUp  = {0, 1,  0};
static DBL CamSpeed = 0.04;
static DBL MSensitivity = 0.1;
static DBL JSensitivity = 0.1;
static DBL CamAngleX = 0;
static BOOL PlaneMovement = FALSE;
static DBL KSpeedFactor = 1.0;
static DBL JSpeedFactor = 1.0;

/***  ////////////////
 ***  Update Functions
 ***/ ////////////////

/*  UpdateKeyboard updates keyboard state values.
 *  Parameters:
 *     tm5ANIM Anim - Animation context with keyboard values.
 *  Returns: Nothing.
 */
static VOID UpdateKeyboard( tm5ANIM *Anim )
{
  INT i;

  memcpy(Anim->KeysOld, Anim->Keys, 256);
  GetKeyboardState(Anim->Keys);
  for (i = 0; i < 256; i++)
  {
    Anim->Keys[i] >>= 7;
    Anim->KeysClick[i] = Anim->Keys[i] && !Anim->KeysOld[i];
    Anim->KeysRelease[i] = !Anim->Keys[i] && Anim->KeysOld[i];
  }
} /* End of 'UpdateKeyboard' function */


/*  UpdateMouse updates mouse values.
 *  Parameters:
 *     tm5ANIM Anim - Animation context with mouse values.
 *  Returns: Nothing.
 */
static VOID UpdateMouse( tm5ANIM *Anim )
{
  POINT MousePos;

  GetCursorPos(&MousePos);

  if (!Anim->Mouse.Capture)
  {
    Anim->Mouse.dX = MousePos.x - Anim->Mouse.X;
    Anim->Mouse.dY = MousePos.y - Anim->Mouse.Y;

    Anim->Mouse.oldX = Anim->Mouse.X;
    Anim->Mouse.oldY = Anim->Mouse.Y;

    Anim->Mouse.X = MousePos.x;
    Anim->Mouse.Y = MousePos.y;
  }
  else
  {
    Anim->Mouse.dX = MousePos.x - Anim->Mouse.oldX;
    Anim->Mouse.dY = MousePos.y - Anim->Mouse.oldY;

    SetCursorPos(Anim->Mouse.oldX, Anim->Mouse.oldY);
  }

  if (!Anim->Mouse.Visible)
    SetCursor(CreateCursor(NULL, 0, 0, 0, 0, NULL, NULL));
} /* End of 'UpdateMouse' function */


/*  UpdateJoystick updates joystick state values.
 *  Parameters:
 *     tm5ANIM Anim - Animation context with joystick values.
 *  Returns: Nothing.
 */
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
          Anim->JButRelease[i] = !Anim->JBut[i] && Anim->JButOld[i];
          Anim->JButOld[i] = Anim->JBut[i];
        }
        /* Axes */
        Anim->JX = TM5_GET_JOYSTIC_AXIS(X);
        Anim->JY = TM5_GET_JOYSTIC_AXIS(Y);
        Anim->JZ = TM5_GET_JOYSTIC_AXIS(Z);
        Anim->JR = TM5_GET_JOYSTIC_AXIS(R);
        Anim->JPov = ji.dwPOV == 0xFFFF ? -1 : ji.dwPOV / 4500;
      }
    }
  }
} /* End of 'UpdateJoystick' function */


/***  /////////////////
 ***  Process Functions
 ***/ /////////////////

static VOID ProcessKeyboard( tm5ANIM *Anim )
{
  VEC NewDir = CamDir;

  /* General Keys */
  if (Anim->KeysClick[VK_ESCAPE])
    TM5_AnimExit();
  if (Anim->KeysClick[' '])
    Anim->IsPause = !Anim->IsPause;
  if (Anim->KeysClick[VK_F11])
    TM5_AnimFlipFullScreen();

  /* Movement Keys */
  if (Anim->KeysClick[' '])
    PlaneMovement = !PlaneMovement;

  if (PlaneMovement)
  {
    NewDir.Y = 0;
    NewDir = VecNormalize(NewDir);
  }
  if (Anim->Keys['W'])
    CamPos = VecAddVec(CamPos, VecScale(NewDir,  CamSpeed * KSpeedFactor));
  if (Anim->Keys['A'])
    CamPos = VecAddVec(CamPos, VecScale(VecNormalize(VecCross(NewDir, VecUp)), -CamSpeed * KSpeedFactor));
  if (Anim->Keys['S'])
    CamPos = VecAddVec(CamPos, VecScale(NewDir, -CamSpeed * KSpeedFactor));
  if (Anim->Keys['D'])
    CamPos = VecAddVec(CamPos, VecScale(VecNormalize(VecCross(NewDir, VecUp)),  CamSpeed * KSpeedFactor));

  if (Anim->KeysClick['1'])
    CamSpeed = 0.04;
  if (Anim->KeysClick['2'])
    CamSpeed = 0.16;
  if (Anim->KeysClick['3'])
    CamSpeed = 0.64;
  if (Anim->KeysClick['4'])
    CamSpeed = 2.56;
  if (Anim->KeysClick['5'])
    CamSpeed = 16;
  if (Anim->KeysClick['6'])
    CamSpeed = 128;

  if (Anim->KeysClick[VK_SHIFT])
    KSpeedFactor = 0.1;
  if (Anim->KeysRelease[VK_SHIFT])
    KSpeedFactor = 1.0;

  TM5_RndCamSet(CamPos, VecAddVec(CamPos, CamDir), VecUp);
} /* End of 'ProcessKeyboard' function */


static VOID ProcessMouse( tm5ANIM *Anim )
{
  if (Anim->KeysClick[VK_RBUTTON])
  {
    Anim->Mouse.Capture = TRUE;
    Anim->Mouse.Visible = FALSE;
  }
  if (Anim->KeysRelease[VK_RBUTTON])
  {
    Anim->Mouse.Capture = FALSE;
    Anim->Mouse.Visible = TRUE;
  }

  if (Anim->Keys[VK_RBUTTON])
  {
    VEC Right;
    CamDir = VecMulMatr(CamDir, MatrRotateY(-Anim->Mouse.dX * MSensitivity));
    Right = VecCross(CamDir, VecUp);
    CamAngleX += -Anim->Mouse.dY * MSensitivity;
    if (CamAngleX > -89 && CamAngleX < 89)
      CamDir = VecRotateVec(Right, CamDir, -Anim->Mouse.dY * MSensitivity);
    else
      CamAngleX = 89 * Sign(CamAngleX);
  }
} /* End of 'ProcessMouse' function */


static VOID ProcessJoystick( tm5ANIM *Anim )
{
  static BOOL SlowDown = FALSE;
  VEC Right = VecCross(CamDir, VecUp);
  VEC NewDir = CamDir;

  if (!Anim->JBut[4])
  {
    if (Anim->JButClick[0])
      CamSpeed = 0.04;
    if (Anim->JButClick[1])
      CamSpeed = 0.16;
    if (Anim->JButClick[2])
      CamSpeed = 0.64;
    if (Anim->JButClick[3])
      CamSpeed = 2.56;
  }

  if (Anim->JButClick[6]) /* L2 */
    PlaneMovement = !PlaneMovement;
  if (Anim->JButClick[5]) /* R1 */
    SlowDown = !SlowDown;

  if (Anim->JBut[4])
  {
    if (Anim->JButClick[0])
      JSensitivity = 0.1;
    if (Anim->JButClick[1])
      JSensitivity = 0.2;
    if (Anim->JButClick[2])
      JSensitivity = 0.3;
    if (Anim->JButClick[3])
      JSensitivity = 0.5;
  }

  if (SlowDown)
    JSpeedFactor = 0.1;
  else
    JSpeedFactor = 1.0;

  if (PlaneMovement)
  {
    NewDir.Y = 0;
    NewDir = VecNormalize(NewDir);
  }

  CamPos = VecAddVec(CamPos, VecScale(NewDir, -CamSpeed * JSpeedFactor * Anim->JY));
  CamPos = VecAddVec(CamPos, VecScale(VecCross(NewDir, VecUp), CamSpeed * JSpeedFactor * Anim->JX));
  
  CamDir = VecMulMatr(CamDir, MatrRotateY(-Anim->JR * 10 * JSensitivity));
  CamDir = VecRotateVec(Right, CamDir, -Anim->JZ * 10 * JSensitivity);
  TM5_RndCamSet(CamPos, VecAddVec(CamPos, CamDir), VecUp);
} /* End of 'ProcessJoystick' function */


/***  /////////////////
 ***  Unit Functions
 ***/ /////////////////

/*  Response processes all input.
 *  Parameters:
 *     tm5ANIM Anim - Animation context with joystick values.
 *  Returns: Nothing.
 */
static VOID Response( tm5UNIT *Unit, tm5ANIM *Anim )
{
  if (!Anim->IsActive || !Anim->ProcessInput)
    return;

  UpdateKeyboard(Anim);
  UpdateMouse(Anim);
  UpdateJoystick(Anim);

  if (Anim->ProcessKeyboard)
    ProcessKeyboard(Anim);
  if (Anim->ProcessMouse)
    ProcessMouse(Anim);
  if (Anim->ProcessJoystick)
    ProcessJoystick(Anim);
}

tm5UNIT* TM5_UnitCreateControl( VOID )
{
  tm5UNIT *NewUnit = TM5_AnimCreateUnit(sizeof(tm5UNIT));

  NewUnit->Response = Response;

  return (VOID *)NewUnit;
}

/* End of 'control_unit.c' file */