/* FILE NAME: main.c
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: 3D Animation Project.
 */
#include <windows.h>
#include <math.h>
// #include <glew.h>
// #include <gl/gl.h>

#include "anim/units/units.h"

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
  wc.hCursor = LoadCursor(NULL, IDC_NO);
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
    SetTimer(hWnd, 7969, 16, NULL);
    TM5_AnimInit(hWnd);
    
    /*
    TM5_AnimUnitAdd(TM5_UnitCreateSphere());
    TM5_AnimUnitAdd(TM5_UnitCreateCylinder());
    TM5_AnimUnitAdd(TM5_UnitCreateTorus());

    
    TM5_AnimUnitAdd(TM5_UnitCreateObj("bin/models/person.obj", VecSet3(0, 0, -4)));
    TM5_AnimUnitAdd(TM5_UnitCreateObj("bin/models/rose+vase.obj", VecSet3(-4, 0, 0)));
    TM5_AnimUnitAdd(TM5_UnitCreateObj("bin/models/spider.obj", VecSet3(4, 0, 0)));
    TM5_AnimUnitAdd(TM5_UnitCreateObj("bin/models/furry.obj", VecSet3(0, 4, 0)));
    TM5_AnimUnitAdd(TM5_UnitCreateObj("bin/models/city.obj", VecSet3(0, 0, 0)));
    */
    TM5_AnimUnitAdd(TM5_UnitCreateFurry());
    TM5_AnimUnitAdd(TM5_UnitCreateFPS());
    
    return 0;
  case WM_SIZE:
    TM5_AnimResize(LOWORD(lParam), HIWORD(lParam));
    return 0;
  case WM_DESTROY:
    TM5_AnimClose();
    PostMessage(NULL, WM_QUIT, 0, 0);
    return 0;
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;

  case WM_ERASEBKGND:
    return 1;
  case WM_TIMER:
    TM5_AnimRender();
    TM5_AnimCopyFrame();
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    return 0;
  
  case WM_LBUTTONDOWN:
    SetCapture(hWnd);
    return 0;
  case WM_LBUTTONUP:
    ReleaseCapture();
    return 0;

  case WM_ACTIVATE:
    TM5_Animation.IsActive = LOWORD(wParam) != WA_INACTIVE;
    return 0;
  case WM_ENTERSIZEMOVE:
    TM5_Animation.IsActive = FALSE;
    return 0;
  case WM_EXITSIZEMOVE:
    TM5_Animation.IsActive = TRUE;
    return 0;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

/* End of 'main.c' file */