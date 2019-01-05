//
// Copyright (C) Aleksandr Vorontcov. 2017
// Distributed under the MIT License (license terms are at http://opensource.org/licenses/MIT).
//

#include "stdafx.h"

#include "Systems/CameraSystem.h"

#include "Component/CameraComponent.h"
#include "Core/ECS/Entity.h"
#include "Math/MathHelpers.h"
#include "Systems/EventSystem/EventSystem.h"
#include "Systems/EventSystem/EngineEvents.h"
#include "Render/Renderer.h"

namespace Kioto
{

CameraSystem::CameraSystem()
{
    m_components.reserve(4);
}

CameraSystem::~CameraSystem()
{
    m_components.clear();
}

void CameraSystem::Init()
{
    EventSystem::GlobalEventSystem.Subscribe<OnMainWindowResized>({
        [this](std::shared_ptr<Event> e)
    {
        if (m_mainCamera == nullptr)
            return;
        OnMainWindowResized::Data* resizeData = reinterpret_cast<OnMainWindowResized::Data*>(e->GetEventData());
        m_mainCamera->SetAspect(resizeData->aspect);
    }
    }, this);
}

void CameraSystem::OnEntityAdd(Entity* entity)
{
    CameraComponent* t = entity->GetComponent<CameraComponent>();
    if (t == nullptr)
        return;
    m_components.push_back(t);
}

void CameraSystem::OnEntityRemove(Entity* entity)
{
    CameraComponent* t = entity->GetComponent<CameraComponent>();
    if (t == nullptr)
        return;
    auto it = std::find(m_components.begin(), m_components.end(), t);
    if (it != m_components.end())
        m_components.erase(it);
}

void CameraSystem::Update(float32 dt)
{
    for (CameraComponent* camComponent : m_components)
    {
        Renderer::Camera& currCam = camComponent->GetCamera();
        if (camComponent->GetIsMain())
            m_mainCamera = currCam;

        UpdateView(camComponent);
        if (currCam->m_isProjDirty)
            UpdateProjection(currCam);

        currCam->m_VP = currCam->m_view * currCam->m_projection;
    }
    Renderer::SetMainCamera(m_mainCamera);
}

void CameraSystem::Shutdown()
{
    EventSystem::GlobalEventSystem.Unsubscribe(this);
}

void CameraSystem::UpdateView(CameraComponent* cam)
{
    Matrix4 m = cam->GetTransform()->GetToWorld();
    cam->m_view = m.InversedOrthonorm();
}

void CameraSystem::UpdateProjection(Renderer::Camera* cam)
{
    cam->m_projection = Matrix4::BuildProjectionFov(cam->GetFovY(), cam->GetAspect(), cam->GetNearPlane(), cam->GetFarPlane());
    // [a_vorontcov] TODO: If cam - ortho, than other, but later.

    cam->m_nearPlaneHeight = 2.0f * cam->m_nearPlane * std::tan(0.5f * cam->m_fovY);
    cam->m_farPlaneHeight = 2.0f * cam->m_farPlane * std::tan(0.5f * cam->m_fovY);

    float32 halfWidth = 0.5f * cam->m_aspect * cam->m_nearPlaneHeight;
    cam->m_foxX = 2.0f * atan(halfWidth / cam->m_nearPlane);

    cam->m_isProjDirty = false;
}

}