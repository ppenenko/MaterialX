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

namespace
{
    // Ignore lists for attributes which are handled explicitly by import.
    // These do not need to be stored as string attributes since at export 
    // they are added explicitly.
    //
    // TODO: Make into a TokenSet
    //
    static const StringSet nodedefIgnoreAttr    = { "name", "type", "node" };
    static const StringSet portdefIgnoreAttr    = { "name", "type", "nodename", "output", "colorspace", "unit" };
    static const StringSet nodeIgnoreAttr       = { "name", "type", "node" };
    static const StringSet nodegraphIgnoreAttr  = { "name", "nodedef" };

    void readAttributes(const ElementPtr src, PrvElement* dest, const StringSet& ignoreList)
    {
        // Read in any attributes so we can export the element again
        // without loosing data. Since it's just for storage we can
        // keep the attributes as strings (tokens).
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

    void writeAttributes(const PrvElement* elem, ElementPtr dest)
    {
        for (size_t i = 0; i < elem->numAttributes(); ++i)
        {
            const RtAttribute* attr = elem->getAttribute(i);
            if (attr->getType() == RtType::STRING)
            {
                dest->setAttribute(attr->getName(), attr->getValue().asToken().str());
            }
        }
    }

    PrvObjectHandle readNodeDef(const NodeDefPtr& src)
    {
        const RtToken name(src->getName());
        const RtToken category(src->getNodeString());

        PrvObjectHandle nodedefH = PrvNodeDef::createNew(name, category);
        PrvNodeDef* nodedef = nodedefH->asA<PrvNodeDef>();

        readAttributes(src, nodedef, nodedefIgnoreAttr);

        // Add outputs
        if (src->getOutputCount() > 0)
        {
            for (auto elem : src->getOutputs())
            {
                const RtToken portName(elem->getName());
                const RtToken portType(elem->getType());
                PrvObjectHandle outputH = PrvPortDef::createNew(portName, portType, RtValue(), 
                                                                RtPortFlag::OUTPUT);
                PrvPortDef* output = outputH->asA<PrvPortDef>();
                readAttributes(elem, output, portdefIgnoreAttr);
                nodedef->addPort(outputH);
            }
        }
        else
        {
            const RtToken type(src->getType());
            PrvObjectHandle outputH = PrvPortDef::createNew(PrvPortDef::DEFAULT_OUTPUT_NAME, type, RtValue(),
                                                            RtPortFlag::OUTPUT);
            nodedef->addPort(outputH);
        }

        // Add inputs
        for (auto elem : src->getChildrenOfType<ValueElement>())
        {
            if (elem->isA<Input>())
            {
                const RtToken portName(elem->getName());
                const RtToken portType(elem->getType());
                PrvObjectHandle inputH = PrvPortDef::createNew(portName, portType, RtValue(elem->getValue()),
                                                               RtPortFlag::INPUT);
                PrvPortDef* input = inputH->asA<PrvPortDef>();
                input->setColorSpace(RtToken(elem->getColorSpace()));
                // TODO: fix when units are implemented in core
                // input->setUnit(RtToken(elem->getUnit()));
                readAttributes(elem, input, portdefIgnoreAttr);
                nodedef->addPort(inputH);
            }
            else if (elem->isA<Parameter>())
            {
                const RtToken portName(elem->getName());
                const RtToken portType(elem->getType());
                PrvObjectHandle inputH = PrvPortDef::createNew(portName, portType, RtValue(elem->getValue()),
                                                               RtPortFlag::INPUT | RtPortFlag::UNIFORM);
                PrvPortDef* input = inputH->asA<PrvPortDef>();
                input->setColorSpace(RtToken(elem->getColorSpace()));
                // TODO: fix when units are implemented in core
                // input->setUnit(RtToken(elem->getUnit()));
                readAttributes(elem, input, portdefIgnoreAttr);
                nodedef->addPort(inputH);
            }
        }

        return nodedefH;
    }

    PrvObjectHandle readNode(const NodePtr& src, PrvStage* stage)
    {
        NodeDefPtr srcNodedef = src->getNodeDef();
        if (!srcNodedef)
        {
            throw ExceptionRuntimeError("No matching nodedef was found for node '" + src->getName() + "'");
        }

        const RtToken nodedefName(srcNodedef->getName());
        PrvObjectHandle nodedefH = stage->findElementByName(nodedefName);
        if (!nodedefH)
        {
            // NodeDef is not loaded yet so create it now.
            nodedefH = readNodeDef(srcNodedef);
            stage->addElement(nodedefH);
        }

        const RtToken nodeName(src->getName());
        PrvObjectHandle nodeH = PrvNode::createNew(nodeName, nodedefH);
        PrvNode* node = nodeH->asA<PrvNode>();

        readAttributes(src, node, nodeIgnoreAttr);

        // Copy input values.
        for (auto elem : src->getChildrenOfType<ValueElement>())
        {
            const RtToken portName(elem->getName());
            RtPort port = node->findPort(portName);
            if (!port)
            {
                throw ExceptionRuntimeError("No port named '" + elem->getName() + "' was found on runtime node '" + node->getName().str() + "'");
            }
            RtValue value(elem->getValue());
            port.setValue(value);
        }

        return nodeH;
    }

    PrvObjectHandle readNodeGraph(const NodeGraphPtr& src, PrvStage* stage)
    {
        PrvObjectHandle graphInterfaceH;
        PrvNodeDef* graphInterface;

        NodeDefPtr srcNodedef = src->getNodeDef();
        if (srcNodedef)
        {
            graphInterfaceH = readNodeDef(srcNodedef);
            graphInterface = graphInterfaceH->asA<PrvNodeDef>();
        }
        else
        {
            const RtToken nodedefName("ND_" + src->getName());
            graphInterfaceH = PrvNodeDef::createNew(nodedefName, EMPTY_TOKEN);
            graphInterface = graphInterfaceH->asA<PrvNodeDef>();

            for (auto elem : src->getOutputs())
            {
                const RtToken name(elem->getName());
                const RtToken type(elem->getType());
                PrvObjectHandle output = PrvPortDef::createNew(name, type, RtValue(),
                                                               RtPortFlag::OUTPUT);
                graphInterface->addPort(output);
            }
        }

        const RtToken nodegraphName(src->getName());
        PrvObjectHandle nodegraphH = PrvNodeGraph::createNew(nodegraphName);
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
                PrvObjectHandle nodeH = readNode(srcNnode, stage);
                nodegraph->addElement(nodeH);

                // Check for connections to the graph interface
                PrvNode* node = nodeH->asA<PrvNode>();
                for (auto elem : srcNnode->getChildrenOfType<ValueElement>())
                {
                    const string& interfaceName = elem->getInterfaceName();
                    if (!interfaceName.empty())
                    {
                        const RtToken graphInputName(interfaceName);
                        RtPort graphInput = graphInputs->findPort(graphInputName);
                        if (!graphInput)
                        {
                            throw ExceptionRuntimeError("Interface name '" + interfaceName + "' does not match an input on the interface for nodegraph '" + 
                                                        nodegraph->getName().str() + "'");
                        }
                        const RtToken inputName(elem->getName());
                        RtPort input = node->findPort(inputName);
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
                            graphOutputs->findPort(RtToken(downstreamElem->getName()));
                        PrvNode::connect(output, graphOutput);
                    }
                    else
                    {
                        const RtToken downstreamNodeName(downstreamElem->getName());
                        const RtToken downstreamInputName(connectingElem->getName());
                        PrvNode* downstreamNode = nodegraph->node(downstreamNodeName);
                        RtPort input = downstreamNode->findPort(downstreamInputName);
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

    void writeNodedef(const PrvNodeDef* nodedef, DocumentPtr dest)
    {
        const size_t numPorts = nodedef->numPorts();
        const size_t numOutputs = nodedef->numOutputs();

        string type = numOutputs == 1 ? nodedef->port(0)->getType().str() : "multioutput";
        NodeDefPtr destNodeDef = dest->addNodeDef(nodedef->getName(), type, nodedef->getCategory());

        for (size_t i = numOutputs; i < numPorts; ++i)
        {
            const PrvPortDef* input = nodedef->port(i);

            ValueElementPtr destInput;
            if (input->isUniform())
            {
                destInput = destNodeDef->addParameter(input->getName(), input->getType().str());
            }
            else
            {
                destInput = destNodeDef->addInput(input->getName(), input->getType().str());
            }

            if (input->getColorSpace())
            {
                destInput->setColorSpace(input->getColorSpace().str());
            }

            if (input->getUnit())
            {
                // TODO: fix when units are implemented in core.
                // destInput->setUnit(input->getUnit().str());
            }

            writeAttributes(input, destInput);
        }
        for (size_t i = 0; i < numOutputs; ++i)
        {
            const PrvPortDef* output = nodedef->port(i);
            OutputPtr destOutput = destNodeDef->addOutput(output->getName(), output->getType().str());
            writeAttributes(output, destOutput);
        }

        writeAttributes(nodedef, destNodeDef);
    }

    template<typename T>
    void writeNode(const PrvNode* node, T dest)
    {
        const PrvNodeDef* nodedef = node->nodedef();

        const size_t numPorts = nodedef->numPorts();
        const size_t numOutputs = nodedef->numOutputs();

        string type = numOutputs == 1 ? nodedef->port(0)->getType().str() : "multioutput";
        NodePtr destNode = dest->addNode(nodedef->getCategory(), node->getName().str(), type);

        for (size_t i = numOutputs; i < numPorts; ++i)
        {
            const PrvPortDef* inputDef = nodedef->port(i);
            RtPort input = const_cast<PrvNode*>(node)->findPort(inputDef->getName());
            if (input.isConnected() || input.getValue() != inputDef->getValue())
            {
                ValueElementPtr destInput;
                if (inputDef->isUniform())
                {
                    destInput = destNode->addParameter(input.getName().str(), input.getType().str());
                }
                else
                {
                    destInput = destNode->addInput(input.getName().str(), input.getType().str());
                }

                destInput->setValueString(input.getValueString());

                if (input.getColorSpace())
                {
                    destInput->setColorSpace(input.getColorSpace().str());
                }
                if (input.getUnit())
                {
                    // TODO: fix when units are implemented in core.
                    // destInput->setUnit(input->getUnit().str());
                }
            }
        }
        if (numOutputs > 1)
        {
            for (size_t i = 0; i < numOutputs; ++i)
            {
                const PrvPortDef* output = nodedef->port(i);
                OutputPtr destOutput = destNode->addOutput(output->getName(), output->getType().str());
            }
        }

        writeAttributes(node, destNode);
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
            stage->addElement(readNodeDef(elem));
        }
    }
    for (auto elem : doc->getNodes())
    {
        stage->addElement(readNode(elem, stage));
    }
    for (auto elem : doc->getNodeGraphs())
    {
        stage->addElement(readNodeGraph(elem, stage));
    }
}

void RtCoreIo::write(DocumentPtr& doc)
{
    PrvStage* stage = data()->asA<PrvStage>();
    writeAttributes(stage, doc);

    for (size_t i = 0; i < stage->numElements(); ++i)
    {
        PrvObjectHandle elemH = stage->getElement(i);
        if (elemH->getObjType() == RtObjType::NODEDEF)
        {
            writeNodedef(elemH->asA<PrvNodeDef>(), doc);
        }
        else if (elemH->getObjType() == RtObjType::NODE)
        {
            writeNode(elemH->asA<PrvNode>(), doc);
        }
    }
}

}
