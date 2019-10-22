//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTPORTDEFDATA_H
#define MATERIALX_RTPORTDEFDATA_H

#include <MaterialXRuntime/Private/PrvElement.h>

#include <MaterialXRuntime/RtObject.h>
#include <MaterialXRuntime/RtPortDef.h>
#include <MaterialXRuntime/RtValue.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class PrvPortDef : public PrvElement
{
public:
    PrvPortDef(const RtToken& name, const RtToken& type, const RtValue& value, uint32_t flags);

    static PrvObjectHandle createNew(const RtToken& name, const RtToken& type, const RtValue& value, uint32_t flags);

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

    const RtToken& getColorSpace() const
    {
        return _colorspace;
    }

    const RtToken& getUnit() const
    {
        return _unit;
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

    void setColorSpace(const RtToken& colorspace)
    {
        _colorspace = colorspace;
    }

    void setUnit(const RtToken& unit)
    {
        _unit = unit;
    }

    uint32_t getFlags() const
    {
        return _flags;
    }

    bool isInput() const
    {
        return _flags & RtPortFlag::INPUT;
    }

    bool isOutput() const
    {
        return _flags & RtPortFlag::OUTPUT;
    }

    bool isConnectable() const
    {
        return !(_flags & RtPortFlag::UNCONNECTABLE);
    }

    bool isUniform() const
    {
        return _flags & RtPortFlag::UNIFORM;
    }

    static const RtToken DEFAULT_OUTPUT_NAME;

protected:
    RtToken _type;
    RtValue _value;
    RtToken _colorspace;
    RtToken _unit;
    uint32_t _flags;
};

}

#endif
