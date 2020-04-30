#pragma once

#include "Render/RenderPass/RenderPass.h"

namespace Kioto::Renderer
{
    class WireframeRenderPass : public RenderPass
    {
    public:
        WireframeRenderPass();

        bool ConfigureInputsAndOutputs(ResourcesBlackboard& resources) override;
        void BuildRenderPackets(CommandList* commandList) override;
        void Cleanup() override;

    private:
        void SetRenderTargets(CommandList* commandList) override;
        void SetPassConstantBuffers(CommandList* commandList) override;
        void SetCameraConstantBuffers(CommandList* commandList) override;
    };
}
