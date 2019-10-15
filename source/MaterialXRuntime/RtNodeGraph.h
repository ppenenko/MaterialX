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

    /// Create a new nodegraph and add it to a parent if specified.
    /// The parent must be a stage object.
    static RtObject create(const RtToken& name, RtObject parent = RtObject());

    /// Return the type for this API.
    RtApiType getApiType() const override;

    /// Add a node to the graph.
    void addNode(RtObject node);

    /// Return the node count.
    size_t numNodes() const;

    /// Return a node by index, or a null object 
    /// if no such node exists.
    RtObject getNode(size_t index) const;

    /// Find a node by name. Return a null object 
    /// if no such node is found.
    RtObject findNode(const RtToken& name) const;

    /// Set the interface for the graph, creating inputs and outputs 
    /// according to the given nodedef.
    void setInterface(RtObject nodedef);

    /// Return the internal node holding the input interface.
    RtObject getInputsNode() const;

    /// Return the internal node holding the output interface.
    RtObject getOutputsNode() const;

    /// Convert this graph to a string in the DOT language syntax. This can be
    /// used to visualise the graph using GraphViz (http://www.graphviz.org).
    string asStringDot() const;
};

}

#endif
