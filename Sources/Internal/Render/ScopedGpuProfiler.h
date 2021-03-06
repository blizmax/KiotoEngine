#pragma once

namespace Kioto::Renderer
{
class CommandList;

class ScopedGpuProfiler
{
public:
    ScopedGpuProfiler(CommandList* cmdList, std::string name);

    ~ScopedGpuProfiler();

private:
    CommandList* m_cmdList = nullptr;
};
}