//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTNODEGRAPH_H
#define MATERIALX_RTNODEGRAPH_H

/// @file
/// TODO: Docs

#include <MaterialXRuntime/RtElement.h>

namespace MaterialX
{

/// @class RtNodeGraph
/// TODO: Docs
class RtNodeGraph : public RtElement
{
public:
    /// Constructor attaching and object to the API.
    RtNodeGraph(const RtObject& obj);

    /// Create a new nodegraph on the given stage.
    static RtObject create(const RtToken& name, RtObject stage);

    /// Return the type for this object.
    RtApiType getApiType() const override;

    /// Add a node to the graph.
    void addNode(RtObject node);

    /// Return a node by name, or a null object 
    /// if no such node exists.
    RtObject getNode(const RtToken& name) const;

    /// Return a node by index, or a null object 
    /// if no such node exists.
    RtObject getNode(size_t index) const;

    /// Return the node count.
    size_t numNodes() const;

    void setInterface(RtObject nodedef);

    RtObject getInputInterface() const;

    RtObject getOutputInterface() const;
};

}

#endif
