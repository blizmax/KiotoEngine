//
// Copyright (C) Aleksandr Vorontcov. 2017
// Distributed under the MIT License (license terms are at http://opensource.org/licenses/MIT).
//

#include "stdafx.h"

#include "Core/WindowsApplication.h"

#include <functional>
#include <Strsafe.h>

#include "Core/CoreTypes.h"

namespace Kioto
{
namespace KiotoCore
{
void Init();
void Update();
void Shutdown();
void ChangeFullscreenMode(bool fullScreen);
void Resize(uint16 width, uint16 height, bool minimized);
}

namespace WindowsApplication
{
namespace
{
const UINT WindowStyle = WS_OVERLAPPEDWINDOW;
RECT WindowRect = {};
std::wstring WindowCaption;
bool IsFullscreen = false;
HWND Hwnd;
}

void MessageError(LPTSTR lpszFunction);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

bool Init(HINSTANCE hInstance, int32 nCmdShow, std::wstring caption)
{
    // TODO: create mutex to check if another instance exist.
    IsFullscreen = false;
    WindowCaption = caption;

    WNDCLASSEX windowClass = {};
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = hInstance;
    windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowClass.lpszClassName = caption.c_str();
    RegisterClassEx(&windowClass);

    RECT windowRect = { 0, 0, 1024, 768 };
    if (!AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE))
        return false;

    Hwnd = CreateWindow(windowClass.lpszClassName,
        WindowCaption.c_str(),
        WindowStyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr,
        nullptr,
        hInstance,
        nullptr);

    if (Hwnd == nullptr)
    {
        MessageError(TEXT("Create window"));
        return false;
    }

    ShowWindow(Hwnd, nCmdShow);

    return true;
}

int64 Run()
{
    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            KiotoCore::Update(); // [a_vorontcov] TODO: If app minimized go to sleep.
        }
    }
    Shutdown();

    return static_cast<int64>(msg.wParam);
}

void Shutdown()
{
    KiotoCore::Shutdown();
}

void ChangeFullscreenMode(bool fullScreen)
{
    if (fullScreen == IsFullscreen)
        return;

    IsFullscreen = fullScreen;
    if (!IsFullscreen)
    {
        SetWindowLong(Hwnd, GWL_STYLE, WindowStyle);

        SetWindowPos(
            Hwnd,
            HWND_NOTOPMOST,
            WindowRect.left,
            WindowRect.top,
            WindowRect.right - WindowRect.left,
            WindowRect.bottom - WindowRect.top,
            SWP_FRAMECHANGED | SWP_NOACTIVATE);

        ShowWindow(Hwnd, SW_NORMAL);
        return;
    }
    GetWindowRect(Hwnd, &WindowRect);

    UINT fullScreenWindowStyle = WindowStyle & ~(WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME);
    SetWindowLong(Hwnd, GWL_STYLE, fullScreenWindowStyle);

    DEVMODE devMode = {};
    devMode.dmSize = sizeof(DEVMODE);
    EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode);

    SetWindowPos(
        Hwnd,
        HWND_TOPMOST,
        devMode.dmPosition.x,
        devMode.dmPosition.y,
        devMode.dmPosition.x + devMode.dmPelsWidth,
        devMode.dmPosition.y + devMode.dmPelsHeight,
        SWP_FRAMECHANGED | SWP_NOACTIVATE);

    ShowWindow(Hwnd, SW_MAXIMIZE);
}

HWND GetHWND()
{
    return Hwnd;
}

LRESULT WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
    }
    return 0;

    case WM_KEYDOWN:
        return 0;

    case WM_KEYUP:
        return 0;

    case WM_SYSKEYDOWN:
        if ((wParam == VK_RETURN) && (lParam & (1 << 29))) // [a_vorontcov] Handle Alt+Enter.
        {
            KiotoCore::ChangeFullscreenMode(!IsFullscreen);
        }
        return 0;

    case WM_EXITSIZEMOVE:
    {
        RECT clientRect = {};
        GetClientRect(Hwnd, &clientRect);
        KiotoCore::Resize(static_cast<uint16>(clientRect.right - clientRect.left), static_cast<uint16>(clientRect.bottom - clientRect.top), wParam == SIZE_MINIMIZED);
    }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

void MessageError(LPTSTR lpszFunction)
{
    LPVOID messageBuffer;
    LPVOID displayBuffer;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&messageBuffer,
        0, nullptr);

    displayBuffer = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)messageBuffer) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)displayBuffer,
        LocalSize(displayBuffer) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, messageBuffer);
    MessageBox(nullptr, (LPCTSTR)displayBuffer, TEXT("Error"), MB_OK);

    LocalFree(messageBuffer);
    LocalFree(displayBuffer);
}
}
}