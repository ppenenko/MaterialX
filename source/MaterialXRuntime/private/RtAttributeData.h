//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RT_ATTRIBUTEDATA_H
#define MATERIALX_RT_ATTRIBUTEDATA_H

#include <MaterialXRuntime/private/RtElementData.h>

#include <MaterialXRuntime/RtObject.h>
#include <MaterialXRuntime/RtValue.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class RtAttrFlag
{
public:
    static const uint32_t INPUT       = 0x00000001;
    static const uint32_t OUTPUT      = 0x00000002;
    static const uint32_t CONNECTABLE = 0x00000004;
};

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

    void setType(const RtToken& type)
    {
        _type = type;
    }

    const RtValue& getValue() const
    {
        return _value;
    }

    RtValue& getValue()
    {
        return _value;
    }

    void setValue(const RtValue& value)
    {
        _value = value;
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
    RtToken _type;
    RtValue _value;
    uint32_t _flags;
};

}

#endif
