//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_PRVNODE_H
#define MATERIALX_PRVNODE_H

#include <MaterialXRuntime/Private/PrvNodeDef.h>

#include <MaterialXRuntime/RtNode.h>
#include <MaterialXRuntime/RtValue.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

using RtPortVec = vector<RtPort>;

class PrvNode : public PrvElement
{
public:
    PrvNode(const RtToken& name, const PrvObjectHandle& nodedef);

    static PrvObjectHandle createNew(const RtToken& name, const PrvObjectHandle& nodedef);

    PrvObjectHandle getNodeDef() const
    {
        return _nodedef;
    }

    const RtToken& getCategory() const
    {
        return nodedef()->getCategory();
    }

    size_t numPorts() const
    {
        return nodedef()->numPorts();
    }

    size_t numOutputs() const
    {
        return nodedef()->numOutputs();
    }

    RtPort getPort(size_t index)
    {
        PrvNodeDef* nodedef = _nodedef->asA<PrvNodeDef>();
        PrvPortDef* portdef = nodedef->port(index);
        return portdef ? RtPort(shared_from_this(), index) : RtPort();
    }

    RtPort findPort(const RtToken& name)
    {
        const size_t index = findPortIndex(name);
        return index != INVALID_INDEX ? RtPort(shared_from_this(), index) : RtPort();
    }

    size_t findPortIndex(const RtToken& name)
    {
        auto it = _portIndices.find(name);
        return it != _portIndices.end() ? it->second : INVALID_INDEX;
    }

    static void connect(const RtPort& source, const RtPort& dest);

    static void disconnect(const RtPort& source, const RtPort& dest);

    // Short syntax getter for convenience.
    PrvNodeDef* nodedef() { return (PrvNodeDef*)_nodedef.get(); }
    const PrvNodeDef* nodedef() const { return (PrvNodeDef*)_nodedef.get(); }

protected:
    struct Port
    {
        Port();
        RtValue value;
        RtToken colorspace;
        RtToken unit;
        RtPortVec connections;
    };

    PrvObjectHandle _nodedef;
    vector<Port> _ports;
    RtTokenMap<size_t> _portIndices;
    friend class RtPort;
};

}

#endif
