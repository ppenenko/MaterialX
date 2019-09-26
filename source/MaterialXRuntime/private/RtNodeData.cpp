//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/private/RtNodeData.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

RtPortData::RtPortData() :
    _node(nullptr),
    _index(RtNodeDefData::INVALID_INDEX)
{
}

RtPortData::RtPortData(RtNodeData* node, size_t index) :
    _node(node),
    _index(index)
{
}

const RtAttributeData* RtPortData::getAttribute() const
{
    return _node->getNodeDef()->getAttribute(_index);
}

RtValue& RtPortData::getValue()
{
    return _node->_values[_index];
}

void RtPortData::setValue(const RtValue& v)
{
    _node->_values[_index] = v;
}

void RtPortData::setValue(bool v)
{
    _node->_values[_index].asBool() = v;
}

void RtPortData::setValue(int v)
{
    _node->_values[_index].asInt() = v;
}

void RtPortData::setValue(unsigned int v)
{
    _node->_values[_index].asUInt() = v;
}

void RtPortData::setValue(float v)
{
    _node->_values[_index].asFloat() = v;
}

void RtPortData::setValue(const Color3& v)
{
    _node->_values[_index].asColor3() = v;
}

void RtPortData::setValue(const Vector4& v)
{
    _node->_values[_index].asVector4() = v;
}

void RtPortData::setValue(void* v)
{
    _node->_values[_index].asPtr() = v;
}

void RtPortData::connectTo(RtPortData& other)
{
    const RtAttributeData* thisAttr = getAttribute();
    const RtAttributeData* otherAttr = other.getAttribute();
    if (!thisAttr->isConnectableTo(otherAttr))
    {
        throw ExceptionRuntimeError("Ports are not connectable");
    }
    other.disconnect();
    this->disconnect();
    _node->_connections[_index] = other;
    other._node->_connections[other._index] = *this;
}

void RtPortData::disconnect()
{
    RtPortData& other = _node->_connections[_index];
    if (other.isValid())
    {
        // Wipe out existing connection
        other._node->_connections[other._index] = RtPortData();
        other = RtPortData();
    }
}

RtNodeData::RtNodeData(const RtToken& name, const RtNodeDefData* nodedef) :
    RtElementData(RtObjType::NODE, name),
    _nodedef(nodedef)
{
    // Set default values
    _values.resize(nodedef->numAttributes());
    for (size_t i = 0; i < nodedef->numAttributes(); ++i)
    {
        _values[i] = _nodedef->getAttribute(i)->getValue();
    }

    _connections.resize(nodedef->numAttributes());
}

}
