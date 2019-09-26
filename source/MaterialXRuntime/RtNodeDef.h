//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RT_NODEDEF_H
#define MATERIALX_RT_NODEDEF_H

/// @file
/// TODO: Docs

#include <MaterialXRuntime/RtElement.h>
#include <MaterialXRuntime/RtAttribute.h>

namespace MaterialX
{

class RtAttribute;

/// @class RtNodeDef API
/// TODO: Docs
class RtNodeDef : public RtElement
{
public:
    /// Constructor attaching and object to the API.
    RtNodeDef(const RtObject& obj);

    /// Return the type for this object.
    RtApiType getApiType() const override;

    /// Return the node category.
    const RtToken& getCategory() const;

    /// Add a new attribute.
    RtAttribute addAttribute(const RtToken& name, const RtToken& type,
                             uint32_t flags = RtAttrFlag::INPUT | RtAttrFlag::CONNECTABLE,
                             const RtValue& value = RtValue());

    /// Return an attribute by name, or a null object
    /// if no such attribute exists.
    RtAttribute getAttribute(const RtToken& name) const;
};

}

#endif
