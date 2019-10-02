//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTNODEGRAPHDATA_H
#define MATERIALX_RTNODEGRAPHDATA_H

#include <MaterialXRuntime/private/RtElementData.h>
#include <MaterialXRuntime/private/RtNodeData.h>

#include <MaterialXRuntime/RtObject.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class RtNodeGraphData : public RtElementData
{
public:
    RtNodeGraphData(const RtToken& name);

    static RtDataHandle create(const RtToken& name);

    void addNode(RtDataHandle node);

    RtDataHandle getNode(const RtToken& name) const
    {
        auto it = _nodesByName.find(name);
        return it != _nodesByName.end() ? _nodes[it->second] : nullptr;
    }

    RtDataHandle getNode(size_t index) const
    {
        return index < _nodes.size() ? _nodes[index] : nullptr;
    }

    size_t numNodes() const
    {
        return _nodes.size();
    }

    size_t getNodeIndex(const RtToken& name) const
    {
        auto it = _nodesByName.find(name);
        return it != _nodesByName.end() ? it->second : INVALID_INDEX;
    }

    void setInterface(RtDataHandle nodedef);

    RtDataHandle getInputInterface() const
    {
        return _inputs;
    }

    RtDataHandle getOutputInterface() const
    {
        return _outputs;
    }

protected:
    // Short syntax getter for convenience.
    inline RtNodeData* node(size_t index) { return (RtNodeData*)getNode(index).get(); }
    inline RtNodeData* inputs() { return (RtNodeData*)_inputs.get(); }
    inline RtNodeData* outputs() { return (RtNodeData*)_outputs.get(); }

    RtDataHandleArray _nodes;
    RtTokenIndexMap _nodesByName;

    RtDataHandle _inputs;
    RtDataHandle _inputsDef;
    RtDataHandle _outputs;
    RtDataHandle _outputsDef;
};

}

#endif
