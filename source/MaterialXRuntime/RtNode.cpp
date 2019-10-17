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
        _data = node.data();
        PrvNode * n = _data->asA<PrvNode>();
        RtPortDef pd(portdef);
        _index = n->findPortIndex(pd.getName());
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

const RtToken& RtPort::getType() const
{
    PrvNode* node = _data->asA<PrvNode>();
    return node->nodedef()->portdef(_index)->getType();
}

RtObject RtPort::getNode() const
{
    return RtObject(_data);
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
    return node->_ports[_index].value;
}

RtValue& RtPort::getValue()
{
    PrvNode* node = _data->asA<PrvNode>();
    return node->_ports[_index].value;
}

string RtPort::getValueString()
{
    PrvNode* node = _data->asA<PrvNode>();
    return node->_ports[_index].value.getValueString(getType());
}

const RtToken& RtPort::getColorSpace() const
{
    PrvNode* node = _data->asA<PrvNode>();
    return node->_ports[_index].colorspace;
}

const RtToken& RtPort::getUnit() const
{
    PrvNode* node = _data->asA<PrvNode>();
    return node->_ports[_index].unit;
}

void RtPort::setValue(const RtValue& v)
{
    PrvNode* node = _data->asA<PrvNode>();
    node->_ports[_index].value = v;
}

void RtPort::setValue(bool v)
{
    PrvNode* node = _data->asA<PrvNode>();
    node->_ports[_index].value.asBool() = v;
}

void RtPort::setValue(int v)
{
    PrvNode* node = _data->asA<PrvNode>();
    node->_ports[_index].value.asInt() = v;
}

void RtPort::setValue(float v)
{
    PrvNode* node = _data->asA<PrvNode>();
    node->_ports[_index].value.asFloat() = v;
}

void RtPort::setValue(const Color3& v)
{
    PrvNode* node = _data->asA<PrvNode>();
    node->_ports[_index].value.asColor3() = v;
}

void RtPort::setValue(const Vector4& v)
{
    PrvNode* node = _data->asA<PrvNode>();
    node->_ports[_index].value.asVector4() = v;
}

void RtPort::setValue(void* v)
{
    PrvNode* node = _data->asA<PrvNode>();
    node->_ports[_index].value.asPtr() = v;
}

void RtPort::setColorSpace(const RtToken& colorspace)
{
    PrvNode* node = _data->asA<PrvNode>();
    node->_ports[_index].colorspace = colorspace;
}

void RtPort::setUnit(const RtToken& unit)
{
    PrvNode* node = _data->asA<PrvNode>();
    node->_ports[_index].unit = unit;
}

bool RtPort::isConnected() const
{
    PrvNode* node = _data->asA<PrvNode>();
    return !node->_ports[_index].connections.empty();
}

RtPort RtPort::getSourcePort() const
{
    PrvNode* node = _data->asA<PrvNode>();
    const RtPortVec& connections = node->_ports[_index].connections;
    return connections.size() ? connections[0] : RtPort();
}

size_t RtPort::numDestinationPorts() const
{
    PrvNode* node = _data->asA<PrvNode>();
    return node->_ports[_index].connections.size();
}

RtPort RtPort::getDestinationPort(size_t index) const
{
    PrvNode* node = _data->asA<PrvNode>();
    RtPortVec& connections = node->_ports[_index].connections;
    return index < connections.size() ? connections[index] : RtPort();
}

RtGraphIterator RtPort::traverseUpstream(RtTraversalFilter filter) const
{
    return RtGraphIterator(*this, filter);
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

    PrvObjectHandle node = PrvNode::create(name, nodedef.data());

    if (parent)
    {
        if (parent.hasApi(RtApiType::STAGE))
        {
            parent.data()->asA<PrvStage>()->addElement(node);
        }
        else if (parent.hasApi(RtApiType::NODEGRAPH))
        {
            parent.data()->asA<PrvNodeGraph>()->addElement(node);
        }
        else
        {
            throw ExceptionRuntimeError("Parent object must be a stage or a nodegraph");
        }
    }

    return RtObject(node);
}

RtApiType RtNode::getApiType() const
{
    return RtApiType::NODE;
}

const RtToken& RtNode::getCategory() const
{
    return data()->asA<PrvNode>()->getCategory();
}

size_t RtNode::numPorts() const
{
    return data()->asA<PrvNode>()->numPorts();
}

size_t RtNode::numOutputs() const
{
    return data()->asA<PrvNode>()->numOutputs();
}

RtPort RtNode::getPort(size_t index) const
{
    return data()->asA<PrvNode>()->getPort(index);
}

RtPort RtNode::findPort(const RtToken& name) const
{
    return data()->asA<PrvNode>()->findPort(name);
}

RtPort RtNode::getPort(RtObject portdef) const
{
    if (portdef.hasApi(RtApiType::PORTDEF))
    {
        PrvPortDef* pd = portdef.data()->asA<PrvPortDef>();
        return data()->asA<PrvNode>()->findPort(pd->getName());
    }
    return RtPort();
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
