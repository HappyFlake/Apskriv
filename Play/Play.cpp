#include <windows.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <wchar.h>

#define ID_BUTTONEND 5


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int Textpos = 0;

std::string fileContent;
std::string line;
std::string chars;

std::string Correct = ""; // Green 
std::string False = ""; // Red


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
    static HWND hButtonEnd;

    char keymsg[32];
    switch (msg) {
        case WM_CREATE: {// Här Skapat bygg klossarna
            std::ifstream file("text.txt");
            int x, y;
            x = y = 0;
            if (file) {
                std::string line;
                while (std::getline(file, line)) {
                    fileContent += line + "\n";
                    chars = line;
                }
            }

            hButtonEnd =
                CreateWindow("Button", "End", WS_CHILD | WS_VISIBLE | WS_BORDER, 430, 485, 100, 20, hwnd, (HMENU)ID_BUTTONEND, NULL, NULL);
            
            return 0;
        }

        case WM_CHAR: // Knapp tryckning
            if (chars[Textpos] == wParam) {
                Correct += chars[Textpos];
                False += " ";
            } else {
                Correct += " ";
                False += chars[Textpos];
            }
            Textpos++;

            InvalidateRect(hwnd, NULL, TRUE); // Uppdaterar fönstret, så att den overlapar det svar användaren skriver.

            SendMessage(hwnd, WM_PAINT, 0, 0);
        break;

        case WM_COMMAND: // Här Skapas funktionerna till knapparna
            if (LOWORD(wParam) == ID_BUTTONEND ) {
                DestroyWindow(hwnd);
            }

        return 0;

        case WM_SETFONT:

        return 0;

        case WM_GETFONT:

        return 0;
        
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            

            HFONT hFont = CreateFont (20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, 
        OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
        DEFAULT_PITCH | FF_DONTCARE, TEXT("Consolas"));

            auto hOldFont = (HFONT) SelectObject(hdc, hFont);

            SetTextColor(hdc, RGB(226, 165, 40));
            SetBkMode(hdc, TRANSPARENT);
            RECT rect = rect;
            rect.top = 0;

            GetClientRect(hwnd, &rect);

            DrawTextA(hdc, fileContent.c_str(), -1, &rect, DT_VCENTER | DT_WORDBREAK); // Yellow
            
            RECT rectFalse = rect;
            rectFalse.top = 0;
            SetTextColor(hdc, RGB(255, 0, 0));
            DrawTextA(hdc, False.c_str(), -1, &rectFalse, DT_VCENTER | DT_WORDBREAK); // Red
            
            RECT rectCorrect = rect;
            rectCorrect.top = 0;
            SetTextColor(hdc, RGB(0, 255, 0));
            DrawTextA(hdc, Correct.c_str(), -1, &rectCorrect, DT_VCENTER | DT_WORDBREAK); // Green

            SelectObject(hdc, hOldFont);
            DeleteObject(hFont);
                   
            EndPaint(hwnd, &ps);

            return 0;
        }
        
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}


// Make the Correct n False character line up with the Textpos,
// Make a timer
// Make it summarize when timer is done,
// Make the words random scrambeld everytime user starts a game.