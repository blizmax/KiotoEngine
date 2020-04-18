#include "stdafx.h"

#include "Render/ScopedGpuProfiler.h"

#include "Render/RenderPass/RenderPass.h"

namespace Kioto::Renderer
{
ScopedGpuProfiler::ScopedGpuProfiler(RenderPass* pass, std::string name)
    : m_pass(pass)
{
    m_pass->PushCommand(RenderCommandHelpers::CreateBeginGpuEventCommand(std::move(name)));
}

ScopedGpuProfiler::~ScopedGpuProfiler()
{
    m_pass->PushCommand(RenderCommandHelpers::CreateEndGpuEventCommand());
}
}