//
// Copyright (C) Alexandr Vorontsov. 2017
// Distributed under the MIT License (license terms are at http://opensource.org/licenses/MIT).
//

#include "stdafx.h"

#include "Render/Renderer.h"

#include "Render/DX12/RendererDX12.h"

namespace Kioto::Renderer
{

RendererDX12 renderer; // [a_vorontsov] Not too cross-api for now.

void Init(eRenderApi api)
{
    if (api == eRenderApi::DirectX12)
        renderer.Init();
}

void Shutdown()
{

}

void Resize(uint16 width, uint16 height, bool minimized)
{

}

void ChangeFullscreenMode(bool fullScreen)
{

}

}