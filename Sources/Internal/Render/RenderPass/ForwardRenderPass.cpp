#include "stdafx.h"

#include "Render/RenderPass/ForwardRenderPass.h"

#include "Core/KiotoEngine.h"
#include "Render/Camera.h"
#include "Render/Material.h"
#include "Render/Renderer.h"
#include "Render/RenderCommand.h"
#include "Render/RenderObject.h"
#include "Render/RenderPacket.h"
#include "Render/RenderOptions.h"
#include "Render/Shader.h"
#include "Render/RenderGraph/ResourcesBlackboard.h"

namespace Kioto::Renderer
{
ForwardRenderPass::ForwardRenderPass()
    : RenderPass("Forward")
{
    Renderer::RegisterRenderPass(this);
    SetRenderTargetCount(1);
}

void ForwardRenderPass::BuildRenderPackets(CommandList* commandList, ResourceTable& resources)
{
    SetPassConstantBuffers(commandList);
    SetCameraConstantBuffers(commandList);
    SetRenderTargets(commandList);
    for (auto ro : m_renderObjects)
    {
        Material* mat = ro->GetMaterial();
        Mesh* mesh = ro->GetMesh();
        mat->BuildMaterialForPass(this);

        ro->SetValueToBuffer("ToModel", ro->GetToModel()->GetForGPU(), m_passName);// [a_vorontcov] TODO: Move to render object?
        ro->SetValueToBuffer("ToWorld", ro->GetToWorld()->GetForGPU(), m_passName);

        RenderPacket currPacket = {};
        currPacket.Material = mat->GetHandle();
        currPacket.Shader = mat->GetPipelineState(m_passName).Shader->GetHandle();
        currPacket.TextureSet = ro->GetTextureSet(m_passName).GetHandle();
        currPacket.Mesh = mesh->GetHandle();
        currPacket.Pass = GetHandle();
        currPacket.CBSet = ro->GetBufferLayout(m_passName).bufferSetHandle;

        commandList->PushCommand(RenderCommandHelpers::CreateRenderPacketCommand(currPacket, this));
    }

    commandList->PushCommand(RenderCommandHelpers::CreatePassEndsCommand(this));
}

void ForwardRenderPass::Cleanup()
{
}

void ForwardRenderPass::SetRenderTargets(CommandList* commandList)
{
    SetRenderTargetsCommand cmd;
    cmd.SetRenderTargets(Renderer::DefaultBackBufferHandle);
    cmd.RenderTargetCount = GetRenderTargetCount();
    cmd.DepthStencil = Renderer::DefaultDepthStencilHandle;

    cmd.Viewport = { 0, 0, Renderer::GetWidth(), Renderer::GetHeight() };
    cmd.Scissor = { 0, 0, Renderer::GetWidth(), Renderer::GetHeight() };
    cmd.ClearDepth = true;
    cmd.ClearDepthValue = 0.0f;
    cmd.ClearColor = true;
    cmd.ClearStencil = true;
    cmd.ClearStencilValue = 0;

    commandList->PushCommand(RenderCommandHelpers::CreateSetRenderTargetCommand(cmd, this));
}

void ForwardRenderPass::SetPassConstantBuffers(CommandList* commandList)
{

}

void ForwardRenderPass::SetCameraConstantBuffers(CommandList* commandList)
{
    commandList->PushCommand(RenderCommandHelpers::CreateConstantBufferCommand(Renderer::GetMainCamera()->GetConstantBuffer(), this));
}

bool ForwardRenderPass::ConfigureInputsAndOutputs(ResourcesBlackboard& resources)
{
    const RenderOptions& settings = KiotoCore::GetRenderSettings();

    TextureDescriptor desc;
    desc.Dimension = eResourceDim::Texture2D;
    desc.Format = eResourceFormat::Format_R8G8B8A8_UNORM;
    desc.Flags = eResourceFlags::AllowRenderTarget;
    desc.Width = 1024;
    desc.Height = 768;
    desc.InitialState = eResourceState::Common;

    resources.NewTexture("FwdTargetTexture", std::move(desc));

    if (settings.RenderMode == RenderOptions::RenderModeOptions::Final
        || settings.RenderMode == RenderOptions::RenderModeOptions::FinalAndWireframe)
        return true;
    return false;
}

}