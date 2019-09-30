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

void RtNodeData::connect(const RtPort& source, const RtPort& dest)
{
    if (dest.isConnected())
    {
        throw ExceptionRuntimeError("Destination port is already connected");
    }
    if (!(source.isOutput() && source.isConnectable()))
    {
        throw ExceptionRuntimeError("Source port is not a connectable output");
    }
    if (!(dest.isInput() && dest.isConnectable()))
    {
        throw ExceptionRuntimeError("Destination port is not a connectable input");
    }

    RtNodeData* sourceNode = source._data->asA<RtNodeData>();
    RtNodeData* destNode = dest._data->asA<RtNodeData>();
    RtPortArray& sourceConnections = sourceNode->_connections[source._index];
    RtPortArray& destConnections = destNode->_connections[dest._index];

    // Make room for the new source.
    destConnections.resize(1);
    destConnections[0] = source;
    sourceConnections.push_back(dest);
}

void RtNodeData::disconnect(const RtPort& source, const RtPort& dest)
{
    RtNodeData* destNode = dest._data->asA<RtNodeData>();
    RtPortArray& destConnections = destNode->_connections[dest._index];
    if (destConnections.size() != 1 || destConnections[0] != source)
    {
        throw ExceptionRuntimeError("Given source and destination is not connected");
    }

    destConnections.clear();

    RtNodeData* sourceNode = source._data->asA<RtNodeData>();
    RtPortArray& sourceConnections = sourceNode->_connections[source._index];
    for (auto it = sourceConnections.begin(); it != sourceConnections.end(); ++it)
    {
        if (*it == dest)
        {
            sourceConnections.erase(it);
            break;
        }
    }
}

}
