//
// Copyright (C) Alexandr Vorontsov. 2017
// Distributed under the MIT License (license terms are at http://opensource.org/licenses/MIT).
//

#pragma once

#include <d3d12.h>
#include <exception>
#include <string>
#include <windows.h>

namespace Kioto::Renderer
{
class ComException : public std::exception // [a_vorontsov] https://github.com/Microsoft/DirectXTK/wiki/ThrowIfFailed
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

}
