//
// Copyright (C) Alexandr Vorontsov. 2017
// Distributed under the MIT License (license terms are at http://opensource.org/licenses/MIT).
//

#include "stdafx.h"

#include "Render/Renderer.h"

#include "Core/CoreHelpers.h"
#include "Render/DX12/RendererDX12.h"

#include "Systems/EventSystem/EngineEvents.h"
#include "Systems/EventSystem/EventSystem.h"

#include "Core/KiotoEngine.h"
#include "Core/Scene.h"

namespace Kioto::Renderer
{
namespace
{
RendererDX12* GameRenderer = nullptr; // [a_vorontsov] Not too cross-api for now.

uint16 m_height = 1024;
uint16 m_width = 768;
float32 m_aspect = 1.0f;
}

void Init(eRenderApi api, uint16 width, uint16 height)
{
    m_width = width;
    m_height = height;
    m_aspect = static_cast<float32>(m_width) / static_cast<float32>(m_height);

    GameRenderer = new RendererDX12();
    if (api == eRenderApi::DirectX12)
        GameRenderer->Init(width, height);
}

void Shutdown()
{
    GameRenderer->Shutdown();
    SafeDelete(GameRenderer);
}

void Resize(uint16 width, uint16 height, bool minimized)
{
    m_width = width;
    m_height = height;
    m_aspect = static_cast<float32>(m_width) / static_cast<float32>(m_height);

    Scene* scene = nullptr;
    if (GetScene() != nullptr)
    {
        std::shared_ptr<OnMainWindowResized> e = std::make_shared<OnMainWindowResized>();
        OnMainWindowResized::Data* data = reinterpret_cast<OnMainWindowResized::Data*>(e->GetEventData());
        data->width = m_width;
        data->height = m_height;
        data->aspect = m_aspect;

        GetScene()->GetEventSystem()->RaiseEvent(e);
    }


    GameRenderer->Resize(width, height);
}

void ChangeFullScreenMode(bool fullScreen)
{
    GameRenderer->ChangeFullScreenMode(fullScreen);
}

void Update(float32 dt) // [a_vorontsov] TODO: set frame command buffers here.
{
    GameRenderer->Update(dt);
}

void Present()
{
    GameRenderer->Present();
}

uint16 GetWidth()
{
    return m_width;
}

uint16 GetHeight()
{
    return m_height;
}

float32 GetAspect()
{
    return m_aspect;
}

}