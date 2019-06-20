//
// Copyright (C) Aleksandr Vorontcov. 2017
// Distributed under the MIT License (license terms are at http://opensource.org/licenses/MIT).
//

#pragma once

#include <vector>

#include "Core/CoreTypes.h"
#include "Core/ECS/Component.h"
#include "Math/Matrix4.h"
#include "Math/Vector3.h"

namespace Kioto
{
class TransformComponent : public Component
{
    DECLARE_COMPONENT(TransformComponent);

public:
    KIOTO_API TransformComponent() = default;
    KIOTO_API ~TransformComponent() = default;

    bool GetDirty() const;
    const Matrix4& GetToWorld() const;
    const Matrix4& GetToParent() const;
    const Matrix4& GetToModel() const;
    const Vector3& GetWorldPosition() const;
    const Matrix4& GetWorldRotation() const;

    void SetToWorld(const Matrix4& m);
    void SetToParent(const Matrix4& m);
    void SetToModel(const Matrix4& m);
    void SetParent(TransformComponent* parent);
    void SetWorldPosition(const Vector3& pos);
    void SetWorldRotation(const Matrix4& rot);

    Component* Clone() const override;

private:
    void SetDirty();
    void RemoveDirty();
    void SetChildrenDirty();

    Matrix4 m_toWorld = Matrix4::Identity;
    Matrix4 m_toParent = Matrix4::Identity;
    Matrix4 m_toModel = Matrix4::Identity;
    bool m_isDirty = false;

    Vector3 m_worldPosition{};
    Matrix4 m_worldRotation = Matrix4::Identity; // [a_vorontcov] TODO: quaternion.

    TransformComponent* m_parent = nullptr;
    std::vector<TransformComponent*> m_children;

    friend class TransformSystem;
};

inline bool TransformComponent::GetDirty() const
{
    return m_isDirty;
}

inline const Matrix4& TransformComponent::GetToWorld() const
{
    return m_toWorld;
}

inline const Matrix4& TransformComponent::GetToParent() const
{
    return m_toParent;
}

inline const Matrix4& TransformComponent::GetToModel() const
{
    return m_toModel;
}

inline const Vector3& TransformComponent::GetWorldPosition() const
{
    return m_worldPosition;
}

inline const Matrix4& TransformComponent::GetWorldRotation() const
{
    return m_worldRotation;
}

inline void TransformComponent::SetToWorld(const Matrix4& m)
{
    m_toWorld = m;
    if (!m_isDirty)
        SetChildrenDirty();
}

inline void TransformComponent::SetToParent(const Matrix4& m)
{
    m_toParent = m;
    if (!m_isDirty)
        SetChildrenDirty();
    m_isDirty = true;
}

inline void TransformComponent::SetToModel(const Matrix4& m)
{
    m_toModel = m;
    if (!m_isDirty)
        SetChildrenDirty();
    m_isDirty = true;
}

inline void TransformComponent::SetParent(TransformComponent* parent)
{
    m_parent = parent;
    if (!m_isDirty)
        SetChildrenDirty();
    m_isDirty = true;
}

inline void TransformComponent::SetWorldPosition(const Vector3& pos)
{
    m_worldPosition = pos;
    m_isDirty = true;
}

inline void TransformComponent::SetWorldRotation(const Matrix4& rot)
{
    m_worldRotation = rot;
    m_isDirty = true;
}

inline void TransformComponent::SetDirty()
{
    m_isDirty = true;
}

inline void TransformComponent::RemoveDirty()
{
    m_isDirty = false;
}

inline void TransformComponent::SetChildrenDirty()
{
    for (auto c : m_children)
        c->SetDirty();
}
}