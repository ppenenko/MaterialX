//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/private/RtNodeData.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

RtNodeData::RtNodeData(const RtToken& name, const RtDataHandle& nd) :
    RtElementData(RtObjType::NODE, name),
    _nodedef(nd)
{
    const size_t numAttr = nodedef()->numAttributes();
    _values.resize(numAttr);
    _connections.resize(numAttr);

    // Set default values
    for (size_t i = 0; i < numAttr; ++i)
    {
        _values[i] = nodedef()->attribute(i)->getValue();
    }
}

RtDataHandle RtNodeData::create(const RtToken& name, const RtDataHandle& nodedef)
{
    return std::make_shared<RtNodeData>(name, nodedef);
}

RtPort RtNodeData::getInputPort(const RtToken& name)
{
    RtNodeDefData* nodedef = _nodedef->asA<RtNodeDefData>();
    const size_t index = nodedef->getAttributeIndex(name);
    RtAttributeData* attr = nodedef->getAttribute(index)->asA<RtAttributeData>();
    return attr && attr->isInput() ? RtPort(shared_from_this(), index) : RtPort();
}

RtPort RtNodeData::getOutputPort(const RtToken& name)
{
    RtNodeDefData* nodedef = _nodedef->asA<RtNodeDefData>();
    const size_t index = nodedef->getAttributeIndex(name);
    RtAttributeData* attr = nodedef->getAttribute(index)->asA<RtAttributeData>();
    return attr && attr->isOutput() ? RtPort(shared_from_this(), index) : RtPort();
}

}
