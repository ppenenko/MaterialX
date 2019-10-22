//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_PRVNODEGRAPH_H
#define MATERIALX_PRVNODEGRAPH_H

#include <MaterialXRuntime/Private/PrvElement.h>
#include <MaterialXRuntime/Private/PrvNode.h>

#include <MaterialXRuntime/RtObject.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class PrvNodeGraph : public PrvCompoundElement
{
public:
    PrvNodeGraph(const RtToken& name);

    static PrvObjectHandle createNew(const RtToken& name);

    void addNode(PrvObjectHandle node);

    size_t numNodes() const
    {
        return numElements();
    }

    void setInterface(PrvObjectHandle nodedef);

    PrvObjectHandle getInputsNode() const
    {
        return _inputsNode;
    }

    PrvObjectHandle getOutputsNode() const
    {
        return _outputsNode;
    }

    string asStringDot() const;

    // Short syntax getter for convenience.
    PrvNode* node(const RtToken& name) const { return (PrvNode*)findElementByName(name).get(); }
    PrvNode* node(size_t index) const { return (PrvNode*)getElement(index).get(); }
    PrvNode* inputsNode() const { return (PrvNode*)_inputsNode.get(); }
    PrvNode* outputsNode() const { return (PrvNode*)_outputsNode.get(); }

    // Token constants.
    static const RtToken INPUTS;
    static const RtToken OUTPUTS;
    static const RtToken NODEGRAPH_INPUTS;
    static const RtToken NODEGRAPH_OUTPUTS;

protected:
    PrvObjectHandle _inputsNode;
    PrvObjectHandle _inputsDef;
    PrvObjectHandle _outputsNode;
    PrvObjectHandle _outputsDef;
};

}

#endif
