/* FILE NAME: t02arrow.c
 * PROGRAMMER: TM5
 * DATE: 02.06.2026
 * PURPOSE: ---
 */
#include <windows.h>
#include <math.h>
#define WND_CLASS_NAME "CSID:002959"
#define RAND_NUM 7626
#define MAXIMIZE_BUTTON_ID 6110
#define LAYOUT_BUTTON_ID 9880
#define ARROW_SIZE 40
#define ARROW_MARGIN 50
#define TIMER_MS 14
#define RAND_BATCH 256
#define PTS_SIZE 8
#define HALF_PI 1.5708
struct TagARROW
{
  POINT pts[PTS_SIZE];
};
typedef struct TagARROW ARROW;

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

  CreateWindow("BUTTON", "maximise", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 10, 10, 120, 40, hWnd, (HMENU)MAXIMIZE_BUTTON_ID, hInstance, NULL);
  CreateWindow("BUTTON", "layout", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 10, 60, 120, 40, hWnd, (HMENU)LAYOUT_BUTTON_ID, hInstance, NULL);
  
  // Main Loop //
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}

INT Sqr( INT x )
{
  return x * x;
}

VOID SetColor( HDC hDC, INT r, INT g, INT b )
{
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(r, g, b));
}

VOID RotateArrow( ARROW *Arrow, DOUBLE Rads )
{
  INT i, N = sizeof(Arrow->pts) / sizeof(POINT);
  DOUBLE X, Y;
  for (i = 0; i < N; i++)
  {
    X = Arrow->pts[i].x;
    Y = Arrow->pts[i].y;
    Arrow->pts[i].x = (LONG)(X * cos(Rads) + Y * sin(Rads));
    Arrow->pts[i].y = (LONG)(X * sin(Rads) - Y * cos(Rads));
  }
}

VOID ResizeArrow( ARROW *Arrow, DOUBLE M )
{
  INT i, N = sizeof(Arrow->pts) / sizeof(POINT);
  for (i = 0; i < N; i++)
  {
    Arrow->pts[i].x = (LONG)(Arrow->pts[i].x * M);
    Arrow->pts[i].y = (LONG)(Arrow->pts[i].y * M);
  }
}

VOID InitPoints( ARROW *Arrow )
{
  static POINT Points[PTS_SIZE] = {{0, 12}, {-5, 6}, {-1, 6}, {-4, -8}, {0, -6}, {4, -8}, {1, 6}, {5, 6}};
  INT i, N = sizeof(Arrow->pts) / sizeof(POINT);

  for (i = 0; i < N; i++)
    Arrow->pts[i] = Points[i];
}

ARROW CreateArrow( INT X, INT Y, DOUBLE M, DOUBLE Rads )
{
  ARROW Arrow;
  INT i, N = sizeof(Arrow.pts) / sizeof(POINT);

  InitPoints(&Arrow);
  ResizeArrow(&Arrow, M);
  RotateArrow(&Arrow, Rads);
  for (i = 0; i < N; i++)
  {
    Arrow.pts[i].x += X;
    Arrow.pts[i].y += Y;
  }

  return Arrow;
}

INT Sign ( INT X )
{
  return (X > 0) * 2 - 1;
}

VOID DrawArrow ( HDC hDC, INT X, INT Y, INT MX, INT MY )
{
  ARROW Arrow;
  DOUBLE Hyp, ReHyp, Rads;

  ReHyp = Hyp = sqrt(Sqr(X - MX) + Sqr(Y - MY));
  Hyp = pow(Hyp, 1.2 + 8 / Hyp);
  
  Rads = Sign(MX - X) * (asin((MY - Y) / ReHyp)  + HALF_PI);
  Arrow = CreateArrow(X, Y, sqrt(0.01 * Hyp), Rads);

  Polygon(hDC, Arrow.pts, sizeof(Arrow.pts) / sizeof(POINT));
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  static DOUBLE Rad = 0;
  static INT W, H;
  static BOOL Random = FALSE;
  static HDC hMemDC, hClockDC;
  static HBITMAP hBM;
  INT i, j;
  POINT Point;
  PAINTSTRUCT ps;
  HDC hDC;

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    SetTimer(hWnd, RAND_NUM, TIMER_MS, NULL);
    return 0;
  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);

    if (hBM != NULL)
      DeleteObject(hBM);
    hDC = GetDC(hWnd);
    hBM = CreateCompatibleBitmap(hDC, W, H);
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
    SetColor(hMemDC, abs((INT)(sin(Rad) * 128)) + 60, abs((INT)(sin(Rad) * 128)) + 60, abs((INT)(sin(Rad) * 128)) + 60);
    Rectangle(hMemDC, 0, 0, W, H);

    GetCursorPos(&Point);      
    ScreenToClient(hWnd, &Point);
    
    Rad += 0.01;
    SetColor(hMemDC, abs((INT)(sin(Rad + 1) * 255)), abs((INT)(cos(Rad) * 255)), abs((INT)(sin(Rad - 1) * 255)));
    if (Random)
      for (i = 0, srand(RAND_NUM * MAXIMIZE_BUTTON_ID); i < RAND_BATCH; i++)
        DrawArrow(hMemDC, rand() % W, rand() % H, Point.x, Point.y);
    else
      for (i = 0; i <= W / (ARROW_SIZE + ARROW_MARGIN); i++)
        for (j = 0; j <= H / (ARROW_SIZE + ARROW_MARGIN); j++)
          DrawArrow(hMemDC, i * (ARROW_SIZE + ARROW_MARGIN), j * (ARROW_SIZE + ARROW_MARGIN), Point.x, Point.y);
    
    
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_COMMAND:
    switch (LOWORD(wParam))
    {
    case MAXIMIZE_BUTTON_ID:
      ShowWindow(hWnd, SW_MAXIMIZE);
      break;
    case LAYOUT_BUTTON_ID:
      Random = !Random;
      break;
    }
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}