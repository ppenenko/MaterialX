//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtNodeGraph.h>
#include <MaterialXRuntime/RtObject.h>

#include <MaterialXRuntime/Private/PrvNodeGraph.h>
#include <MaterialXRuntime/Private/PrvStage.h>

namespace MaterialX
{

RtNodeGraph::RtNodeGraph(const RtObject& obj) :
    RtElement(obj)
{
}

RtObject RtNodeGraph::createNew(const RtToken& name, RtObject parent)
{
    PrvObjectHandle nodegraph = PrvNodeGraph::createNew(name);

    if (parent)
    {
        if (!parent.hasApi(RtApiType::STAGE))
        {
            throw ExceptionRuntimeError("Given parent object is not a valid stage");
        }
        parent.data()->asA<PrvStage>()->addElement(nodegraph);
    }

    return RtObject(nodegraph);
}

RtApiType RtNodeGraph::getApiType() const
{
    return RtApiType::NODEGRAPH;
}

void RtNodeGraph::addNode(RtObject node)
{
    return data()->asA<PrvNodeGraph>()->addNode(node.data());
}

size_t RtNodeGraph::numNodes() const
{
    return data()->asA<PrvNodeGraph>()->numElements();
}

RtObject RtNodeGraph::getNode(size_t index) const
{
    PrvObjectHandle node = data()->asA<PrvNodeGraph>()->getElement(index);
    return RtObject(node);
}

RtObject RtNodeGraph::findNode(const RtToken& name) const
{
    PrvObjectHandle node = data()->asA<PrvNodeGraph>()->findElementByName(name);
    return RtObject(node);
}

void RtNodeGraph::setInterface(RtObject nodedef)
{
    return data()->asA<PrvNodeGraph>()->setInterface(nodedef.data());
}

RtObject RtNodeGraph::getInputsNode() const
{
    PrvObjectHandle node = data()->asA<PrvNodeGraph>()->getInputsNode();
    return RtObject(node);
}

RtObject RtNodeGraph::getOutputsNode() const
{
    PrvObjectHandle node = data()->asA<PrvNodeGraph>()->getOutputsNode();
    return RtObject(node);
}

string RtNodeGraph::asStringDot() const
{
    return data()->asA<PrvNodeGraph>()->asStringDot();
}

}
