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

class RtNodeGraphData : public RtCompoundElementData
{
public:
    RtNodeGraphData(const RtToken& name);

    static RtDataHandle create(const RtToken& name);

    void addNode(RtDataHandle node);

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
    inline RtNodeData* node(size_t index) { return (RtNodeData*)getElement(index).get(); }
    inline RtNodeData* inputs() { return (RtNodeData*)_inputs.get(); }
    inline RtNodeData* outputs() { return (RtNodeData*)_outputs.get(); }

    RtDataHandle _inputs;
    RtDataHandle _inputsDef;
    RtDataHandle _outputs;
    RtDataHandle _outputsDef;
};

}

#endif
