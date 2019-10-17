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

    /// Create a new nodegraph and add it to a parent if specified.
    /// The parent must be a stage object.
    static RtObject create(const RtToken& name, const RtToken& category, RtObject parent = RtObject());

    /// Return the type for this API.
    RtApiType getApiType() const override;

    /// Return the node category.
    const RtToken& getCategory() const;

    /// Add an port definition.
    void addPort(RtObject portdef);

    /// Return the port count.
    size_t numPorts() const;

    /// Return the output count.
    size_t numOutputs() const;

    /// Return a port definition by index,
    /// or a null object if no such port exists.
    RtObject getPort(size_t index) const;

    /// Find a port definition by name.
    /// Return a null object if no such port is found.
    RtObject findPort(const RtToken& name) const;
};

}

#endif
