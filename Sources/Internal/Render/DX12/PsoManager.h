//
// Copyright (C) Alexandr Vorontsov 2017.
// Distributed under the MIT License (license terms are at http://opensource.org/licenses/MIT).
//

#pragma once

#include <d3d12.h>
#include <map>
#include <wrl/client.h>

#include "Render/RendererPublic.h"
#include "Render/DX12/ShaderDX12.h"

namespace Kioto::Renderer
{
class Material;
class RenderPass;
class TextureManagerDX12;
class ShaderManagerDX12;
class VertexLayoutManagerDX12;

class PsoManager
{
public:
    void BuildPipelineState(const Material* mat, const RenderPass& pass, ID3D12RootSignature* sig, TextureManagerDX12* textureManager, ShaderManagerDX12* shaderManager, VertexLayoutManagerDX12* vertexLayoutManager);

private:
    std::map<uint64, Microsoft::WRL::ComPtr<ID3D12PipelineState>> m_psos;
};
}