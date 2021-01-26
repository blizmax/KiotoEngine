#include "stdafx.h"

#include "Render/RenderPass/ShadowMapRenderPass.h"

#include "Core/KiotoEngine.h"
#include "Render/Camera.h"
#include "Render/Geometry/GeometryGenerator.h"
#include "Render/Material.h"
#include "Render/Renderer.h"
#include "Render/RenderCommand.h"
#include "Render/RenderObject.h"
#include "Render/RenderPacket.h"
#include "Render/RenderSettings.h"
#include "Render/Shader.h"
#include "Render/RenderGraph/ResourcesBlackboard.h"
#include "Render/RenderGraph/ResourceTable.h"

#include "Render/Shaders/autogen/sInp/ShadowMap.h"

namespace Kioto::Renderer
{
ShadowMapRenderPass::ShadowMapRenderPass()
    : RenderPass("ShadowMap")
{
    Renderer::RegisterRenderPass(this);

    SetRenderTargetCount(1);
    m_debugQuad = GeometryGenerator::GetFullscreenQuad();
}

bool ShadowMapRenderPass::ConfigureInputsAndOutputs(ResourcesBlackboard& resources)
{
    TextureDescriptor desc;
    desc.Dimension = eResourceDim::Texture2D;
    desc.Format = eResourceFormat::Format_R8G8B8A8_UNORM;
    desc.Flags = eResourceFlags::AllowRenderTarget;
    desc.Width = m_shadowmapSize;
    desc.Height = m_shadowmapSize;
    desc.InitialState = eResourceState::Common;
    desc.FastClear = true;
    desc.FastClearValue = Color::Black;
    desc.Name = "ShadowMap";

    resources.NewTexture("ShadowMap", std::move(desc));
    resources.ScheduleWrite("ShadowMap");

    return true;
}

void ShadowMapRenderPass::BuildRenderPackets(CommandList* commandList, ResourceTable& resources)
{
    SetRenderTargets(commandList, resources);

    for (auto ro : m_drawData->RenderObjects)
    {
        if (!ro->GetIsVisible() || !ro->GetCastShadow())
            continue;

        ro->SetExternalCB(m_passName, Renderer::SInp::ShadowMap_sinp::cbCameraName, Renderer::GetMainCamera()->GetConstantBuffer().GetHandle());
        ro->SetExternalCB(m_passName, Renderer::SInp::ShadowMap_sinp::cbEngineName, Renderer::EngineBuffers::GetTimeBuffer().GetHandle());

        Material* mat = ro->GetMaterial();
        Mesh* mesh = ro->GetMesh();
        mat->BuildMaterialForPass(this);

        ro->PrepareConstantBuffers(m_passName);

        RenderPacket currPacket = {};
        currPacket.Material = mat->GetHandle();
        currPacket.Shader = mat->GetPipelineState(m_passName).Shader->GetHandle();
        currPacket.TextureSet = ro->GetTextureSet(m_passName).GetHandle();
        currPacket.Mesh = mesh->GetHandle();
        currPacket.Pass = GetHandle();
        currPacket.ConstantBufferHandles = std::move(ro->GetCBHandles(m_passName));

        commandList->PushCommand(RenderCommandHelpers::CreateRenderPacketCommand(currPacket, this));
    }
    commandList->PushCommand(RenderCommandHelpers::CreatePassEndsCommand(this));
}

void ShadowMapRenderPass::Cleanup()
{

}

void ShadowMapRenderPass::SetRenderTargets(CommandList* commandList, ResourceTable& resources)
{
    SetRenderTargetsCommand cmd;
    Texture* shadowMap = resources.GetResource("ShadowMap");
    cmd.SetRenderTargets(shadowMap->GetHandle());
    cmd.RenderTargetCount = GetRenderTargetCount();
    cmd.DepthStencil = Renderer::DefaultDepthStencilHandle;

    cmd.Viewport = { 0, 0, m_shadowmapSize, m_shadowmapSize };
    cmd.Scissor = { 0, 0, m_shadowmapSize, m_shadowmapSize };
    cmd.ClearDepth = true;
    cmd.ClearDepthValue = 0.f;
    cmd.ClearColor = true;
    cmd.ClearColorValue = Color::Black;
    cmd.ClearStencil = true;
    cmd.ClearStencilValue = 0;

    commandList->PushCommand(RenderCommandHelpers::CreateSetRenderTargetCommand(cmd, this));
}

}