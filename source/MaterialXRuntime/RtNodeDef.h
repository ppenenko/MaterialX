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

    /// Add a new input attribute.
    RtAttribute addInputAttr(const RtToken& name, const RtToken& type,
                         const RtValue& value = RtValue(), bool connectable = true);

    /// Add a new output attribute.
    RtAttribute addOutputAttr(const RtToken& name, const RtToken& type,
                          const RtValue& value = RtValue(), bool connectable = true);

    /// Return an input attribute by name, or a null object
    /// if no such input attribute exists.
    RtAttribute getInputAttr(const RtToken& name) const;

    /// Return an output attribute by name, or a null object
    /// if no such output attribute exists.
    RtAttribute getOutputAttr(const RtToken& name) const;
};

}

#endif
