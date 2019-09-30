//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RT_ELEMENT_H
#define MATERIALX_RT_ELEMENT_H

/// @file
/// TODO: Docs

#include <MaterialXRuntime/RtObject.h>
#include <MaterialXRuntime/RtValue.h>

namespace MaterialX
{

/// @class RtElement
/// TODO: Docs
class RtElement : public RtApiBase
{
public:
    /// Constructor attaching an object to the API.
    RtElement(const RtObject& obj);

    /// Return the type for this object.
    RtApiType getApiType() const override;

    /// Query if the given object type is supported by this API.
    bool isSupported(RtObjType type) const override;

    /// Get element name.
    const RtToken& getName() const;

    /// Add an attribute.
    void addAttribute(RtObject attr);

    /// Return an attribute by name, or a null object
    /// if no such attribute exists.
    RtObject getAttribute(const RtToken& name) const;

    /// Return the attribute count.
    size_t numAttributes() const;
};

}

#endif
