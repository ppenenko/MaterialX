//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/private/RtNodeGraphData.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

RtNodeGraphData::RtNodeGraphData(const RtToken& name) :
    RtCompoundElementData(RtObjType::NODEGRAPH, name)
{
}

RtDataHandle RtNodeGraphData::create(const RtToken& name)
{
    return std::make_shared<RtNodeGraphData>(name);
}

void RtNodeGraphData::addNode(RtDataHandle node)
{
    if (!node->hasApi(RtApiType::NODE))
    {
        throw ExceptionRuntimeError("Given object is not a valid node");
    }
    RtNodeData* n = node->asA<RtNodeData>();
    auto it = _elementsByName.find(n->getName());
    if (it != _elementsByName.end())
    {
        throw ExceptionRuntimeError("A node named '" + n->getName() + "' already exists for nodegraph '" + getName() + "'");
    }
    _elementsByName[n->getName()] = _elements.size();
    _elements.push_back(node);
}

void RtNodeGraphData::setInterface(RtDataHandle nodedef)
{
    if (!nodedef->hasApi(RtApiType::NODEDEF))
    {
        throw ExceptionRuntimeError("Given object is not a valid nodedef");
    }

    RtNodeDefData* nd = nodedef->asA<RtNodeDefData>();

    // Create nodedefs for the input and output interface nodes
    _inputsDef = RtNodeDefData::create("inputs", "nodegraphinputs");
    _outputsDef = RtNodeDefData::create("outputs", "nodegraphoutputs");

    RtNodeDefData* def;
    for (size_t i = 0; i < nd->numElements(); ++i)
    {
        const RtPortDefData* pd = nd->portdef(i);
        uint32_t flags = pd->getFlags();
        if (pd->isInput())
        {
            // And interface input turns into an output on the inputs node
            flags &= ~RtPortFlag::INPUT;
            flags |= RtPortFlag::OUTPUT | RtPortFlag::CONNECTABLE | RtPortFlag::INTERFACE;
            def = _inputsDef->asA<RtNodeDefData>();
        }
        else
        {
            // And interface output turns into an input on the outputs node
            flags &= ~RtPortFlag::OUTPUT;
            flags |= RtPortFlag::INPUT | RtPortFlag::CONNECTABLE | RtPortFlag::INTERFACE;
            def = _outputsDef->asA<RtNodeDefData>();
        }
        RtDataHandle port = RtPortDefData::create(pd->getName(), pd->getType(), pd->getValue(), flags);
        def->addPortDef(port);
    }

    // Instantiate the input and output interface nodes
    _inputs = RtNodeData::create("inputs", _inputsDef);
    _outputs = RtNodeData::create("inputs", _outputsDef);
}

}
