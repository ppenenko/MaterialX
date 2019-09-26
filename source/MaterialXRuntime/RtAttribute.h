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
class RtAttribute : public RtElement
{
public:
    /// Constructor attaching and object to the API.
    RtAttribute(const RtObject& obj);

    /// Return the type for this object.
    RtApiType getApiType() const override;

    /// Return the data type for this attribute.
    const RtToken& getType() const;

    /// Return the value for this attribute.
    const RtValue& getValue() const;

    /// Return the value for this attribute.
    RtValue& getValue();

    /// Set the value for this attribute.
    void setValue(const RtValue& v);

    /// Set the value for this attribute.
    void setValue(bool v);
    void setValue(int v);
    void setValue(unsigned int v);
    void setValue(float v);
    void setValue(const Color3& v);
    void setValue(const Vector4& v);
    void setValue(void* v);

    /// Return true if this is an input attribute.
    bool isInput() const;

    /// Return true if this is an output attribute.
    bool isOutput() const;

    /// Return true if this attribute is connectable.
    bool isConnectable() const;
};

}

#endif
