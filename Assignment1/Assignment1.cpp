// Assignment1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Assignment1.h"
#include <string>
#include <list>
#include "bil.h"
#include <cstdlib>
using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:

int iteratorLys = 0;
bool lagBilNord = false;
int tidStart = 0;
list<Car*> l;
list<Car*> l2;
int pw = 10;
int pn = 10;
HWND hWestSpawnRateEdit;
HWND hNorthSpawnRateEdit;

int tall1;
int tall2;

HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    SpawnRateDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ASSIGNMENT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASSIGNMENT1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASSIGNMENT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ASSIGNMENT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   //trafikklys
   SetTimer(hWnd,            // handle to main window 
       1,                    // timer identifier 
       2000,                 // 2-second interval 
       (TIMERPROC)NULL);     // no timer callback
   
   //kjører biler nord-sør
   SetTimer(hWnd,            // handle to main window 
       2,                    // timer identifier 
       50,                 // 0,05-second interval 
       (TIMERPROC)NULL);     // no timer callback

   //kjører biler vest-øst
   SetTimer(hWnd,            // handle to main window 
       3,                    // timer identifier 
       50,                 // 0,05-second interval 
       (TIMERPROC)NULL);     // no timer callback

   //bilgenerator vest
   SetTimer(hWnd,            // handle to main window 
       4,                    // timer identifier 
       1000,                 // 1-second interval 
       (TIMERPROC)NULL);     // no timer callback

   //bilgenerator nord
   SetTimer(hWnd,            // handle to main window 
       5,                    // timer identifier 
      1000,                 // 1-second interval 
       (TIMERPROC)NULL);     // no timer callback

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void TegnSannsynlighet(HDC hdc, double nord, double vest) {

    TCHAR text[256];
    swprintf_s(text, 256, L"Sannsynlighet Nord: %lf", nord);
    TextOut(hdc, 600, 70, text, wcslen(text));
    swprintf_s(text, 256, L"Sannsynlighet Vest: %lf", vest);
    TextOut(hdc, 600, 85, text, wcslen(text));
}

void TegnTall(HDC hdc, int tall1, int tall2) {

    TCHAR text[256];
    swprintf_s(text, 256, L"Tall1 vest: %lf", tall1);
    TextOut(hdc, 600, 100, text, wcslen(text));
    swprintf_s(text, 256, L"Tall2 nord: %lf", tall2);
    TextOut(hdc, 600, 115, text, wcslen(text));
}

void lagLys(HWND hWnd, HDC hdc, int left1, int top1, int right1, int bottom1, BYTE R1, BYTE G1, BYTE B1, int left2, int top2, int right2, int bottom2, BYTE R2, BYTE G2, BYTE B2, int left3, int top3, int right3, int bottom3, BYTE R3, BYTE G3, BYTE B3) {
   
    double skala = 0.5;

    
    HBRUSH hBrush1 = CreateSolidBrush(RGB(R1,G1,B1));
    HGDIOBJ hOrg1 = SelectObject(hdc, hBrush1);
    Ellipse(hdc, left1*skala, top1 * skala, right1 * skala, bottom1 * skala);
    
    HBRUSH hBrush2 = CreateSolidBrush(RGB(R2, G2, B2));
    HGDIOBJ hOrg2 = SelectObject(hdc, hBrush2);
    Ellipse(hdc, left2 * skala, top2 * skala, right2 * skala, bottom2 * skala);

    HBRUSH hBrush3 = CreateSolidBrush(RGB(R3, G3, B3));
    HGDIOBJ hOrg3 = SelectObject(hdc, hBrush3);
    Ellipse(hdc, left3 * skala, top3 * skala, right3 * skala, bottom3 * skala);
    
    // Cleanup, set back original brush
    SelectObject(hdc, hOrg1);
    DeleteObject(hBrush1);
    DeleteObject(hBrush2);
    DeleteObject(hBrush3);
}

void visState(HWND hWnd, HDC hdc, int staten, int posX, int posY) {
    if (staten == 1) {
        lagLys(hWnd, hdc, 100+posX, 100+posY, 200 + posX, 200 + posY, 255, 0, 0, 100 + posX, 200 + posY, 200 + posX, 300 + posY, 100, 100, 100, 100 + posX, 300 + posY, 200 + posX, 400 + posY, 100, 100, 100);
    }
    if (staten == 2) {
        lagLys(hWnd, hdc, 100 + posX, 100 + posY, 200 + posX, 200 + posY, 255, 0, 0, 100 + posX, 200 + posY, 200 + posX, 300 + posY, 255, 255, 0, 100 + posX, 300 + posY, 200 + posX, 400 + posY, 100, 100, 100);
    }
    if (staten == 3) {
        lagLys(hWnd, hdc, 100 + posX, 100 + posY, 200 + posX, 200 + posY, 100, 100, 100, 100 + posX, 200 + posY, 200 + posX, 300 + posY, 100, 100, 100, 100 + posX, 300 + posY, 200 + posX, 400 + posY, 0, 255, 0);
    }
    if (staten == 4) {
        lagLys(hWnd, hdc, 100 + posX, 100 + posY, 200 + posX, 200 + posY, 100, 100, 100, 100 + posX, 200 + posY, 200 + posX, 300 + posY, 255, 255, 0, 100 + posX, 300 + posY, 200 + posX, 400 + posY, 100, 100, 100);
    }
}

//vest-øst
void kjorBiler() {
    
    list<Car*>::iterator i;
    list<Car*>::iterator j;
    for (i = l.begin(); i != l.end(); ++i) {

        if(i == l.begin()) {
        j = l.begin();
        }

        int x = (*i)->getX();

        int xPosFremfor;
        if (i == l.begin()) {
            xPosFremfor = 2000;
        }

        if (i != l.begin()) {
            xPosFremfor = (*j)->getX();
        }

        if ((((390 < x) && (x < 395)) && (iteratorLys % 4 != 2)) || ((xPosFremfor - x) < 20)) {

        }

        else {
            (*i)->kjorBilX();
        }

        if (i != l.begin()) {
            ++j;
        }   
    } 
}

//nord-sør
void kjorBiler2() {
    list<Car*>::iterator i;
    list<Car*>::iterator j;
    for (i = l2.begin(); i != l2.end(); ++i) {

        if (i == l2.begin()) {
            j = l2.begin();
        }

        int y = (*i)->getY();

        int yPosFremfor;
        if (i == l2.begin()) {
            yPosFremfor = 2000;
        }

        if (i != l2.begin()) {
            yPosFremfor = (*j)->getY();
        }

        if ((((290 < y) && (y < 295)) && (iteratorLys % 4 != 0)) || ((yPosFremfor - y) < 20)) {

        }

        else {
            (*i)->kjorBilY();
        }

        if (i != l2.begin()) {
            ++j;
        }
    }
}

//oppretter bil vest
void bilOppretterVest() {
int x = 100;
int y = 325;
l.push_back(new Car(x, y));
}

//oppretter bil nord
void bilOppretterNord() {
    int x = 425;
    int y = 50;
    l2.push_back(new Car(x, y));
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_RBUTTONDOWN:
    {
        DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);

        break;
    }

    case WM_LBUTTONDOWN:
    {
        DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, SpawnRateDialog);
        InvalidateRect(hWnd, 0, true);
        break;
    }

    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_LEFT:
            pw = pw - 10;
            break;
        case VK_RIGHT:
            pw = pw + 10;
            break;
        case VK_UP:
            pn = pn + 10;
            break;
        case VK_DOWN:
            pn = pn - 10;
            break;
        default:
            break;
        }
    }
    break;

    case WM_TIMER:
    {
        switch (wParam)
        {

        case 1:
            //trafikklys
            iteratorLys++;
            InvalidateRect(hWnd, 0, true);
            break;

        case 2: //biler fra nord spawner
            kjorBiler2();
            InvalidateRect(hWnd, 0, true);
            break;

        case 3:

            kjorBiler();

            InvalidateRect(hWnd, 0, true);
            break;

        case 4:

            tall1 = rand() % 100;

            if (tall1 < pw) {
                bilOppretterVest();
               // InvalidateRect(hWnd, 0, true);
                break;
            }

        case 5:
        
            tall2 = rand() % 100;

            if (tall2 < pn) {
                bilOppretterNord();
               // InvalidateRect(hWnd, 0, true);
            }
            break;
        }
    }
    
    case WM_PAINT:
    {
        
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        //tekst
        TegnSannsynlighet(hdc, pn, pw);

        //tall
       //( TegnTall(hdc, tall1, tall2);

        //vei
        HBRUSH hBrush1 = CreateSolidBrush(RGB(100, 100, 100));
        HGDIOBJ hOrg1 = SelectObject(hdc, hBrush1);
        Rectangle(hdc, 50, 300, 800, 350);
        Rectangle(hdc, 400, 50, 450, 800);
        DeleteObject(hBrush1);

        //biler vest-øst
        list<Car*>::iterator i;
        for (i = l.begin(); i != l.end(); ++i) {
            int x = (*i)->getX();
            int y = (*i)->getY();
            
            HBRUSH hBrush1 = CreateSolidBrush(RGB(255, 0, 100));
            HGDIOBJ hOrg1 = SelectObject(hdc, hBrush1);
            Rectangle(hdc, x, y, x + 10, y + 10);
            DeleteObject(hBrush1);
        }

        //biler nord-sør
        list<Car*>::iterator j;
        for (j = l2.begin(); j != l2.end(); ++j) {
            int x = (*j)->getX();
            int y = (*j)->getY();

            HBRUSH hBrush1 = CreateSolidBrush(RGB(255, 0, 100));
            HGDIOBJ hOrg1 = SelectObject(hdc, hBrush1);
            Rectangle(hdc, x, y, x + 10, y + 10);
            DeleteObject(hBrush1);
        }

        //TrafikkLys 1 og 2
        if ((iteratorLys % 4) == 0)
        {
            //rødt - state 1
            visState(hWnd, hdc, 1, 300, 180);

            //grønt - state 3
            visState(hWnd, hdc, 3, 820, 100);
        }
        if ((iteratorLys % 4) == 1)
        {
            //rødt + gult - state 2
            visState(hWnd, hdc, 2, 300, 180);
            //gult - state 4
            visState(hWnd, hdc, 4, 820, 100);
        }
        if ((iteratorLys % 4) == 2)
        {
            //grønt - state 3
            visState(hWnd, hdc, 3, 300, 180);
            //rødt - state 1
            visState(hWnd, hdc, 1, 820, 100);
        }
        if ((iteratorLys % 4) == 3)
        {
            //gult - state 4
            visState(hWnd, hdc, 4, 300, 180);
            //rødt + gult - state 2
            visState(hWnd, hdc, 2, 820, 100);
        }

        EndPaint(hWnd, &ps);
        break;
    }

    case WM_DESTROY:
    
        PostQuitMessage(0);
        KillTimer(hWnd, 1);
        break;
        default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
    

}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK SpawnRateDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
        static HWND hEdit;
    case WM_INITDIALOG:
        hWestSpawnRateEdit = CreateWindow(
            L"EDIT",
            L"",
            WS_BORDER | WS_CHILD | WS_VISIBLE,
            50, 50, 200, 20,
            hDlg, NULL, NULL, NULL);

        hNorthSpawnRateEdit = CreateWindow(
            L"EDIT",
            L"",
            WS_BORDER | WS_CHILD | WS_VISIBLE,
            50, 30, 200, 20,
            hDlg, NULL, NULL, NULL);
        SetWindowTextA(hWestSpawnRateEdit, "--fyll inn % for vest-øst--");
        SetWindowTextA(hNorthSpawnRateEdit, "--fyll inn %-- for nord-sør--");
        hEdit = GetDlgItem(hDlg, IDC_EDIT1);
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDCANCEL)
        {

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }

        if (LOWORD(wParam) == IDOK) {
            TCHAR northPercent[3];
            TCHAR westPercent[3];
            GetWindowText(hNorthSpawnRateEdit, northPercent, 3);
            GetWindowText(hWestSpawnRateEdit, westPercent, 3);

            pn = _tstoi(northPercent);
            pw = _tstoi(westPercent);
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}