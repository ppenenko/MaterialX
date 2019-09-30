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

namespace
{
    static const RtPortArray EMPTY_PORT_ARRAY;
}

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
        return node->nodedef()->portdef(_index) != nullptr;
    }
    return false;
}

const RtToken& RtPort::getName() const
{
    RtNodeData* node = _data->asA<RtNodeData>();
    return node->nodedef()->portdef(_index)->getName();
}

RtObject RtPort::getNode() const
{
    return RtApiBase::object(_data);
}

int32_t RtPort::getFlags() const
{
    RtNodeData* node = _data->asA<RtNodeData>();
    return node->nodedef()->portdef(_index)->getFlags();
}

bool RtPort::isInput() const
{
    RtNodeData* node = _data->asA<RtNodeData>();
    return node->nodedef()->portdef(_index)->isInput();
}

bool RtPort::isOutput() const
{
    RtNodeData* node = _data->asA<RtNodeData>();
    return node->nodedef()->portdef(_index)->isOutput();
}

bool RtPort::isConnectable() const
{
    RtNodeData* node = _data->asA<RtNodeData>();
    return node->nodedef()->portdef(_index)->isConnectable();
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

bool RtPort::isConnected() const
{
    RtNodeData* node = _data->asA<RtNodeData>();
    return !node->_connections[_index].empty();
}

RtPort RtPort::getConnectionSource() const
{
    RtNodeData* node = _data->asA<RtNodeData>();
    const RtPortArray& connections = node->_connections[_index];
    return connections.size() ? connections[0] : RtPort();
}

const RtPortArray& RtPort::getConnectionDestinations() const
{
    RtNodeData* node = _data->asA<RtNodeData>();
    const RtPortArray& connections = node->_connections[_index];
    return connections.size() ? connections : EMPTY_PORT_ARRAY;
}


RtNode::RtNode(const RtObject& obj) :
    RtElement(obj)
{
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

RtApiType RtNode::getApiType() const
{
    return RtApiType::NODE;
}

const RtToken& RtNode::getCategory() const
{
    return data()->asA<RtNodeData>()->getCategory();
}

RtPort RtNode::getPort(const RtToken& name) const
{
    return data()->asA<RtNodeData>()->getPort(name);
}

RtPort RtNode::getPort(size_t index) const
{
    return data()->asA<RtNodeData>()->getPort(index);
}

size_t RtNode::numPorts() const
{
    return data()->asA<RtNodeData>()->numPorts();
}

void RtNode::connect(const RtPort& source, const RtPort& dest)
{
    RtNodeData::connect(source, dest);
}

void RtNode::disconnect(const RtPort& source, const RtPort& dest)
{
    RtNodeData::disconnect(source, dest);
}

}
