//
// Copyright (C) Alexandr Vorontsov. 2017
// Distributed under the MIT License (license terms are at http://opensource.org/licenses/MIT).
//

#pragma once

#include "Core/CoreTypes.h"
#include "Core/ECS/Component.h"
#include "Math/Matrix4.h"

namespace Kioto
{
class TransformComponent;

class CameraComponent : public Component
{
    DECLARE_COMPONENT(CameraComponent);

public:
    CameraComponent();
    CameraComponent(float32 fovY, float32 aspect, float32 nearPlane, float32 farPlane);
    ~CameraComponent() = default;

    Component* Clone() const override;

    const TransformComponent* GetTransform() const;
    void SetFovY(float32 fovY);
    void SetNearPlane(float32 nearPlane);
    void SetFarPlane(float32 farPlane);
    void SetAspect(float32 aspect);
    void SetOrthographic(bool ortho);

    float32 GetFovY() const;
    float32 GetNearPlane() const;
    float32 GetFarPlane() const;
    float32 GetAspect() const;
    bool GetOrthographic() const;

    float32 GetNearPlaneHeight() const;
    float32 GetFarPlaneHeight() const;

private:
    TransformComponent* m_transform = nullptr;
    Matrix4 m_view = Matrix4::Identity;
    bool m_isViewDirty = true;
    float32 m_fovY = 60.0f;
    float32 m_foxX = -1.0f;
    float32 m_nearPlane = 0.01f;
    float32 m_farPlane = 100.0f;
    float32 m_nearPlaneHeight = 0.0f;
    float32 m_farPlaneHeight = 0.0f;
    float32 m_aspect = 1.0f;
    bool m_hdr = true;
    bool m_isOrtho = false;

    friend class CameraSystem;
};

inline const TransformComponent* CameraComponent::GetTransform() const
{
    return m_transform;
}

inline void CameraComponent::SetFovY(float32 fovY)
{
    m_fovY = fovY;
    m_isViewDirty = true;
}

inline void CameraComponent::SetNearPlane(float32 nearPlane)
{
    m_nearPlane = nearPlane;
    m_isViewDirty = true;
}

inline void CameraComponent::SetFarPlane(float32 farPlane)
{
    m_farPlane = farPlane;
    m_isViewDirty = true;
}

inline void CameraComponent::SetAspect(float32 aspect)
{
    m_aspect = aspect;
    m_isViewDirty = true;
}

inline void CameraComponent::SetOrthographic(bool ortho)
{
    m_isOrtho = ortho;
    m_isViewDirty = true;
}

inline float32 CameraComponent::GetFovY() const
{
    return m_fovY;
}

inline float32 CameraComponent::GetNearPlane() const
{
    return m_nearPlane;
}

inline float32 CameraComponent::GetFarPlane() const
{
    return m_farPlane;
}

inline float32 CameraComponent::GetAspect() const
{
    return m_aspect;
}

inline bool CameraComponent::GetOrthographic() const
{
    return m_isOrtho;
}

inline float32 CameraComponent::GetNearPlaneHeight() const
{
    return m_nearPlane;
}
inline float32 CameraComponent::GetFarPlaneHeight() const
{
    return m_farPlane;
}

}