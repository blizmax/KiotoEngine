//
// Copyright (C) Aleksandr Vorontcov 2017.
// Distributed under the MIT License (license terms are at http://opensource.org/licenses/MIT).
//

#pragma once

#include "Render/Renderer.h"
#include "Render/RenderCommand.h"
#include "Render/RenderPass/RenderPass.h"

namespace Kioto::Renderer
{
class ForwardRenderPass : public RenderPass
{
public:
    ForwardRenderPass()
        : RenderPass("ForwardPass")
    {
        m_material = AssetsSystem::GetRenderAssetsManager()->GetOrLoadAsset<Renderer::Material>(m_matPath);
        m_mesh = AssetsSystem::GetRenderAssetsManager()->GetOrLoadAsset<Renderer::Mesh>(m_meshPath);

        Renderer::RegisterRenderPass(this);
        SetRenderTargetCount(1);

        m_material->BuildMaterialForPass(this);
    }

    virtual void CollectRenderData() override
    {
        Renderer::RenderPacket currPacket;
        currPacket.Material = m_material->GetHandle();
        currPacket.Shader = m_material->GetShader()->GetHandle();
        currPacket.TextureSet = m_material->GetShaderData().textureSet.GetHandle();
        currPacket.Mesh = m_mesh->GetHandle();

        PushCommand(RenderCommandHelpers::CreateRenderPacketCommand(currPacket));
    }


    virtual void SubmitRenderData() override
    {
        Renderer::SubmitRenderCommands(GetRenderCommands());
    }


    virtual void Cleanup() override
    {
        throw std::logic_error("The method or operation is not implemented.");
    }

private:
    virtual void SetRenderTargets() override
    {
        SetRenderTargetsCommand cmd;
        for (int32 i = 0; i < GetRenderTargetCount(); ++i)
            cmd.RenderTargets[i] = Renderer::DefaultBackBufferHandle;
        cmd.DepthStencil = Renderer::DefaultDepthStencilHandle;

        cmd.Viewport = { 0, 0, Renderer::GetWidth(), Renderer::GetHeight() };
        cmd.Scissor = { 0, 0, Renderer::GetWidth(), Renderer::GetHeight() };
        cmd.ClearDepth = true;
        cmd.ClearDepthValue = 0.0f;
        cmd.ClearColor = true;
        cmd.ClearStencil = true;
        cmd.ClearStencilValue = 0;

        PushCommand(RenderCommandHelpers::CreateSetRenderTargetCommand(cmd));
    }


    virtual void SetPassConstantBuffers() override
    {
        throw std::logic_error("The method or operation is not implemented.");
    }


    virtual void SetCameraConstantBuffers() override
    {
        PushCommand(RenderCommandHelpers::CreateConstantBufferCommand(Renderer::GetMainCamera().GetConstantBufferHandle()));
    }




    std::string m_matPath = AssetsSystem::GetAssetFullPath("Materials\\Test.mt");
    Renderer::Material* m_material = nullptr;
    Renderer::Material* m_materialCopy = nullptr;
    std::string m_meshPath = AssetsSystem::GetAssetFullPath(R"(Models\Teapot.fbx)");
    Renderer::Mesh* m_mesh = nullptr;
    Renderer::Mesh* m_meshCopy = nullptr;

    std::string m_spherePath = AssetsSystem::GetAssetFullPath(R"(Models\Sphere.fbx)");
    std::string m_matPath = AssetsSystem::GetAssetFullPath("Materials\\TestRed.mt");
    Renderer::Material* m_materialRed = nullptr;
    Renderer::Mesh* m_meshSphere = nullptr;
};
}