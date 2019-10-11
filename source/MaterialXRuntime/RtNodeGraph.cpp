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

RtObject RtNodeGraph::create(const RtToken& name, RtObject parent)
{
    PrvObjectHandle nodegraph = PrvNodeGraph::create(name);

    if (parent)
    {
        if (!parent.hasApi(RtApiType::STAGE))
        {
            throw ExceptionRuntimeError("Given parent object is not a valid stage");
        }
        RtApiBase::data(parent)->asA<PrvStage>()->addElement(nodegraph);
    }

    return RtApiBase::object(nodegraph);
}

RtApiType RtNodeGraph::getApiType() const
{
    return RtApiType::NODEGRAPH;
}

void RtNodeGraph::addNode(RtObject node)
{
    return data()->asA<PrvNodeGraph>()->addNode(RtApiBase::data(node));
}

RtObject RtNodeGraph::getNode(const RtToken& name) const
{
    PrvObjectHandle node = data()->asA<PrvNodeGraph>()->getElement(name);
    return RtApiBase::object(node);
}

RtObject RtNodeGraph::getNode(size_t index) const
{
    PrvObjectHandle node = data()->asA<PrvNodeGraph>()->getElement(index);
    return RtApiBase::object(node);
}

size_t RtNodeGraph::numNodes() const
{
    return data()->asA<PrvNodeGraph>()->numElements();
}

void RtNodeGraph::setInterface(RtObject nodedef)
{
    return data()->asA<PrvNodeGraph>()->setInterface(RtApiBase::data(nodedef));
}

RtObject RtNodeGraph::getInputsNode() const
{
    PrvObjectHandle node = data()->asA<PrvNodeGraph>()->getInputsNode();
    return RtApiBase::object(node);
}

RtObject RtNodeGraph::getOutputsNode() const
{
    PrvObjectHandle node = data()->asA<PrvNodeGraph>()->getOutputsNode();
    return RtApiBase::object(node);
}

string RtNodeGraph::asStringDot() const
{
    return data()->asA<PrvNodeGraph>()->asStringDot();
}

}
