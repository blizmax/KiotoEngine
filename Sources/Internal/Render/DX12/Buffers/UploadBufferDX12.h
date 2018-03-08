//
// Copyright (C) Alexandr Vorontsov 2018.
// Distributed under the MIT License (license terms are at http://opensource.org/licenses/MIT).
//

#pragma once

#include "Core/CoreTypes.h"

#include <d3d12.h>
#include <wrl.h>
#include "Sources/External/Dx12Helpers/d3dx12.h"

namespace Kioto::Renderer
{
class UploadBufferDX12 final
{
public:
    UploadBufferDX12(uint32 framesCount, float32* data, uint32 dataSize, bool isConstantBuffer, ID3D12Device* device);
    UploadBufferDX12(const UploadBufferDX12&) = delete;
    UploadBufferDX12(UploadBufferDX12&&) = delete;
    UploadBufferDX12& operator=(const UploadBufferDX12&) = delete;
    UploadBufferDX12& operator=(UploadBufferDX12&&) = delete;
    ~UploadBufferDX12();

    ID3D12Resource* GetResource() const;
    void UploadData(uint32 frameIndex, const float32* data);
    const byte* const GetBufferStart() const;
    const byte* const GetBufferEnd() const;
    size_t GetFrameDataSize() const;
    size_t GetBufferSize() const;
    uint32 GetFramesCount() const;

    D3D12_GPU_VIRTUAL_ADDRESS GetFrameDataGpuAddress(uint32 frame) const;

private:
    bool m_isConstantBuffer = false;
    size_t m_frameDataSize = 0;
    size_t m_rawDataSize = 0;
    size_t m_bufferSize = 0;
    uint32 m_framesCount = 0;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;
    byte* m_data = nullptr;

    static constexpr uint32 GetConstantBufferByteSize(uint32 byteSize); // [a_vorontsov] Constant buffers must be 255 byte aligned.
};

}