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
#include <MaterialXRuntime/RtTraversal.h>

namespace MaterialX
{

/// @class RtAttribute
/// TODO: Docs
class RtAttribute
{
public:
    /// Get attribute name.
    const RtToken& getName() const
    {
        return _name;
    }

    /// Get attribute type.
    const RtToken& getType() const
    {
        return _type;
    }

    /// Get attribute value.
    const RtValue& getValue() const
    {
        return _value;
    }

    /// Get attribute value.
    RtValue& getValue()
    {
        return _value;
    }

    /// Set attribute value.
    void setValue(const RtValue& v)
    {
        _value = v;
    }

    /// Set attribute value as a bool.
    void setValue(bool v)
    {
        _value.asBool() = v;
    }

    /// Set attribute value as a bool.
    void setValue(int v)
    {
        _value.asInt() = v;
    }

    /// Set attribute value as a bool.
    void setValue(float v)
    {
        _value.asFloat() = v;
    }

    /// Set attribute value as a Color2.
    void setValue(const Color2& v)
    {
        _value.asColor2() = v;
    }

    /// Set attribute value as a Color3.
    void setValue(const Color3& v)
    {
        _value.asColor3() = v;
    }

    /// Set attribute value as a Color4.
    void setValue(const Color4& v)
    {
        _value.asColor4() = v;
    }

    /// Set attribute value as a Vector2.
    void setValue(const Vector2& v)
    {
        _value.asVector2() = v;
    }

    /// Set attribute value as a Vector3.
    void setValue(const Vector3& v)
    {
        _value.asVector3() = v;
    }

    /// Set attribute value as a Vector4.
    void setValue(const Vector4& v)
    {
        _value.asVector4() = v;
    }

    /// Set attribute value as a token.
    void setValue(const RtToken& v)
    {
        _value.asToken() = v;
    }

    /// Set attribute value as a raw pointer.
    void setValue(void* v)
    {
        _value.asPtr() = v;
    }

private:
    /// Private constructor.
    RtAttribute(const RtToken& name, const RtToken& type, const RtValue& value) :
        _name(name),
        _type(type),
        _value(value)
    {}

    RtToken _name;
    RtToken _type;
    RtValue _value;
    friend class PrvElement;
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

    /// Return an iterator traversing all children
    /// of this element.
    /// If a filter is set it will be called to restrict
    /// which objects to return.
    RtTreeIterator traverseTree(RtTraversalFilter filter = nullptr);
};

}

#endif
