//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtNode.h>
#include <MaterialXRuntime/RtObject.h>

#include <MaterialXRuntime/private/RtNodeData.h>
#include <MaterialXRuntime/private/RtStageData.h>

namespace MaterialX
{

RtPort::RtPort() :
    _data(nullptr),
    _index(RtNodeDefData::INVALID_INDEX)
{
}

RtPort::RtPort(RtDataHandle data, size_t index) :
    _data(data),
    _index(index)
{
}

bool RtPort::isValid() const
{
    if (_data)
    {
        RtNodeData* node = _data->asA<RtNodeData>();
        return node->nodedef()->attribute(_index) != nullptr;
    }
    return false;
}

const RtToken& RtPort::getName() const
{
    RtNodeData* node = _data->asA<RtNodeData>();
    return node->nodedef()->attribute(_index)->getName();
}

RtObject RtPort::getNode() const
{
    return RtApiBase::object(_data);
}

int32_t RtPort::getFlags() const
{
    RtNodeData* node = _data->asA<RtNodeData>();
    return node->nodedef()->attribute(_index)->getFlags();
}

bool RtPort::isInput() const
{
    RtNodeData* node = _data->asA<RtNodeData>();
    return node->nodedef()->attribute(_index)->isInput();
}

bool RtPort::isOutput() const
{
    RtNodeData* node = _data->asA<RtNodeData>();
    return node->nodedef()->attribute(_index)->isOutput();
}

bool RtPort::isConnectable() const
{
    RtNodeData* node = _data->asA<RtNodeData>();
    return node->nodedef()->attribute(_index)->isConnectable();
}

bool RtPort::isConnectableTo(const RtPort& other) const
{
    if (_data == other._data)
    {
        return false;
    }
    RtNodeData* thisNode = _data->asA<RtNodeData>();
    RtNodeData* otherNode = (RtNodeData*)other._data.get();
    const RtAttributeData* thisAttr = thisNode->nodedef()->attribute(_index);
    const RtAttributeData* otherAttr = otherNode->nodedef()->attribute(other._index);
    return thisAttr->isConnectableTo(otherAttr);
}

const RtValue& RtPort::getValue() const
{
    RtNodeData* node = _data->asA<RtNodeData>();
    return node->_values[_index];
}

RtValue& RtPort::getValue()
{
    RtNodeData* node = _data->asA<RtNodeData>();
    return node->_values[_index];
}

void RtPort::setValue(const RtValue& v)
{
    RtNodeData* node = _data->asA<RtNodeData>();
    node->_values[_index] = v;
}

void RtPort::setValue(bool v)
{
    RtNodeData* node = _data->asA<RtNodeData>();
    node->_values[_index].asBool() = v;
}

void RtPort::setValue(int v)
{
    RtNodeData* node = _data->asA<RtNodeData>();
    node->_values[_index].asInt() = v;
}

void RtPort::setValue(unsigned int v)
{
    RtNodeData* node = _data->asA<RtNodeData>();
    node->_values[_index].asUInt() = v;
}

void RtPort::setValue(float v)
{
    RtNodeData* node = _data->asA<RtNodeData>();
    node->_values[_index].asFloat() = v;
}

void RtPort::setValue(const Color3& v)
{
    RtNodeData* node = _data->asA<RtNodeData>();
    node->_values[_index].asColor3() = v;
}

void RtPort::setValue(const Vector4& v)
{
    RtNodeData* node = _data->asA<RtNodeData>();
    node->_values[_index].asVector4() = v;
}

void RtPort::setValue(void* v)
{
    RtNodeData* node = _data->asA<RtNodeData>();
    node->_values[_index].asPtr() = v;
}

void RtPort::connectTo(RtPort& other)
{
    if (_data == other._data)
    {
        throw ExceptionRuntimeError("Connecting a node to itself is not allowed");
    }
    if (!isConnectableTo(other))
    {
        throw ExceptionRuntimeError("Ports are not connectable");
    }

    RtNodeData* thisNode = _data->asA<RtNodeData>();
    RtNodeData* otherNode = (RtNodeData*)other._data.get();

    // Break any existing connections.
    other.disconnect();
    this->disconnect();

    // Make new connections.
    thisNode->_connections[_index] = other;
    otherNode->_connections[other._index] = *this;
}

void RtPort::disconnect()
{
    RtNodeData* node = _data->asA<RtNodeData>();
    RtPort& other = node->_connections[_index];
    if (other.isValid())
    {
        RtNodeData* otherNode = (RtNodeData*)other._data.get();
        otherNode->_connections[other._index] = RtPort();
        other = RtPort();
    }
}


RtNode::RtNode(const RtObject& obj) :
    RtElement(obj)
{
}

RtApiType RtNode::getApiType() const
{
    return RtApiType::NODE;
}

const RtToken& RtNode::getCategory() const
{
    return data()->asA<RtNodeData>()->getCategory();
}

RtPort RtNode::getInputPort(const RtToken& name) const
{
    return data()->asA<RtNodeData>()->getInputPort(name);
}

RtPort RtNode::getOutputPort(const RtToken& name) const
{
    return data()->asA<RtNodeData>()->getOutputPort(name);
}

RtObject RtNode::create(const RtToken& name, RtObject nodedef, RtObject stage)
{
    if (!nodedef.hasApi(RtApiType::NODEDEF))
    {
        throw ExceptionRuntimeError("Given object is not a valid nodedef");
    }
    if (!stage.hasApi(RtApiType::STAGE))
    {
        throw ExceptionRuntimeError("Given object is not a valid stage");
    }

    RtStageData* stagedata = RtApiBase::data(stage)->asA<RtStageData>();
    // TODO: Check if name exists

    RtDataHandle node = RtNodeData::create(name, RtApiBase::data(nodedef));
    stagedata->addNode(node);

    return RtApiBase::object(node);
}

}
