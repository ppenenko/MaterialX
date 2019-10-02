//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTNODEDEF_H
#define MATERIALX_RTNODEDEF_H

/// @file
/// TODO: Docs

#include <MaterialXRuntime/RtElement.h>

namespace MaterialX
{

/// @class RtNodeDef API
/// TODO: Docs
class RtNodeDef : public RtElement
{
public:
    /// Constructor attaching and object to the API.
    RtNodeDef(const RtObject& obj);

    /// Create a new nodedef on the given stage.
    static RtObject create(const RtToken& name, const RtToken& category, RtObject stage);

    /// Return the type for this object.
    RtApiType getApiType() const override;

    /// Return the node category.
    const RtToken& getCategory() const;

    /// Add an attribute defining a port.
    void addPortDef(RtObject attr);

    /// Return a port attribute by name,
    /// or a null object if no such port exists.
    RtObject getPortDef(const RtToken& name) const;

    /// Return a port attribute by index,
    /// or a null object if no such port exists.
    RtObject getPortDef(size_t index) const;

    /// Return the port count.
    size_t numPorts() const;
};

}

#endif
