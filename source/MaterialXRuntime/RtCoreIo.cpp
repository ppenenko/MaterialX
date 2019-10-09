//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtCoreIo.h>
#include <MaterialXRuntime/RtObject.h>
#include <MaterialXRuntime/RtNodeDef.h>
#include <MaterialXRuntime/RtPortDef.h>
#include <MaterialXRuntime/RtNode.h>
#include <MaterialXRuntime/RtNodeGraph.h>

#include <MaterialXRuntime/Private/PrvStage.h>
#include <MaterialXRuntime/Private/PrvNodeDef.h>
#include <MaterialXRuntime/Private/PrvNode.h>
#include <MaterialXRuntime/Private/PrvNodeGraph.h>

namespace MaterialX
{

PrvObjectHandle createNodeDef(const NodeDefPtr& nodedef)
{
    PrvObjectHandle data = PrvNodeDef::create(nodedef->getName(), nodedef->getNodeString());
    PrvNodeDef* prvNodeDef = data->asA<PrvNodeDef>();

    // Add outputs
    if (nodedef->getOutputCount() > 0)
    {
        for (auto elem : nodedef->getOutputs())
        {
            PrvObjectHandle output = PrvPortDef::create(elem->getName(), elem->getType(), RtValue(),
                                                     RtPortFlag::CONNECTABLE | RtPortFlag::OUTPUT);
            prvNodeDef->addPortDef(output);
        }
    }
    else
    {
        PrvObjectHandle output = PrvPortDef::create("out", nodedef->getType(), RtValue(),
                                                 RtPortFlag::CONNECTABLE | RtPortFlag::OUTPUT);
        prvNodeDef->addPortDef(output);
    }

    // Add inputs
    for (auto elem : nodedef->getChildrenOfType<ValueElement>())
    {
        if (elem->isA<Input>())
        {
            PrvObjectHandle input = PrvPortDef::create(elem->getName(), elem->getType(), RtValue(elem->getValue()), 
                                                    RtPortFlag::CONNECTABLE | RtPortFlag::INPUT);
            prvNodeDef->addPortDef(input);
        }
        else if (elem->isA<Parameter>())
        {
            PrvObjectHandle input = PrvPortDef::create(elem->getName(), elem->getType(), RtValue(elem->getValue()),
                                                    RtPortFlag::CONNECTABLE | RtPortFlag::INPUT | RtPortFlag::UNIFORM);
            prvNodeDef->addPortDef(input);
        }
    }

    return data;
}

PrvObjectHandle createNode(const NodePtr& node, PrvStage* stage)
{
    NodeDefPtr nodedef = node->getNodeDef();
    if (!nodedef)
    {
        throw ExceptionRuntimeError("No matching nodedef was found for node '" + node->getName() + "'");
    }

    PrvObjectHandle prvNodeDefData = stage->getElement(nodedef->getName());
    if (!prvNodeDefData)
    {
        // NodeDef is not loaded yet so create it now.
        prvNodeDefData = createNodeDef(nodedef);
        stage->addElement(prvNodeDefData);
    }

    PrvObjectHandle prvNodeData = PrvNode::create(node->getName(), prvNodeDefData);
    PrvNode* prvNode = prvNodeData->asA<PrvNode>();

    // Copy input values.
    for (auto elem : nodedef->getChildrenOfType<ValueElement>())
    {
        RtPort port = prvNode->getPort(elem->getName());
        if (!port.isValid())
        {
            throw ExceptionRuntimeError("No port named '" + elem->getName() + "' was found on runtime node '" + prvNode->getName() + "'");
        }
        RtValue value(elem->getValue());
        port.setValue(value);
    }

    return prvNodeData;
}

PrvObjectHandle createNodeGraph(const NodeGraphPtr& nodegraph, PrvStage* stage)
{
    PrvObjectHandle graphInterfaceData;
    PrvNodeDef* graphInterface;

    NodeDefPtr nodedef = nodegraph->getNodeDef();
    if (nodedef)
    {
        graphInterfaceData = createNodeDef(nodedef);
        graphInterface = graphInterfaceData->asA<PrvNodeDef>();
    }
    else
    {
        graphInterfaceData = PrvNodeDef::create("ND_" + nodegraph->getName(), EMPTY_STRING);
        graphInterface = graphInterfaceData->asA<PrvNodeDef>();

        for (auto elem : nodegraph->getOutputs())
        {
            PrvObjectHandle output = PrvPortDef::create(elem->getName(), elem->getType(), RtValue(),
                                                     RtPortFlag::OUTPUT | RtPortFlag::CONNECTABLE);
            graphInterface->addPortDef(output);
        }
    }

    PrvObjectHandle prvNodeGraphData = PrvNodeGraph::create(nodegraph->getName());
    PrvNodeGraph* prvNodeGraph = prvNodeGraphData->asA<PrvNodeGraph>();

    // Create the graph interface.
    prvNodeGraph->setInterface(graphInterfaceData);
    PrvNode* graphInputs = prvNodeGraph->inputsNode();
    PrvNode* graphOutputs = prvNodeGraph->outputsNode();

    // Create all nodes.
    for (auto elem : nodegraph->getChildren())
    {
        NodePtr node = elem->asA<Node>();
        if (node)
        {
            PrvObjectHandle prvNodeData = createNode(node, stage);
            prvNodeGraph->addElement(prvNodeData);

            // Check for connections to the graph interface
            PrvNode* prvNode = prvNodeData->asA<PrvNode>();
            for (auto port : node->getChildrenOfType<ValueElement>())
            {
                const string& interfaceName = port->getInterfaceName();
                if (!interfaceName.empty())
                {
                    RtPort graphInput = graphInputs->getPort(interfaceName);
                    if (!graphInput.isValid())
                    {
                        throw ExceptionRuntimeError("Interface name '" + interfaceName + "' does not match an input on the interface for nodegraph '" + nodegraph->getName() + "'");
                    }
                    RtPort input = prvNode->getPort(port->getName());
                    PrvNode::connect(graphInput, input);
                }
            }
        }
    }

    // Create all connections.
    std::set<Edge> processedEdges;
    std::set<Element*> processedInterfaces;
    for (auto elem : nodegraph->getOutputs())
    {
        for (Edge edge : elem->traverseGraph())
        {
            if (processedEdges.count(edge))
            {
                continue;
            }

            ElementPtr downstreamElem = edge.getDownstreamElement();
            ElementPtr connectingElem = edge.getConnectingElement();
            ElementPtr upstreamElem = edge.getUpstreamElement();

            if (upstreamElem->isA<Node>() && !processedInterfaces.count(upstreamElem.get()))
            {
                PrvNode* upstreamNode = prvNodeGraph->node(upstreamElem->getName());

                if (downstreamElem->isA<Output>())
                {
                    RtPort output = upstreamNode->getPort(0); // TODO: Fixme!
                    RtPort graphOutput = graphOutputs->numPorts() == 1 ? // Single outputs can have arbitrary names
                                            graphOutputs->getPort(0) :   // so access by index in this case.
                                            graphOutputs->getPort(downstreamElem->getName());
                    PrvNode::connect(output, graphOutput);
                }
                else
                {
                    PrvNode* downstreamNode = prvNodeGraph->node(downstreamElem->getName());
                    RtPort input = downstreamNode->getPort(connectingElem->getName());
                    RtPort output = upstreamNode->getPort(0); // TODO: Fixme!
                    PrvNode::connect(output, input);
                }

                processedInterfaces.insert(upstreamElem.get());
            }

            processedEdges.insert(edge);
        }
    }

    return prvNodeGraphData;
}


RtCoreIo::RtCoreIo(RtObject stage) :
    RtApiBase(stage)
{
}

RtApiType RtCoreIo::getApiType() const
{
    return RtApiType::CORE_IO;
}

void RtCoreIo::read(const Document& doc, bool allNodeDefs)
{
    PrvStage* stage = data()->asA<PrvStage>();

    if (allNodeDefs)
    {
        for (auto elem : doc.getNodeDefs())
        {
            stage->addElement(createNodeDef(elem));
        }
    }
    for (auto elem : doc.getNodes())
    {
        stage->addElement(createNode(elem, stage));
    }
    for (auto elem : doc.getNodeGraphs())
    {
        stage->addElement(createNodeGraph(elem, stage));
    }
}

void RtCoreIo::write(Document& doc)
{

}

}
