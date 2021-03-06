#pragma once

#include "Render/PipelineStateParams.h"
#include "Render/RenderLayer.h"

namespace Kioto::RenderParamsConverter
{
template <typename T>
T FromString(const std::string& s)
{}

template <>
eBlendModes FromString<eBlendModes>(const std::string& s);

template <>
eRenderLayerType FromString<eRenderLayerType>(const std::string& s);

template <>
eBlendOps FromString<eBlendOps>(const std::string& s);

template <>
eFillMode FromString<eFillMode>(const std::string& s);

template <>
eColorMask FromString<eColorMask>(const std::string& s);

template <>
eCullMode FromString<eCullMode>(const std::string& s);

template <>
eZTest FromString<eZTest>(const std::string& s);

template <>
eStencilOp FromString<eStencilOp>(const std::string& s);

template <>
eStencilTest FromString<eStencilTest>(const std::string& s);
}