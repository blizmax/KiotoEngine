//
// Copyright (C) Alexandr Vorontsov. 2017
// Distributed under the MIT License (license terms are at http://opensource.org/licenses/MIT).
//

#pragma once

#define WIN32_LEAN_AND_MEAN

#include <string>
#include <functional>
#include <windows.h>

#include "Core/Core.h"
#include "Core/CoreTypes.h"

#define RUN_KIOTO                                                                                                   \
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int nCmdShow)                         \
{                                                                                                                   \
    Kioto::KiotoMain(hInstance, prevInstance, cmdLine, nCmdShow, L"Kioto game", OnEngineInit, OnEngineShutdown);    \
    return 0;                                                                                                       \
}

namespace Kioto
{
class Scene;

///
/// Entry point of Kioto Engine. Call this function from your WinMain.
///
KIOTO_API void KiotoMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int nCmdShow, std::wstring capture, std::function<void()> initEngineCallback = nullptr, std::function<void()> shutdownEngineCallback = nullptr);
///
/// Set scene to engine.
///
KIOTO_API void SetScene(Scene* scene);
///
/// Get active scene.
///
KIOTO_API Scene* GetScene();

namespace KiotoCore
{
struct ApplicationInfoData
{
    HINSTANCE HInstance = {};
    HINSTANCE PrevInstance = {};
    PSTR CmdLine = {};
    int NCmdShow = {};
    std::wstring WindowCapture;
};

void Init();
void Update();
void Shutdown();
void ChangeFullscreenMode(bool fullScreen);
void Resize(uint16 width, uint16 height, bool minimized);

}

}