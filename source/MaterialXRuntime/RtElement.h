//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTELEMENT_H
#define MATERIALX_RTELEMENT_H

/// @file
/// TODO: Docs

#include <MaterialXRuntime/RtObject.h>
#include <MaterialXRuntime/RtValue.h>

namespace MaterialX
{

/// @class RtAttribute
/// TODO: Docs
class RtAttribute
{
public:
    const RtToken& getName() const
    {
        return _name;
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

private:
    RtAttribute(const RtToken& name, const RtToken& type, const RtValue& value) :
        _name(name),
        _type(type),
        _value(value)
    {}

    RtToken _name;
    RtToken _type;
    RtValue _value;
    friend class RtElementData;
};


/// @class RtElement
/// TODO: Docs
class RtElement : public RtApiBase
{
public:
    /// Constructor attaching an object to the API.
    RtElement(const RtObject& obj);

    /// Return the type for this API.
    RtApiType getApiType() const override;

    /// Get element name.
    const RtToken& getName() const;

    /// Add an attribute.
    void addAttribute(const RtToken& name, const RtToken& type, const RtValue& value);

    /// Return an attribute by name, or a nullptr
    /// if no such attribute exists.
    const RtAttribute* getAttribute(const RtToken& name) const;

    /// Return an attribute by name, or a nullptr
    /// if no such attribute exists.
    RtAttribute* getAttribute(const RtToken& name);

    /// Return an attribute by index, or a nullptr
    /// if no such attribute exists.
    const RtAttribute* getAttribute(size_t index) const;

    /// Return an attribute by name, or a nullptr
    /// if no such attribute exists.
    RtAttribute* getAttribute(size_t index);

    /// Return the attribute count.
    size_t numAttributes() const;
};

}

#endif
