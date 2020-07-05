#pragma once

#include <vector>
#include <variant>

#include "Core/CoreTypes.h"

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix3.h"
#include "Math/Matrix4.h"

#include "Render/RendererPublic.h"

namespace Kioto::Renderer
{
struct ConstantBufferSet
{
    ConstantBufferSetHandle Handle;
    std::vector<ConstantBufferHandle> BuffersSet;
};

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
    template <typename T>
    ConstantBuffer(uint16 index, uint16 space, uint16 elemNum = 1);
    template <typename T>
    void Set(const T& val, uint16 elemOffset = 0);
    template <typename T>
    T* Get(uint16 elemOffset = 0);

    ConstantBuffer(uint16 index, uint16 space);
    ConstantBuffer(const ConstantBuffer& other);
    ConstantBuffer(ConstantBuffer&& other);
    ~ConstantBuffer();

    ConstantBuffer& operator= (ConstantBuffer other);

    eReturnCode Add(const std::string& name, float32 data, bool queueForUpdate = true);
    eReturnCode Add(const std::string& name, const Vector2& data, bool queueForUpdate = true);
    eReturnCode Add(const std::string& name, const Vector3& data, bool queueForUpdate = true);
    eReturnCode Add(const std::string& name, const Vector4& data, bool queueForUpdate = true);
    eReturnCode Add(const std::string& name, const Matrix3& data, bool queueForUpdate = true);
    eReturnCode Add(const std::string& name, const Matrix4& data, bool queueForUpdate = true);

    eReturnCode Set(const std::string& name, float32 data, bool queueForUpdate = true);
    eReturnCode Set(const std::string& name, const Vector2& data, bool queueForUpdate = true);
    eReturnCode Set(const std::string& name, const Vector3& data, bool queueForUpdate = true);
    eReturnCode Set(const std::string& name, const Vector4& data, bool queueForUpdate = true);
    eReturnCode Set(const std::string& name, const Matrix3& data, bool queueForUpdate = true);
    eReturnCode Set(const std::string& name, const Matrix4& data, bool queueForUpdate = true);

    void ComposeBufferData();
    float32* GetBufferData();
    uint32 GetDataSize() const;
    bool GetIsComposed() const;
    ConstantBufferHandle GetHandle() const;
    void SetHandle(ConstantBufferHandle handle);
    void ScheduleToUpdate();

    uint16 GetIndex() const;
    uint16 GetSpace() const;
    uint32 GetKey() const;

    void MakeShallowCopy(ConstantBuffer& target, bool queueForUpdate = true) const; // [a_vorontcov] Copies params, space and key.

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
    bool m_regenerateMemLayout = true;

    std::vector<Param> m_params;
    // [a_vorontcov] TODO: template class with typename m_memData.
    float32* m_memData = nullptr;
    uint32 m_dataSize = 0;
    uint32 m_dataSize4ByteElem = 0;

    ConstantBufferHandle m_handle;

    friend void swap(ConstantBuffer& l, ConstantBuffer& r)
    {
        std::swap(l.m_index, r.m_index);
        std::swap(l.m_space, r.m_space);
        std::swap(l.m_key, r.m_key);
        std::swap(l.m_regenerateMemLayout, r.m_regenerateMemLayout);
        std::swap(l.m_memData, r.m_memData);
        std::swap(l.m_dataSize, r.m_dataSize);
        std::swap(l.m_dataSize4ByteElem, r.m_dataSize4ByteElem);
        l.m_params.swap(r.m_params);
        std::swap(l.m_handle, r.m_handle);
    }
};

template <typename T>
inline ConstantBuffer::ConstantBuffer(uint16 index, uint16 space, uint16 elemNum)
    : m_index(index), m_space(space), m_key(m_index | m_space << 16), m_dataSize(sizeof(T) * elemNum)
{
    m_memData = new float32[m_dataSize / sizeof(float32)];
}

template <typename T>
inline void ConstantBuffer::Set(const T& val, uint16 elemOffset)
{
    T* mem = reinterpret_cast<T*>(m_memData);
    *(mem + elemOffset) = val;
}

template <typename T>
inline T* ConstantBuffer::Get(uint16 elemOffset)
{
    T* mem = reinterpret_cast<T*>(m_memData);
    return mem + elemOffset;
}

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

inline bool ConstantBuffer::GetIsComposed() const
{
    return !m_regenerateMemLayout;
}

inline ConstantBufferHandle ConstantBuffer::GetHandle() const
{
    return m_handle;
}

inline void ConstantBuffer::SetHandle(ConstantBufferHandle handle)
{
    m_handle = handle;
}
}