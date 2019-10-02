//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtNodeGraph.h>
#include <MaterialXRuntime/RtObject.h>

#include <MaterialXRuntime/private/RtNodeGraphData.h>
#include <MaterialXRuntime/private/RtStageData.h>

namespace MaterialX
{

RtNodeGraph::RtNodeGraph(const RtObject& obj) :
    RtElement(obj)
{
}

RtObject RtNodeGraph::create(const RtToken& name, RtObject stage)
{
    if (!stage.hasApi(RtApiType::STAGE))
    {
        throw ExceptionRuntimeError("Given stage object is not a valid stage");
    }

    RtDataHandle nodegraph = RtNodeGraphData::create(name);
    RtApiBase::data(stage)->asA<RtStageData>()->addNodeGraph(nodegraph);

    return RtApiBase::object(nodegraph);
}

RtApiType RtNodeGraph::getApiType() const
{
    return RtApiType::NODEDEF;
}

void RtNodeGraph::addNode(RtObject node)
{
    return data()->asA<RtNodeGraphData>()->addNode(RtApiBase::data(node));
}

RtObject RtNodeGraph::getNode(const RtToken& name) const
{
    RtDataHandle node = data()->asA<RtNodeGraphData>()->getNode(name);
    return RtApiBase::object(node);
}

RtObject RtNodeGraph::getNode(size_t index) const
{
    RtDataHandle node = data()->asA<RtNodeGraphData>()->getNode(index);
    return RtApiBase::object(node);
}

size_t RtNodeGraph::numNodes() const
{
    return data()->asA<RtNodeGraphData>()->numNodes();
}

void RtNodeGraph::setInterface(RtObject nodedef)
{
    return data()->asA<RtNodeGraphData>()->setInterface(RtApiBase::data(nodedef));
}

RtObject RtNodeGraph::getInputInterface() const
{
    RtDataHandle node = data()->asA<RtNodeGraphData>()->getInputInterface();
    return RtApiBase::object(node);
}

RtObject RtNodeGraph::getOutputInterface() const
{
    RtDataHandle node = data()->asA<RtNodeGraphData>()->getOutputInterface();
    return RtApiBase::object(node);
}


}
