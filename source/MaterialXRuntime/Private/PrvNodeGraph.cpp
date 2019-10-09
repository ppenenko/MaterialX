//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/Private/PrvNodeGraph.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

PrvNodeGraph::PrvNodeGraph(const RtToken& name) :
    PrvCompoundElement(RtObjType::NODEGRAPH, name)
{
}

PrvObjectHandle PrvNodeGraph::create(const RtToken& name)
{
    return std::make_shared<PrvNodeGraph>(name);
}

void PrvNodeGraph::addNode(PrvObjectHandle node)
{
    if (!node->hasApi(RtApiType::NODE))
    {
        throw ExceptionRuntimeError("Given object is not a valid node");
    }
    PrvNode* n = node->asA<PrvNode>();
    auto it = _elementsByName.find(n->getName());
    if (it != _elementsByName.end())
    {
        throw ExceptionRuntimeError("A node named '" + n->getName() + "' already exists for nodegraph '" + getName() + "'");
    }
    _elementsByName[n->getName()] = _elements.size();
    _elements.push_back(node);
}

void PrvNodeGraph::setInterface(PrvObjectHandle nodedef)
{
    if (!nodedef->hasApi(RtApiType::NODEDEF))
    {
        throw ExceptionRuntimeError("Given object is not a valid nodedef");
    }

    PrvNodeDef* nd = nodedef->asA<PrvNodeDef>();

    // Create nodedefs for the input and output interface nodes.
    _inputsDef = PrvNodeDef::create("inputs", "nodegraphinputs");
    _outputsDef = PrvNodeDef::create("outputs", "nodegraphoutputs");

    PrvNodeDef* def;
    for (size_t i = 0; i < nd->numElements(); ++i)
    {
        const PrvPortDef* pd = nd->portdef(i);
        uint32_t flags = pd->getFlags();
        if (pd->isInput())
        {
            // And interface input turns into an output on the inputs node.
            flags &= ~RtPortFlag::INPUT;
            flags |= RtPortFlag::OUTPUT | RtPortFlag::CONNECTABLE | RtPortFlag::INTERFACE;
            def = _inputsDef->asA<PrvNodeDef>();
        }
        else
        {
            // And interface output turns into an input on the outputs node.
            flags &= ~RtPortFlag::OUTPUT;
            flags |= RtPortFlag::INPUT | RtPortFlag::CONNECTABLE | RtPortFlag::INTERFACE;
            def = _outputsDef->asA<PrvNodeDef>();
        }
        PrvObjectHandle port = PrvPortDef::create(pd->getName(), pd->getType(), pd->getValue(), flags);
        def->addPortDef(port);
    }

    // Instantiate the input and output interface nodes.
    _inputsNode = PrvNode::create("inputs", _inputsDef);
    _outputsNode = PrvNode::create("outputs", _outputsDef);
}

RtString PrvNodeGraph::asStringDot() const
{
    string dot = "digraph {\n";

    // Add alla nodes.
    PrvNode* inputs = inputsNode();
    dot += "    \"" + inputs->getName() + "\" ";
    dot += "[shape=box];\n";
    PrvNode* outputs = outputsNode();
    dot += "    \"" + outputs->getName() + "\" ";
    dot += "[shape=box];\n";

    // Add alla nodes.
    for (size_t i = 0; i < numNodes(); ++i)
    {
        dot += "    \"" + node(i)->getName() + "\" ";
        dot += "[shape=ellipse];\n";
    }

    auto writeConnections = [](PrvNode* n, RtString& dot)
    {
        for (size_t j = 0; j < n->numPorts(); ++j)
        {
            RtPort p = n->getPort(j);
            if (p.isInput() && p.isConnected())
            {
                RtPort src = p.getSourcePort();
                RtNode srcNode(src.getNode());
                dot += "    \"" + srcNode.getName();
                dot += "\" -> \"" + n->getName();
                dot += "\" [label=\"" + p.getName() + "\"];\n";
            }
        }
    };

    // Add all connections.
    for (size_t i = 0; i < numNodes(); ++i)
    {
        writeConnections(node(i), dot);
    }
    writeConnections(outputs, dot);

    dot += "}\n";

    return dot;
}

}
