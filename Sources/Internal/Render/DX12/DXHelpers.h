#pragma once

#include <array>
#include <d3d12.h>
#include <exception>
#include <string>
#include <windows.h>

#include "Render/Geometry/Mesh.h"

namespace Kioto::Renderer
{
class ComException : public std::exception // [a_vorontcov] https://github.com/Microsoft/DirectXTK/wiki/ThrowIfFailed
{
public:
    ComException(HRESULT hr)
        : m_result(hr)
    {}

    virtual const char* what() const override
    {
        static char str[64] = { 0 };
        sprintf_s(str, "Failure with HRESULT of %08X", m_result);
        return str;
    }

private:
    HRESULT m_result = -1;
};

inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw ComException(hr);
    }
}

#if defined(_DEBUG)
inline void SetName(ID3D12Object* object, LPCWSTR name)
{
    object->SetName(name);
}
#else
inline void SetName(ID3D12Object*, LPCWSTR)
{
}
#endif

#define NAME_D3D12_OBJECT(x) SetName(x.Get(), L#x)

#ifndef ReleaseComPtr
#define ReleaseComPtr(x) { if (x) { x->Release(); x = nullptr; }}
#endif

inline UINT CalculateConstantBufferByteSize(UINT byteSize) // [a_vorontcov] Dx12 constant buffers must be 255 byte aligned.
{
    return (byteSize + 255) & ~255;
}

inline constexpr DXGI_FORMAT IndexFormatToDXGI(eIndexFormat format)
{
    if (format == eIndexFormat::Format16Bit)
        return DXGI_FORMAT_R16_UINT;
    if (format == eIndexFormat::Format32Bit)
        return DXGI_FORMAT_R32_UINT;
    return DXGI_FORMAT_R8_UINT;
}

///
/// Get predefined static samplers.
///
inline std::array<const CD3DX12_STATIC_SAMPLER_DESC, 2> GetStaticSamplers()
{
    const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
        0,
        D3D12_FILTER_MIN_MAG_MIP_LINEAR,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
    const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
        1,
        D3D12_FILTER_MIN_MAG_MIP_LINEAR,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP);
    return { linearClamp, linearWrap };
}
}
