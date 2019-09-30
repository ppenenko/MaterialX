//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RT_ATTRIBUTEDATA_H
#define MATERIALX_RT_ATTRIBUTEDATA_H

#include <MaterialXRuntime/private/RtElementData.h>

#include <MaterialXRuntime/RtObject.h>
#include <MaterialXRuntime/RtAttribute.h>
#include <MaterialXRuntime/RtValue.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class RtAttributeData : public RtElementData
{
public:
    RtAttributeData() :
        RtElementData(RtObjType::ATTRIBUTE),
        _flags(0)
    {
    }

    RtAttributeData(const RtToken& name, const RtToken& type, const RtValue& value, uint32_t flags) :
        RtElementData(RtObjType::ATTRIBUTE, name),
        _type(type),
        _value(value),
        _flags(flags)
    {
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

    bool isConnectableTo(const RtAttributeData* other) const
    {
        // TODO: Optimize using direct bit matching
        return (isConnectable() && other->isConnectable() &&
            (isInput() && other->isOutput() || isOutput() && other->isInput()));
    }

    static RtDataHandle create(const RtToken& name, const RtToken& type, const RtValue& value, uint32_t flags)
    {
        return std::make_shared<RtAttributeData>(name, type, value, flags);
    }

protected:
    RtToken _type;
    RtValue _value;
    uint32_t _flags;
};

}

#endif
