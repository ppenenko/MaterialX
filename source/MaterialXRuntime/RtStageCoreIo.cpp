//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtStageCoreIo.h>
#include <MaterialXRuntime/RtObject.h>
#include <MaterialXRuntime/RtNodeDef.h>
#include <MaterialXRuntime/RtPortDef.h>
#include <MaterialXRuntime/RtNode.h>
#include <MaterialXRuntime/RtNodeGraph.h>

namespace MaterialX
{

RtStageCoreIo::RtStageCoreIo(RtObject stage) :
    RtApiBase(stage)
{
}

RtApiType RtStageCoreIo::getApiType() const
{
    return RtApiType::STAGE_CORE_IO;
}

void RtStageCoreIo::read(const Document& doc)
{
    for (auto nodedef : doc.getNodeDefs())
    {
        read(*nodedef);
    }
    for (auto nodes : doc.getNodes())
    {
        read(*nodes);
    }
}

void RtStageCoreIo::read(const NodeDef& nodedef)
{
    RtObject nodedefObj = RtNodeDef::create(nodedef.getName(), nodedef.getCategory(), getObject());
    for (auto port : nodedef.getChildrenOfType<ValueElement>())
    {
        uint32_t flags = RtPortFlag::CONNECTABLE;
        if (port->isA<Input>())
        {
            flags |= RtPortFlag::INPUT;
        }
        else if (port->isA<Output>())
        {
            flags |= RtPortFlag::OUTPUT;
        }
        else // a parameter
        {
            flags |= RtPortFlag::INPUT | RtPortFlag::UNIFORM;
        }
        RtPortDef::create(port->getName(), port->getType(), RtValue(port->getValue()), flags, nodedefObj);
    }
}

void RtStageCoreIo::read(const Node& node)
{
    NodeDefPtr nodedef = node.getNodeDef();
    if (!nodedef)
    {
        throw ExceptionRuntimeError("No matching nodedef was found for node '" + node.getName() + "'");
    }
}

void RtStageCoreIo::write(Document& doc)
{

}

}
