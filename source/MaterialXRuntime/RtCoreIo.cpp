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
#include <MaterialXRuntime/RtTypes.h>

#include <MaterialXRuntime/Private/PrvStage.h>
#include <MaterialXRuntime/Private/PrvNodeDef.h>
#include <MaterialXRuntime/Private/PrvNode.h>
#include <MaterialXRuntime/Private/PrvNodeGraph.h>

namespace MaterialX
{

namespace {

    // Ignore lists for attributes which are handled explicitly by import.
    // These do not need to be stored as export will add them explicitly.
    //
    // TODO: Make into a TokenSet
    //
    static const StringSet nodedefIgnoreAttr    = { "name", "type", "node" };
    static const StringSet portdefIgnoreAttr    = { "name", "type", "nodename", "output" };
    static const StringSet nodeIgnoreAttr       = { "name", "type", "node" };
    static const StringSet nodegraphIgnoreAttr  = { "name", "nodedef" };

    void readAttributes(const ElementPtr src, PrvElement* dest, const StringSet& ignoreList)
    {
        // Read in any attributes so we can export the element again
        // without loosing data. Since it's just for storage we can
        // keep the attributes as strings.
        for (const string& name : src->getAttributeNames())
        {
            if (!ignoreList.count(name))
            {
                const RtToken attrValue(src->getAttribute(name));
                const RtToken attrName(name);
                dest->addAttribute(attrName, RtType::STRING, RtValue(attrValue));
            }
        }
    }

    PrvObjectHandle createNodeDef(const NodeDefPtr& src)
    {
        const RtToken name(src->getName());
        const RtToken category(src->getNodeString());

        PrvObjectHandle nodedefH = PrvNodeDef::create(name, category);
        PrvNodeDef* nodedef = nodedefH->asA<PrvNodeDef>();

        readAttributes(src, nodedef, nodedefIgnoreAttr);

        // Add outputs
        if (src->getOutputCount() > 0)
        {
            for (auto elem : src->getOutputs())
            {
                const RtToken portName(elem->getName());
                const RtToken portType(elem->getType());
                PrvObjectHandle outputH = PrvPortDef::create(portName, portType, RtValue(),
                                                             RtPortFlag::CONNECTABLE | RtPortFlag::OUTPUT);
                PrvPortDef* output = outputH->asA<PrvPortDef>();
                readAttributes(elem, output, portdefIgnoreAttr);
                nodedef->addPortDef(outputH);
            }
        }
        else
        {
            const RtToken type(src->getType());
            PrvObjectHandle outputH = PrvPortDef::create(PrvPortDef::DEFAULT_OUTPUT_NAME, type, RtValue(),
                                                         RtPortFlag::CONNECTABLE | RtPortFlag::OUTPUT);
            nodedef->addPortDef(outputH);
        }

        // Add inputs
        for (auto elem : src->getChildrenOfType<ValueElement>())
        {
            if (elem->isA<Input>())
            {
                const RtToken portName(elem->getName());
                const RtToken portType(elem->getType());
                PrvObjectHandle inputH = PrvPortDef::create(portName, portType, RtValue(elem->getValue()),
                                                            RtPortFlag::CONNECTABLE | RtPortFlag::INPUT);
                PrvPortDef* input = inputH->asA<PrvPortDef>();
                readAttributes(elem, input, portdefIgnoreAttr);
                nodedef->addPortDef(inputH);
            }
            else if (elem->isA<Parameter>())
            {
                const RtToken portName(elem->getName());
                const RtToken portType(elem->getType());
                PrvObjectHandle inputH = PrvPortDef::create(portName, portType, RtValue(elem->getValue()),
                                                           RtPortFlag::CONNECTABLE | RtPortFlag::INPUT | RtPortFlag::UNIFORM);
                PrvPortDef* input = inputH->asA<PrvPortDef>();
                readAttributes(elem, input, portdefIgnoreAttr);
                nodedef->addPortDef(inputH);
            }
        }

        return nodedefH;
    }

    PrvObjectHandle createNode(const NodePtr& src, PrvStage* stage)
    {
        NodeDefPtr srcNodedef = src->getNodeDef();
        if (!srcNodedef)
        {
            throw ExceptionRuntimeError("No matching nodedef was found for node '" + src->getName() + "'");
        }

        const RtToken nodedefName(srcNodedef->getName());
        PrvObjectHandle nodedefH = stage->getElement(nodedefName);
        if (!nodedefH)
        {
            // NodeDef is not loaded yet so create it now.
            nodedefH = createNodeDef(srcNodedef);
            stage->addElement(nodedefH);
        }

        const RtToken nodeName(src->getName());
        PrvObjectHandle nodeH = PrvNode::create(nodeName, nodedefH);
        PrvNode* node = nodeH->asA<PrvNode>();

        readAttributes(src, node, nodeIgnoreAttr);

        // Copy input values.
        for (auto elem : srcNodedef->getChildrenOfType<ValueElement>())
        {
            const RtToken portName(elem->getName());
            RtPort port = node->getPort(portName);
            if (!port)
            {
                throw ExceptionRuntimeError("No port named '" + elem->getName() + "' was found on runtime node '" + node->getName().str() + "'");
            }
            RtValue value(elem->getValue());
            port.setValue(value);
        }

        return nodeH;
    }

    PrvObjectHandle createNodeGraph(const NodeGraphPtr& src, PrvStage* stage)
    {
        PrvObjectHandle graphInterfaceH;
        PrvNodeDef* graphInterface;

        NodeDefPtr srcNodedef = src->getNodeDef();
        if (srcNodedef)
        {
            graphInterfaceH = createNodeDef(srcNodedef);
            graphInterface = graphInterfaceH->asA<PrvNodeDef>();
        }
        else
        {
            const RtToken nodedefName("ND_" + src->getName());
            graphInterfaceH = PrvNodeDef::create(nodedefName, EMPTY_TOKEN);
            graphInterface = graphInterfaceH->asA<PrvNodeDef>();

            for (auto elem : src->getOutputs())
            {
                const RtToken name(elem->getName());
                const RtToken type(elem->getType());
                PrvObjectHandle output = PrvPortDef::create(name, type, RtValue(),
                                                            RtPortFlag::OUTPUT | RtPortFlag::CONNECTABLE);
                graphInterface->addPortDef(output);
            }
        }

        const RtToken nodegraphName(src->getName());
        PrvObjectHandle nodegraphH = PrvNodeGraph::create(nodegraphName);
        PrvNodeGraph* nodegraph = nodegraphH->asA<PrvNodeGraph>();

        readAttributes(src, nodegraph, nodegraphIgnoreAttr);

        // Create the graph interface.
        nodegraph->setInterface(graphInterfaceH);
        PrvNode* graphInputs = nodegraph->inputsNode();
        PrvNode* graphOutputs = nodegraph->outputsNode();

        // Create all nodes.
        for (auto child : src->getChildren())
        {
            NodePtr srcNnode = child->asA<Node>();
            if (srcNnode)
            {
                PrvObjectHandle nodeH = createNode(srcNnode, stage);
                nodegraph->addElement(nodeH);

                // Check for connections to the graph interface
                PrvNode* node = nodeH->asA<PrvNode>();
                for (auto elem : srcNnode->getChildrenOfType<ValueElement>())
                {
                    const string& interfaceName = elem->getInterfaceName();
                    if (!interfaceName.empty())
                    {
                        const RtToken graphInputName(interfaceName);
                        RtPort graphInput = graphInputs->getPort(graphInputName);
                        if (!graphInput)
                        {
                            throw ExceptionRuntimeError("Interface name '" + interfaceName + "' does not match an input on the interface for nodegraph '" + 
                                                        nodegraph->getName().str() + "'");
                        }
                        const RtToken inputName(elem->getName());
                        RtPort input = node->getPort(inputName);
                        PrvNode::connect(graphInput, input);
                    }
                }
            }
        }

        // Create all connections.
        std::set<Edge> processedEdges;
        std::set<Element*> processedInterfaces;
        for (auto elem : src->getOutputs())
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
                    const RtToken upstreamNodeName(upstreamElem->getName());
                    PrvNode* upstreamNode = nodegraph->node(upstreamNodeName);

                    if (downstreamElem->isA<Output>())
                    {
                        RtPort output = upstreamNode->getPort(0); // TODO: Fixme!
                        RtPort graphOutput = graphOutputs->numPorts() == 1 ? // Single outputs can have arbitrary names.
                            graphOutputs->getPort(0) :                       // so access by index in that case.
                            graphOutputs->getPort(RtToken(downstreamElem->getName()));
                        PrvNode::connect(output, graphOutput);
                    }
                    else
                    {
                        const RtToken downstreamNodeName(downstreamElem->getName());
                        const RtToken downstreamInputName(connectingElem->getName());
                        PrvNode* downstreamNode = nodegraph->node(downstreamNodeName);
                        RtPort input = downstreamNode->getPort(downstreamInputName);
                        RtPort output = upstreamNode->getPort(0); // TODO: Fixme!
                        PrvNode::connect(output, input);
                    }

                    processedInterfaces.insert(upstreamElem.get());
                }

                processedEdges.insert(edge);
            }
        }

        return nodegraphH;
    }

} // end anonymous namespace

RtCoreIo::RtCoreIo(RtObject stage) :
    RtApiBase(stage)
{
}

RtApiType RtCoreIo::getApiType() const
{
    return RtApiType::CORE_IO;
}

void RtCoreIo::read(const DocumentPtr& doc, bool allNodeDefs)
{
    PrvStage* stage = data()->asA<PrvStage>();
    readAttributes(doc, stage, {});

    if (allNodeDefs)
    {
        for (auto elem : doc->getNodeDefs())
        {
            stage->addElement(createNodeDef(elem));
        }
    }
    for (auto elem : doc->getNodes())
    {
        stage->addElement(createNode(elem, stage));
    }
    for (auto elem : doc->getNodeGraphs())
    {
        stage->addElement(createNodeGraph(elem, stage));
    }
}

void RtCoreIo::write(DocumentPtr& doc)
{

}

}
