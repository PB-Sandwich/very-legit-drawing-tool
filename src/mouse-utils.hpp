
#pragma once

#ifdef WINDOWS
#include <windows.h>
#endif

void inline setCursorPosition(int x, int y)
{
#ifdef WINDOWS
    SetCursorPos(x, y);
#endif
}

void inline mouseLeftDown()
{
#ifdef WINDOWS
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
#endif
}

void inline mouseLeftUp()
{
#ifdef WINDOWS
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
#endif
}

void inline sleep(int milliseconds)
{
#ifdef WINDOWS
    Sleep(milliseconds);
#endif
}

// returns true on success
bool inline getCursorPosition(int& x, int& y)
{
#ifdef WINDOWS
    POINT p;
    if (GetCursorPos(&p)) {
        x = p.x;
        y = p.y;
        return true;
    }
    return false;
#endif
}


#ifdef WINDOWS

static void (*mouseListen)(int x, int y);
static HHOOK mouseHook;

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0) {
        MSLLHOOKSTRUCT* pMouseStruct = (MSLLHOOKSTRUCT*)lParam;
        if (pMouseStruct != nullptr) {
            if (wParam == WM_LBUTTONDOWN) {
                mouseListen(pMouseStruct->pt.x, pMouseStruct->pt.y);
            }
        }
    }
    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

// return true on success
bool mouseInit(void (*listen)(int x, int y))
{
    if (listen == nullptr) {
        std::cerr << "Mouse listener function cannot be null.\n";
        return false;
    }
    mouseListen = listen;
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0);
    if (!mouseHook) {
        std::cerr << "Failed to install hook.\n";
        return 1;
    }
}

void waitForMouseEvent()
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void mouseUninit()
{
    if (mouseHook) {
        UnhookWindowsHookEx(mouseHook);
        mouseHook = NULL;
    }
}

#endif
