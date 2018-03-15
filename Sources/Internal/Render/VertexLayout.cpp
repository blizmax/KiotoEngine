//
// Copyright (C) Alexandr Vorontsov. 2017
// Distributed under the MIT License (license terms are at http://opensource.org/licenses/MIT).
//

#include "stdafx.h"

#include "Render/VertexLayout.h"

#include <map>

namespace Kioto::Renderer
{
static const std::map<const eDataFormat, uint16> formats
{
    { eDataFormat::UNKNOWN, 0 },
    { eDataFormat::R32_G32_B32_A32, 16 },
    { eDataFormat::R32_G32_B32, 12 },
    { eDataFormat::R32_G32, 8 },
    { eDataFormat::R32, 4 }
};


const VertexLayout VertexLayout::LayoutPos3Uv2Norm3
{
    std::vector<VertexDesc>
{
    { eVertexSemantic::Position, 0, eDataFormat::R32_G32_B32, 0 },
    { eVertexSemantic::Texcoord, 0, eDataFormat::R32_G32, 12 },
    { eVertexSemantic::Normal, 0, eDataFormat::R32_G32_B32, 20 }
}
};

const VertexLayout VertexLayout::LayoutPos3Norm3
{
    std::vector<VertexDesc>
{
    { eVertexSemantic::Position, 0, eDataFormat::R32_G32_B32, 0 },
    { eVertexSemantic::Normal, 0, eDataFormat::R32_G32_B32, 12 }
}
};

const VertexLayout VertexLayout::LayoutPos3Uv2Norm3Tan3Bit3
{
    std::vector<VertexDesc>
{
    { eVertexSemantic::Position, 0, eDataFormat::R32_G32_B32, 0 },
    { eVertexSemantic::Texcoord, 0, eDataFormat::R32_G32, 12 },
    { eVertexSemantic::Normal, 0, eDataFormat::R32_G32_B32, 20 },
    { eVertexSemantic::Tangent, 0, eDataFormat::R32_G32_B32, 32 },
    { eVertexSemantic::Bitangent, 0, eDataFormat::R32_G32_B32, 44 }
}
};

const VertexLayout VertexLayout::LayoutPos3Uv2Norm3Tan3Bit3Col4
{
    std::vector<VertexDesc>
{
    { eVertexSemantic::Position, 0, eDataFormat::R32_G32_B32, 0 },
    { eVertexSemantic::Texcoord, 0, eDataFormat::R32_G32, 12 },
    { eVertexSemantic::Normal, 0, eDataFormat::R32_G32_B32, 20 },
    { eVertexSemantic::Tangent, 0, eDataFormat::R32_G32_B32, 32 },
    { eVertexSemantic::Bitangent, 0, eDataFormat::R32_G32_B32, 44 },
    { eVertexSemantic::Color, 0, eDataFormat::R32_G32_B32_A32, 56 }
}
};

const VertexLayout VertexLayout::LayoutPos3Uv2Norm3Col4
{
    std::vector<VertexDesc>
{
    { eVertexSemantic::Position, 0, eDataFormat::R32_G32_B32, 0 },
    { eVertexSemantic::Texcoord, 0, eDataFormat::R32_G32, 12 },
    { eVertexSemantic::Normal, 0, eDataFormat::R32_G32_B32, 20 },
    { eVertexSemantic::Color, 0, eDataFormat::R32_G32_B32_A32, 32 }
}
};

const VertexLayout VertexLayout::LayoutPos3Uv2
{
    std::vector<VertexDesc>
{
    { eVertexSemantic::Position, 0, eDataFormat::R32_G32_B32, 0 },
    { eVertexSemantic::Texcoord, 0, eDataFormat::R32_G32, 12 }
}
};


VertexLayout::VertexLayout(const VertexLayout& other)
{
    m_verticesDesc = other.m_verticesDesc;
    m_totalOffset = other.m_totalOffset;
}

VertexLayout::VertexLayout(VertexLayout&& other)
{
    swap(*this, other);
}

VertexLayout::VertexLayout(std::vector<VertexDesc> desc)
{
    m_verticesDesc.swap(desc);
    for (const auto& desc : m_verticesDesc)
        m_totalOffset += desc.Offset;
}

VertexLayout& VertexLayout::operator=(VertexLayout other)
{
    swap(*this, other);
    return *this;
}

void VertexLayout::AddElement(eVertexSemantic semantic, uint8 semanticIndex, eDataFormat format)
{
    m_verticesDesc.emplace_back(semantic, semanticIndex, format, m_totalOffset);
    auto it = formats.find(format);
    if (it != formats.end())
        m_totalOffset += it->second;
}

void VertexLayout::Clear()
{
    m_verticesDesc.clear();
    m_totalOffset = 0;
}

bool VertexLayout::operator==(const VertexLayout& other) const
{
    if (other.m_verticesDesc.size() != m_verticesDesc.size())
        return false;
    for (uint32 i = 0; i < m_verticesDesc.size(); ++i)
    {
        if (m_verticesDesc[i] != other.m_verticesDesc[i])
            return false;
    }
    return true;
}

bool VertexLayout::operator!=(const VertexLayout& other) const
{
    return !(*this == other);
}

const VertexDesc* VertexLayout::FindElement(eVertexSemantic semantic, uint8 semanticIndex) const
{
    for (const auto& desc : m_verticesDesc)
    {
        if (desc.Semantic == semantic && desc.SemanticIndex == semanticIndex)
            return &desc;
    }
    return nullptr;
}

bool VertexDesc::operator==(const VertexDesc& other) const
{
    return Semantic == other.Semantic && SemanticIndex == other.SemanticIndex && Format == other.Format;
}

bool VertexDesc::operator!=(const VertexDesc& other) const
{
    return !(*this == other);
}

}