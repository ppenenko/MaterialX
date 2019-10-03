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
    static RtObject create(const RtToken& name, const RtToken& category, RtObject stage = RtObject());

    /// Return the type for this API.
    RtApiType getApiType() const override;

    /// Return the node category.
    const RtToken& getCategory() const;

    /// Add an port definition.
    void addPortDef(RtObject attr);

    /// Return a portdef by name,
    /// or a null object if no such port exists.
    RtObject getPortDef(const RtToken& name) const;

    /// Return a portdef by index,
    /// or a null object if no such port exists.
    RtObject getPortDef(size_t index) const;

    /// Return the port count.
    size_t numPorts() const;

    /// Return the port index for a given port name
    /// or INVALID_INDEX if no such port is found.
    size_t getPortIndex(const RtToken& name) const;
};

}

#endif
