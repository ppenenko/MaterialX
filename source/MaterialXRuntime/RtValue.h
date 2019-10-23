//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTVALUE_H
#define MATERIALX_RTVALUE_H

/// @file
/// TODO: Docs

#include <MaterialXRuntime/RtToken.h>

#include <MaterialXCore/Value.h>

namespace MaterialX
{

/// @class RtValue
/// TODO: Docs
class RtValue
{
public:
    /// Default constructor
    RtValue() : _data{0,0} {}

    /// Explicit value constructor
    explicit RtValue(bool v) { asBool() = v; }
    explicit RtValue(int v) { asInt() = v; }
    explicit RtValue(float v) { asFloat() = v; }
    explicit RtValue(const Color3& v) { asColor3() = v; }
    explicit RtValue(const Vector4& v) { asVector4() = v; }
    explicit RtValue(void* v) { asPtr() = v; }
    explicit RtValue(const RtToken& v) { asToken() = v; }
    explicit RtValue(const ValuePtr& v);

    /// Return bool value.
    const bool& asBool() const
    {
        return *reinterpret_cast<const bool*>(&_data);
    }
    /// Return reference to bool value.
    bool& asBool()
    {
        return *reinterpret_cast<bool*>(&_data);
    }

    /// Return int value.
    int asInt() const
    {
        return *reinterpret_cast<const int*>(&_data);
    }
    /// Return reference to int value.
    int& asInt()
    {
        return *reinterpret_cast<int*>(&_data);
    }

    /// Return float value.
    float asFloat() const
    {
        return *reinterpret_cast<const float*>(&_data);
    }
    /// Return reference to float value.
    float& asFloat()
    {
        return *reinterpret_cast<float*>(&_data);
    }

    /// Return reference to token value.
    const RtToken& asToken() const
    {
        if (_data[0] == 0 && _data[1] == 0)
        {
            return EMPTY_TOKEN;
        }
        return *reinterpret_cast<const RtToken*>(&_data);
    }
    /// Return reference to token value.
    RtToken& asToken()
    {
        return *reinterpret_cast<RtToken*>(&_data);
    }

    /// Return Color2 value.
    const Color2& asColor2() const
    {
        return *reinterpret_cast<const Color2*>(&_data);
    }
    /// Return reference to Color2 value.
    Color2& asColor2()
    {
        return *reinterpret_cast<Color2*>(&_data);
    }

    /// Return Color3 value.
    const Color3& asColor3() const
    {
        return *reinterpret_cast<const Color3*>(&_data);
    }
    /// Return reference to Color3 value.
    Color3& asColor3()
    {
        return *reinterpret_cast<Color3*>(&_data);
    }

    /// Return Color4 value.
    const Color4& asColor4() const
    {
        return *reinterpret_cast<const Color4*>(&_data);
    }
    /// Return reference to Color4 value.
    Color4& asColor4()
    {
        return *reinterpret_cast<Color4*>(&_data);
    }

    /// Return Vector2 value.
    const Vector2& asVector2() const
    {
        return *reinterpret_cast<const Vector2*>(&_data);
    }
    /// Return reference to Vector2 value.
    Vector2& asVector2()
    {
        return *reinterpret_cast<Vector2*>(&_data);
    }

    /// Return Vector3 value.
    const Vector3& asVector3() const
    {
        return *reinterpret_cast<const Vector3*>(&_data);
    }
    /// Return reference to Vector3 value.
    Vector3& asVector3()
    {
        return *reinterpret_cast<Vector3*>(&_data);
    }

    /// Return Vector4 value.
    const Vector4& asVector4() const
    {
        return *reinterpret_cast<const Vector4*>(&_data);
    }
    /// Return reference to Vector4 value.
    Vector4& asVector4()
    {
        return *reinterpret_cast<Vector4*>(&_data);
    }

    /// Return const pointer.
    void* const& asPtr() const
    {
        return *reinterpret_cast<void* const*>(&_data);
    }
    /// Return pointer.
    void* & asPtr()
    {
        return *reinterpret_cast<void**>(&_data);
    }

    /// Return const pointer of template argument type.
    template<typename T>
    const T& asA() const
    {
        return *reinterpret_cast<const T*>(&_data);
    }
    /// Return pointer of template argument type.
    template<typename T>
    T& asA()
    {
        return *reinterpret_cast<T*>(&_data);
    }

    /// Clear the value with zeores.
    void clear()
    {
        _data[0] = _data[1] = 0;
    }

    bool operator==(const RtValue& other)
    {
        return _data[0] == other._data[0] && _data[1] == other._data[1];
    }

    bool operator!=(const RtValue& other)
    {
        return !(*this==other);
    }

    /// Return a string representing the value
    /// in the given type.
    string getValueString(const RtToken& type) const;

private:
    // 16 bytes of data storage to hold the main data types,
    // up to four component vector/color. Larger data types 
    // needs heap allocation.
    // Storage is aligned to 64-bit to hold pointers for 
    // heap allocated data types as well as other pointers.
    uint64_t _data[2];
};

}

#endif
