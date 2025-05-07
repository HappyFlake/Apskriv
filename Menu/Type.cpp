#include <windows.h>
#include <stdio.h>

#define ID_BUTTONEXIT 5
#define ID_BUTTONSETTINGS 4
#define ID_BUTTONPLAY 3


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, int nCmdShow)
{
    // Register the window class.
    const char CLASS_NAME[] = "BlankWinForm";

    WNDCLASS wc = {0};

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    RegisterClass(&wc);

    RECT workArea;

    SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);

    int windowWidth = 960, windowHeight = 540; // Här tar man storleken på fönstret och storleken på användarens skärm för att centrera fönstret
    int xPos = (workArea.right - windowWidth) / 2;
    int yPos = (workArea.bottom - windowHeight) / 2;
    
    // Create the window.

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, NULL, 
        WS_POPUP | WS_SYSMENU, xPos, yPos,
        windowWidth, windowHeight,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hButtonExit, hButtonPlay, hButtonSettings;
    switch (msg) {
        case WM_CREATE: // Här Skapat bygg klossarna
            hButtonPlay =
                CreateWindow("Button", "Play", WS_CHILD | WS_VISIBLE | WS_BORDER, 430, 250, 100, 20, hwnd, (HMENU)ID_BUTTONPLAY, NULL, NULL);
                
            hButtonSettings =
                CreateWindow("Button", "Settings", WS_CHILD | WS_VISIBLE | WS_BORDER, 430, 275, 100, 20, hwnd, (HMENU)ID_BUTTONSETTINGS, NULL, NULL);

            hButtonExit =
                CreateWindow("Button", "Exit", WS_CHILD | WS_VISIBLE | WS_BORDER, 430, 485, 100, 20, hwnd, (HMENU)ID_BUTTONEXIT, NULL, NULL);
            
            

            
            return 0;
        
        case WM_COMMAND: // Här Skapas funktionerna till knapparna
            if (LOWORD(wParam) == ID_BUTTONEXIT ) {
                DestroyWindow(hwnd);
            }

            if (LOWORD(wParam) == ID_BUTTONPLAY ) {
                system("..\\Play\\Play.exe");
            }

            if (LOWORD(wParam) == ID_BUTTONSETTINGS ) {
                system("..\\Settings\\Settings.exe");
            }
        
        return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            SetTextColor(hdc, RGB(255, 0, 0));
            SetBkMode(hdc, TRANSPARENT);
            TextOut(hdc, 425, 100, "Monkey Write", 16);

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
