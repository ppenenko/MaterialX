//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/private/RtStageData.h>
#include <MaterialXRuntime/private/RtNodeDefData.h>
#include <MaterialXRuntime/private/RtNodeData.h>
#include <MaterialXRuntime/private/RtNodeGraphData.h>

#include <MaterialXRuntime/RtObject.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

RtStageData::RtStageData(const RtToken& name) :
    RtElementData(RtObjType::STAGE, name)
{
}

void RtStageData::addNodeDef(RtDataHandle nodedef)
{
    if (!nodedef->hasApi(RtApiType::NODEDEF))
    {
        throw ExceptionRuntimeError("Given object is not a valid nodedef");
    }
    RtNodeDefData* nd = nodedef->asA<RtNodeDefData>();
    auto it = _nodedefsByName.find(nd->getName());
    if (it != _nodedefsByName.end())
    {
        throw ExceptionRuntimeError("An nodedef named '" + nd->getName() + "' already exists for stage '" + getName() + "'");
    }
    _nodedefsByName[nd->getName()] = _nodedefs.size();
    _nodedefs.push_back(nodedef);
}

void RtStageData::addNode(RtDataHandle node)
{
    if (!node->hasApi(RtApiType::NODE))
    {
        throw ExceptionRuntimeError("Given object is not a valid node");
    }
    RtNodeData* n = node->asA<RtNodeData>();
    auto it = _nodesByName.find(n->getName());
    if (it != _nodesByName.end())
    {
        throw ExceptionRuntimeError("An node named '" + n->getName() + "' already exists for stage '" + getName() + "'");
    }
    _nodesByName[n->getName()] = _nodes.size();
    _nodes.push_back(node);
}

void RtStageData::addNodeGraph(RtDataHandle nodegraph)
{
    if (!nodegraph->hasApi(RtApiType::NODEGRAPH))
    {
        throw ExceptionRuntimeError("Given object is not a valid nodegraph");
    }
    RtNodeGraphData* ng = nodegraph->asA<RtNodeGraphData>();
    auto it = _nodesByName.find(ng->getName());
    if (it != _nodesByName.end())
    {
        throw ExceptionRuntimeError("An nodegraph named '" + ng->getName() + "' already exists for stage '" + getName() + "'");
    }
    _nodesByName[ng->getName()] = _nodes.size();
    _nodes.push_back(nodegraph);
}

void RtStageData::clear()
{
    _nodedefs.clear();
    _nodedefsByName.clear();
}

RtDataHandle RtStageData::create(const RtToken& name)
{
    return std::make_shared<RtStageData>(name);
}

}

