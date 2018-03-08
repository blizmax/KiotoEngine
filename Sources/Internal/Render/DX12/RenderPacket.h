//
// Copyright (C) Alexandr Vorontsov 2017.
// Distributed under the MIT License (license terms are at http://opensource.org/licenses/MIT).
//

#pragma once

#include "Render/RendererPublic.h"

#include <vector>

namespace Kioto::Renderer
{
class RenderPacket
{
public:
    MaterialHandle Material;
    ShaderHandle Shader;
    VertexLayoutHandle VertexLayout;
    TextureSetHandle TextureSet;
};

using RenderPacketList = std::vector<RenderPacket>;
}