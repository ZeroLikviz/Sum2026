/* FILE NAME: t07globe.c
 * PROGRAMMER: TM5
 * DATE: 06.06.2026
 * PURPOSE: Trying 3D.
 */
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "globe.h"
#include "timer.h"
#define WND_CLASS_NAME "CSID:002959"
#define TIMER_NUM 7626
#define TIMER_MS 16


LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  MSG msg;
  HWND hWnd;

  // Class Registration //
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_APPSTARTING);
  wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
  wc.hInstance = hInstance;
  wc.lpszMenuName = NULL;
  wc.lpszClassName = WND_CLASS_NAME;
  wc.lpfnWndProc = MyWindowFunc;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Couldn't register window class", WND_CLASS_NAME, MB_OK | MB_ICONASTERISK);
    return -1;
  }

  // Create Window //
  hWnd = CreateWindow(WND_CLASS_NAME, "HacHKED!!", WS_OVERLAPPEDWINDOW | WS_BORDER, 0, 0, 720, 480, NULL, NULL, hInstance, NULL);
  ShowWindow(hWnd, ShowCmd);

  // Main Loop //
  while (TRUE)
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      DispatchMessage(&msg);
    }
    else
      SendMessage(hWnd, WM_TIMER, TIMER_NUM, 0);
  return 0;
}

VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;

  if (!IsFullScreen)
  {
    HMONITOR hmon;
    MONITORINFO mi;
    RECT rc;

    /* Save old window size and position */
    GetWindowRect(hWnd, &SaveRect);
    
    /* Obtain nearest monitor */
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    mi.cbSize = sizeof(mi);
    GetMonitorInfo(hmon, &mi);
    
    /* Go to full screen mode */
    rc = mi.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    /* Expand window */
    SetWindowPos(hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left,
      rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  else
    /* Restore from full screen mode */
    SetWindowPos(hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left,
      SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  IsFullScreen = !IsFullScreen;
}

VOID SetColor( HDC hDC, INT r, INT g, INT b )
{
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(r, g, b));
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  static MINMAXINFO *minmax;
  static INT W, H, X, Y;
  static HDC hMemDC;
  static HBITMAP hBM;
  static BITMAP ClockBM;
  CHAR FPSString[128];
  PAINTSTRUCT ps;
  HDC hDC;

  switch (Msg)
  {
  case WM_CREATE:
    /* Initialising DCs */
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    
    /* Initialising Globe*/
    GLB_Init(0.5);

    /* Clean up */
    ReleaseDC(hWnd, hDC);
    SetTimer(hWnd, TIMER_NUM, TIMER_MS, NULL);
    return 0;
  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);

    /* Initialising GLB values */
    GLB_Resize(W, H);
    TimerInit(); 

    /* Initialising bitmap for MemDC */
    if (hBM != NULL)
      DeleteObject(hBM); 
    hDC = GetDC(hWnd);
    hBM = CreateCompatibleBitmap(hDC, W, H);
    
    /* Clean up */
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBM);
    return 0;  
  case WM_DESTROY:
    DeleteObject(hBM);
    ReleaseDC(hWnd, hMemDC);
    PostMessage(NULL, WM_QUIT, 0, 0);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_TIMER:
    /* Clean up */
    SetColor(hMemDC, 255, 255, 255);
    Rectangle(hMemDC, 0, 0, W, H);

    /* Update Timer */
    TimerResponse();

    /* Drawnig Globe */
    GLB_Draw(hMemDC);

    /* Printing FPS */
    TextOut(hMemDC, 0, 0, FPSString, sprintf(FPSString, "FPS: %i", (INT)FPS));

    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_KEYDOWN:
    switch (wParam)
    {
    case VK_F1:
      FlipFullScreen(hWnd);
      break;
    case VK_ESCAPE:
      PostMessage(hWnd, WM_DESTROY, wParam, lParam);
      break;
    case ' ':
      IsPause = !IsPause;
      break;
    }
    break;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

/* END OF 't07globe.c' FILE */