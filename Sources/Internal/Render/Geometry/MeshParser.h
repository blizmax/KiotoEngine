//
// Copyright (C) Aleksandr Vorontcov. 2018
// Distributed under the MIT License (license terms are at http://opensource.org/licenses/MIT).
//

#pragma once

#include <string>

namespace Kioto
{
namespace Renderer
{
class Mesh;
}

class MeshParser
{
public:
    virtual void Init() abstract;
    virtual void Shutdown() abstract;
    virtual Renderer::Mesh* ParseMesh(const std::string& path) abstract;
    virtual void ParseMesh(Renderer::Mesh* dst) abstract;

    virtual ~MeshParser() = default;
};
}