//
// Copyright (C) Aleksandr Vorontcov. 2017
// Distributed under the MIT License (license terms are at http://opensource.org/licenses/MIT).
//

#include "stdafx.h"

#include "Component/RenderComponent.h"

namespace Kioto
{
RenderComponent::~RenderComponent()
{
    SafeDelete(m_material);
    SafeDelete(m_mesh);
}

Component* RenderComponent::Clone() const
{
    return new RenderComponent();
}
}