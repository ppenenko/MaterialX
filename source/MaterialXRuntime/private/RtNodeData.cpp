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
    const size_t numPorts = nodedef()->numPorts();
    _values.resize(numPorts);
    _connections.resize(numPorts);

    // Set default values
    for (size_t i = 0; i < numPorts; ++i)
    {
        _values[i] = nodedef()->portdef(i)->getValue();
    }
}

RtDataHandle RtNodeData::create(const RtToken& name, const RtDataHandle& nodedef)
{
    return std::make_shared<RtNodeData>(name, nodedef);
}

RtPort RtNodeData::getPort(size_t index)
{
    RtNodeDefData* nodedef = _nodedef->asA<RtNodeDefData>();
    RtAttributeData* portdef = nodedef->portdef(index);
    return portdef ? RtPort(shared_from_this(), index) : RtPort();
}

RtPort RtNodeData::getPort(const RtToken& name)
{
    RtNodeDefData* nodedef = _nodedef->asA<RtNodeDefData>();
    const size_t index = nodedef->getPortIndex(name);
    RtAttributeData* portdef = nodedef->portdef(index);
    return portdef ? RtPort(shared_from_this(), index) : RtPort();
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
