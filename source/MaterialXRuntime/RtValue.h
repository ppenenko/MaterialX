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
    explicit RtValue(unsigned int v) { asUInt() = v; }
    explicit RtValue(size_t v) { asSize() = v; }
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

    /// Return unsigned int value.
    unsigned int asUInt() const
    {
        return *reinterpret_cast<const unsigned int*>(&_data);
    }
    /// Return reference to unsigned int value.
    unsigned int& asUInt()
    {
        return *reinterpret_cast<unsigned int*>(&_data);
    }

    /// Return size_t value.
    size_t asSize() const
    {
        return *reinterpret_cast<const size_t*>(&_data);
    }
    /// Return reference to size_t value.
    size_t& asSize()
    {
        return *reinterpret_cast<size_t*>(&_data);
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
        return *reinterpret_cast<const RtToken*>(&_data);
    }
    /// Return reference to token value.
    RtToken& asToken()
    {
        return *reinterpret_cast<RtToken*>(&_data);
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
