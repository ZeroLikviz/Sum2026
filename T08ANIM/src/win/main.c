/* FILE NAME: main.c
 * PROGRAMMER: TM5
 * DATE: 09.06.2026
 * PURPOSE: 3D Animation Project.
 */
#include <windows.h>
#include <math.h>
#include "anim/rnd/rnd.h"
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
  static DBL a = 0;
  PAINTSTRUCT ps;
  HDC hDC;

  switch (Msg)
  {
  case WM_CREATE:
    TM5_RndInit(hWnd);
    SetTimer(hWnd, 7969, 16, NULL);
    return 0;
  case WM_SIZE:
    TM5_RndResize(LOWORD(lParam), HIWORD(lParam));
    return 0;  
  case WM_DESTROY:
    TM5_RndClose();
    PostMessage(NULL, WM_QUIT, 0, 0);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_TIMER:
    TM5_RndStart();
    TM5_RndEnd();
    
    {
      tm5PRIM Primitive;
      TM5_RndPrimCreate(&Primitive, 4, 6);
      Primitive.Vertices[0].Vec = VecSet3(0, 0, 0);
      Primitive.Vertices[1].Vec = VecSet3(0, 1, 0);
      Primitive.Vertices[2].Vec = VecSet3(1, 1, 0);
      Primitive.Vertices[3].Vec = VecSet3(1, 0, 0);

      Primitive.Indexes[0] = 0;
      Primitive.Indexes[1] = 3;
      Primitive.Indexes[2] = 1;
      Primitive.Indexes[3] = 1;
      Primitive.Indexes[4] = 2;
      Primitive.Indexes[5] = 3;
      TM5_RndPrimDraw(&Primitive, MatrIdentity());
      TM5_RndPrimFree(&Primitive);
    }

    hDC = GetDC(hWnd);
    TM5_RndCopyFrame(hDC);
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