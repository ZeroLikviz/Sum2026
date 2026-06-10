/* FILE NAME: main.c
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: 3D Animation Project.
 */
#include <windows.h>
#include <math.h>
#include "anim/units/units.h"
#include "anim/rnd/rnd.h"
#include "anim/anim.h"
#include "def.h"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  MSG msg;
  HWND hWnd;

  /* Set up memory leaks detector */
  SetDbgMemHooks();

  /* Class Registration */
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_APPSTARTING);
  wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
  wc.hInstance = hInstance;
  wc.lpszMenuName = NULL;
  wc.lpszClassName = "MainWindow";
  wc.lpfnWndProc = MyWindowFunc;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Couldn't register window class", "CSID:002959", MB_OK | MB_ICONASTERISK);
    return -1;
  }

  /* Create Window */
  hWnd = CreateWindow("MainWindow", "SilentSnow", WS_OVERLAPPEDWINDOW | WS_BORDER, 0, 0, 720, 480, NULL, NULL, hInstance, NULL);
  ShowWindow(hWnd, ShowCmd);

  /* Main Loop */
  while (TRUE)
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      DispatchMessage(&msg);
    }
    else
      SendMessage(hWnd, WM_TIMER, 7969, 0);
  
  return 0;
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  static MINMAXINFO *minmax;
  PAINTSTRUCT ps;
  HDC hDC;

  switch (Msg)
  {
  case WM_CREATE:
    TM5_RndInit(hWnd);
    SetTimer(hWnd, 7969, 16, NULL);
    TM5_AnimInit(hWnd);
    TM5_AnimUnitAdd(TM5_UnitCreateFurry());
    TM5_RndCamSet(VecSet3(0, 0, 1), VecSet3(0, 0, 0), VecSet3(0, 1, 0));
    return 0;
  case WM_SIZE:
    TM5_AnimResize(LOWORD(lParam), HIWORD(lParam));
    return 0;  
  case WM_DESTROY:
    TM5_AnimClose();
    PostMessage(NULL, WM_QUIT, 0, 0);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_TIMER:
    TM5_AnimRender();

    hDC = GetDC(hWnd);
    TM5_AnimCopyFrame(hDC);
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_KEYDOWN:
    switch (wParam)
    {
    case VK_ESCAPE:
      PostMessage(hWnd, WM_DESTROY, wParam, lParam);
      break;
    }
    break;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

/* End of 'main.c' file */