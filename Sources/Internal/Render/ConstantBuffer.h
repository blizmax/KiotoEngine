//
// Copyright (C) Alexandr Vorontsov 2017.
// Distributed under the MIT License (license terms are at http://opensource.org/licenses/MIT).
//

#pragma once

#include "Core/CoreTypes.h"

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix3.h"
#include "Math/Matrix4.h"

#include <vector>
#include <variant>

namespace Kioto::Renderer
{
class ConstantBuffer
{
public:
    enum class eReturnCode
    {
        Ok,
        AlreadyAdded,
        NotFound,
        TypesDontMatch
    };

    ConstantBuffer() {}
    ConstantBuffer(uint16 index, uint16 space);
    ConstantBuffer(const ConstantBuffer& other);
    ConstantBuffer(ConstantBuffer&& other);
    ~ConstantBuffer();

    ConstantBuffer& operator= (ConstantBuffer other);

    eReturnCode Add(const std::string& name, float32 data);
    eReturnCode Add(const std::string& name, const Vector2& data);
    eReturnCode Add(const std::string& name, const Vector3& data);
    eReturnCode Add(const std::string& name, const Vector4& data);
    eReturnCode Add(const std::string& name, const Matrix3& data);
    eReturnCode Add(const std::string& name, const Matrix4& data);

    eReturnCode Set(const std::string& name, float32 data);
    eReturnCode Set(const std::string& name, const Vector2& data);
    eReturnCode Set(const std::string& name, const Vector3& data);
    eReturnCode Set(const std::string& name, const Vector4& data);
    eReturnCode Set(const std::string& name, const Matrix3& data);
    eReturnCode Set(const std::string& name, const Matrix4& data);

    void ComposeBufferData();
    float32* GetBufferData();
    uint32 GetDataSize() const;

    uint16 GetIndex() const;
    uint16 GetSpace() const;
    uint32 GetKey() const;

private:

    enum class eTypeName : byte
    {
        v1 = 1,
        v2 = 2,
        v3 = 3,
        v4 = 4,
        m3 = 9,
        m4 = 16
    };
    struct Param
    {
        std::string name;
        eTypeName Type;
        std::variant<float32, Vector2, Vector3, Vector4, Matrix3, Matrix4> Data;
    };

    bool Find(const std::string& name, uint32& offsetInData, Param*& resParam);

    uint16 m_index = 0;
    uint16 m_space = 0;
    uint32 m_key = 0;
    bool m_isDirty = true;
    bool m_regenerateMemLayout = true;

    std::vector<Param> m_params;
    float32* m_memData = nullptr;
    uint32 m_dataSize = 0;
    uint32 m_dataSize4ByteElem = 0;

    friend void swap(ConstantBuffer& l, ConstantBuffer& r)
    {
        std::swap(l.m_index, r.m_index);
        std::swap(l.m_space, r.m_space);
        std::swap(l.m_key, r.m_key);
        std::swap(l.m_isDirty, r.m_isDirty);
        std::swap(l.m_regenerateMemLayout, r.m_regenerateMemLayout);
        std::swap(l.m_memData, r.m_memData);
        std::swap(l.m_dataSize, r.m_dataSize);
        std::swap(l.m_dataSize4ByteElem, r.m_dataSize4ByteElem);
        l.m_params.swap(r.m_params);
    }
};

inline uint16 ConstantBuffer::GetIndex() const
{
    return m_index;
}

inline uint16 ConstantBuffer::GetSpace() const
{
    return m_space;
}

inline uint32 ConstantBuffer::GetKey() const
{
    return m_key;
}

inline float32* ConstantBuffer::GetBufferData()
{
    return m_memData;
}

inline uint32 ConstantBuffer::GetDataSize() const
{
    return m_dataSize;
}
}