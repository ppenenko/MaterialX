//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RT_ATTRIBUTEDATA_H
#define MATERIALX_RT_ATTRIBUTEDATA_H

#include <MaterialXRuntime/private/RtObjectData.h>

#include <MaterialXRuntime/RtObject.h>
#include <MaterialXRuntime/RtAttribute.h>
#include <MaterialXRuntime/RtValue.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class RtAttributeData : public RtObjectData
{
public:
    RtAttributeData();
    RtAttributeData(const RtToken& name, const RtToken& type, const RtValue& value, uint32_t flags);

    static RtDataHandle create(const RtToken& name, const RtToken& type, const RtValue& value, uint32_t flags);

    const RtToken& getName() const
    {
        return _name;
    }

    void setName(const RtToken& name)
    {
        _name = name;
    }

    const RtToken& getType() const
    {
        return _type;
    }

    const RtValue& getValue() const
    {
        return _value;
    }

    RtValue& getValue()
    {
        return _value;
    }

    void setValue(const RtValue& v)
    {
        _value = v;
    }

    void setValue(bool v)
    {
        _value.asBool() = v;
    }

    void setValue(int v)
    {
        _value.asInt() = v;
    }

    void setValue(unsigned int v)
    {
        _value.asUInt() = v;
    }

    void setValue(float v)
    {
        _value.asFloat() = v;
    }

    void setValue(const Color3& v)
    {
        _value.asColor3() = v;
    }

    void setValue(const Vector4& v)
    {
        _value.asVector4() = v;
    }

    void setValue(void* v)
    {
        _value.asPtr() = v;
    }

    uint32_t getFlags() const
    {
        return _flags;
    }

    bool isInput() const
    {
        return _flags & RtAttrFlag::INPUT;
    }

    bool isOutput() const
    {
        return _flags & RtAttrFlag::OUTPUT;
    }

    bool isConnectable() const
    {
        return _flags & RtAttrFlag::CONNECTABLE;
    }

protected:
    RtToken _name;
    RtToken _type;
    RtValue _value;
    uint32_t _flags;
};

}

#endif
