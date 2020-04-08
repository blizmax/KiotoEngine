#include "stdafx.h"

#include "Render/Texture/TextureDX12.h"

#include "Sources/External/Dx12Helpers/DDSTextureLoader.h"

namespace Kioto::Renderer
{
void TextureDX12::Create(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
    HRESULT texRes = DirectX::CreateDDSTextureFromFile12(device, commandList, Path.c_str(), Resource, UploadResource);
    ThrowIfFailed(texRes);
}
}
