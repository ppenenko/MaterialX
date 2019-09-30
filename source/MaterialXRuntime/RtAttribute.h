//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RT_ATTRIBUTE_H
#define MATERIALX_RT_ATTRIBUTE_H

/// @file
/// TODO: Docs

#include <MaterialXRuntime/RtElement.h>

namespace MaterialX
{

class RtValue;

/// @class RtAttribute
/// TODO: Docs
class RtAttrFlag
{
public:
    static const uint32_t INPUT       = 0x00000001;
    static const uint32_t OUTPUT      = 0x00000002;
    static const uint32_t CONNECTABLE = 0x00000004;
    static const uint32_t UNIFORM     = 0x00000008;
};

/// @class RtAttribute
/// TODO: Docs
class RtAttribute : public RtElement
{
public:
    /// Constructor attaching and object to the API.
    RtAttribute(const RtObject& obj);

    /// Return the type for this API.
    RtApiType getApiType() const override;

    /// Return the data type for this attribute.
    const RtToken& getType() const;

    /// Return the value for this attribute.
    const RtValue& getValue() const;

    /// Return the value for this attribute.
    RtValue& getValue();

    /// Set a new value on the attribute.
    void setValue(const RtValue& v);
    void setValue(bool v);
    void setValue(int v);
    void setValue(unsigned int v);
    void setValue(float v);
    void setValue(const Color3& v);
    void setValue(const Vector4& v);
    void setValue(void* v);

    /// Return the flags for this attribute.
    int32_t getFlags() const;

    /// Return true if this is an input attribute.
    bool isInput() const;

    /// Return true if this is an output attribute.
    bool isOutput() const;

    /// Return true if this attribute is connectable.
    bool isConnectable() const;

    /// Create a new attribute
    static RtObject create(const RtToken& name, const RtToken& type, const RtValue& value, uint32_t flags = 0);
};

}

#endif
