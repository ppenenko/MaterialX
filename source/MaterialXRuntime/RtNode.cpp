//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtNode.h>
#include <MaterialXRuntime/RtNodeDef.h>
#include <MaterialXRuntime/RtObject.h>

#include <MaterialXRuntime/Private/PrvNode.h>
#include <MaterialXRuntime/Private/PrvNodeGraph.h>
#include <MaterialXRuntime/Private/PrvStage.h>

namespace MaterialX
{

namespace
{
    static const RtPortVec EMPTY_PORT_VEC;
}

RtPort::RtPort() :
    _data(nullptr),
    _index(INVALID_INDEX)
{
}

RtPort::RtPort(RtObject node, RtObject portdef) :
    _data(nullptr),
    _index(INVALID_INDEX)
{
    if (node.hasApi(RtApiType::NODE) && portdef.hasApi(RtApiType::PORTDEF))
    {
        _data = RtApiBase::data(node);
        PrvNodeDef* nd = _data->asA<PrvNode>()->nodedef();
        RtPortDef pd(portdef);
        _index = nd->getElementIndex(pd.getName());
    }
}

RtPort::RtPort(PrvObjectHandle data, size_t index) :
    _data(data),
    _index(index)
{
}

bool RtPort::isValid() const
{
    if (_data)
    {
        PrvNode* node = _data->asA<PrvNode>();
        return node->nodedef()->portdef(_index) != nullptr;
    }
    return false;
}

const RtToken& RtPort::getName() const
{
    PrvNode* node = _data->asA<PrvNode>();
    return node->nodedef()->portdef(_index)->getName();
}

RtObject RtPort::getNode() const
{
    return RtApiBase::object(_data);
}

int32_t RtPort::getFlags() const
{
    PrvNode* node = _data->asA<PrvNode>();
    return node->nodedef()->portdef(_index)->getFlags();
}

bool RtPort::isInput() const
{
    PrvNode* node = _data->asA<PrvNode>();
    return node->nodedef()->portdef(_index)->isInput();
}

bool RtPort::isOutput() const
{
    PrvNode* node = _data->asA<PrvNode>();
    return node->nodedef()->portdef(_index)->isOutput();
}

bool RtPort::isConnectable() const
{
    PrvNode* node = _data->asA<PrvNode>();
    return node->nodedef()->portdef(_index)->isConnectable();
}

const RtValue& RtPort::getValue() const
{
    PrvNode* node = _data->asA<PrvNode>();
    return node->_values[_index];
}

RtValue& RtPort::getValue()
{
    PrvNode* node = _data->asA<PrvNode>();
    return node->_values[_index];
}

void RtPort::setValue(const RtValue& v)
{
    PrvNode* node = _data->asA<PrvNode>();
    node->_values[_index] = v;
}

void RtPort::setValue(bool v)
{
    PrvNode* node = _data->asA<PrvNode>();
    node->_values[_index].asBool() = v;
}

void RtPort::setValue(int v)
{
    PrvNode* node = _data->asA<PrvNode>();
    node->_values[_index].asInt() = v;
}

void RtPort::setValue(unsigned int v)
{
    PrvNode* node = _data->asA<PrvNode>();
    node->_values[_index].asUInt() = v;
}

void RtPort::setValue(float v)
{
    PrvNode* node = _data->asA<PrvNode>();
    node->_values[_index].asFloat() = v;
}

void RtPort::setValue(const Color3& v)
{
    PrvNode* node = _data->asA<PrvNode>();
    node->_values[_index].asColor3() = v;
}

void RtPort::setValue(const Vector4& v)
{
    PrvNode* node = _data->asA<PrvNode>();
    node->_values[_index].asVector4() = v;
}

void RtPort::setValue(void* v)
{
    PrvNode* node = _data->asA<PrvNode>();
    node->_values[_index].asPtr() = v;
}

bool RtPort::isConnected() const
{
    PrvNode* node = _data->asA<PrvNode>();
    return !node->_connections[_index].empty();
}

RtPort RtPort::getSourcePort() const
{
    PrvNode* node = _data->asA<PrvNode>();
    const RtPortVec& connections = node->_connections[_index];
    return connections.size() ? connections[0] : RtPort();
}

size_t RtPort::numDestinationPorts() const
{
    PrvNode* node = _data->asA<PrvNode>();
    return node->_connections[_index].size();
}

RtPort RtPort::getDestinationPort(size_t index) const
{
    PrvNode* node = _data->asA<PrvNode>();
    RtPortVec& connections = node->_connections[_index];
    return index < connections.size() ? connections[index] : RtPort();
}


RtNode::RtNode(const RtObject& obj) :
    RtElement(obj)
{
}

RtObject RtNode::create(const RtToken& name, RtObject nodedef, RtObject parent)
{
    if (!nodedef.hasApi(RtApiType::NODEDEF))
    {
        throw ExceptionRuntimeError("Given nodedef object is not a valid nodedef");
    }

    PrvObjectHandle node = PrvNode::create(name, RtApiBase::data(nodedef));

    if (parent)
    {
        if (parent.hasApi(RtApiType::STAGE))
        {
            PrvObjectHandle parentData = RtApiBase::data(parent);
            parentData->asA<PrvStage>()->addElement(node);
        }
        else if (parent.hasApi(RtApiType::NODEGRAPH))
        {
            PrvObjectHandle parentData = RtApiBase::data(parent);
            parentData->asA<PrvNodeGraph>()->addElement(node);
        }
        else
        {
            throw ExceptionRuntimeError("Parent object must be a stage or a nodegraph");
        }
    }

    return RtApiBase::object(node);
}

RtApiType RtNode::getApiType() const
{
    return RtApiType::NODE;
}

const RtToken& RtNode::getCategory() const
{
    return data()->asA<PrvNode>()->getCategory();
}

RtPort RtNode::getPort(RtObject portdef) const
{
    if (portdef.hasApi(RtApiType::PORTDEF))
    {
        PrvPortDef* pd = RtApiBase::data(portdef)->asA<PrvPortDef>();
        return data()->asA<PrvNode>()->getPort(pd->getName());
    }
    return RtPort();
}

RtPort RtNode::getPort(const RtToken& name) const
{
    return data()->asA<PrvNode>()->getPort(name);
}

RtPort RtNode::getPort(size_t index) const
{
    return data()->asA<PrvNode>()->getPort(index);
}

size_t RtNode::numPorts() const
{
    return data()->asA<PrvNode>()->numPorts();
}

void RtNode::connect(const RtPort& source, const RtPort& dest)
{
    PrvNode::connect(source, dest);
}

void RtNode::disconnect(const RtPort& source, const RtPort& dest)
{
    PrvNode::disconnect(source, dest);
}

}
