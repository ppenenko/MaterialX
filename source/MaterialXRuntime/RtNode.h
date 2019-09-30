//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RT_NODE_H
#define MATERIALX_RT_NODE_H

/// @file
/// TODO: Docs

#include <MaterialXRuntime/RtElement.h>
#include <MaterialXRuntime/RtAttribute.h>

namespace MaterialX
{

class RtPort;
using RtPortArray = vector<RtPort>;

/// @class RtPort
/// TODO: Docs
class RtPort
{
public:
    RtPort();

    bool isValid() const;

    const RtToken& getName() const;

    RtObject getNode() const;

    /// Return the attribute flags for this port.
    int32_t getFlags() const;

    /// Return true if this is an input port.
    bool isInput() const;

    /// Return true if this is an output port.
    bool isOutput() const;

    /// Return true if this port is connectable.
    bool isConnectable() const;

    /// Return the value for this port.
    const RtValue& getValue() const;

    /// Return the value for this port.
    RtValue& getValue();

    /// Set a new value on the port.
    void setValue(const RtValue& v);

    /// Set a new bool value on the port.
    void setValue(bool v);

    /// Set a new integer value on the port.
    void setValue(int v);

    /// Set a new unsigned integer value on the port.
    void setValue(unsigned int v);

    /// Set a new float value on the port.
    void setValue(float v);

    /// Set a new color3 value on the port.
    void setValue(const Color3& v);

    /// Set a new vector3 value on the port.
    void setValue(const Vector4& v);

    /// Set a new pointer value on the port.
    void setValue(void* v);

    /// Return true if this port is connected.
    bool isConnected() const;

    /// Return the port connected upstream.
    RtPort getConnectionSource() const;

    /// Return the ports connected downstream.
    const RtPortArray& getConnectionDestinations() const;

    /// Equality operator
    bool operator==(const RtPort& other)
    {
        return _data == other._data && _index == other._index;
    }

    /// Inequality operator
    bool operator!=(const RtPort& other)
    {
        return _data != other._data || _index != other._index;
    }

protected:
    RtPort(RtDataHandle data, size_t index);

    RtDataHandle _data;
    size_t _index;
    friend class RtNodeData;
};

/// @class RtNode
/// TODO: Docs
class RtNode : public RtElement
{
public:
    /// Constructor attaching and object to the API.
    RtNode(const RtObject& obj);

    /// Create a new node instance of the given nodedef
    /// and add it to a stage.
    static RtObject create(const RtToken& name, RtObject nodedef, RtObject stage);

    /// Return the type for this object.
    RtApiType getApiType() const override;

    /// Return the node category for this node.
    const RtToken& getCategory() const;

    /// Return an input port by name, or a null object
    /// if no such input attribute exists.
    RtPort getInputPort(const RtToken& name) const;

    /// Return an output attribute by name, or a null object
    /// if no such output attribute exists.
    RtPort getOutputPort(const RtToken& name) const;

    /// Make a new connection between two ports.
    static void connect(const RtPort& source, const RtPort& dest);

    /// Break a connection between two ports.
    static void disconnect(const RtPort& source, const RtPort& dest);
};

}

#endif
