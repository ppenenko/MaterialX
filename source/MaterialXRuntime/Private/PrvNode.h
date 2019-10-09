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

    static PrvObjectHandle create(const RtToken& name, const PrvObjectHandle& nodedef);

    PrvObjectHandle getNodeDef() const
    {
        return _nodedef;
    }

    const RtToken& getCategory() const
    {
        return nodedef()->getCategory();
    }

    RtPort getPort(const RtToken& name)
    {
        PrvNodeDef* nodedef = _nodedef->asA<PrvNodeDef>();
        const size_t index = nodedef->getElementIndex(name);
        PrvPortDef* portdef = nodedef->portdef(index);
        return portdef ? RtPort(shared_from_this(), index) : RtPort();
    }

    RtPort getPort(size_t index)
    {
        PrvNodeDef* nodedef = _nodedef->asA<PrvNodeDef>();
        PrvPortDef* portdef = nodedef->portdef(index);
        return portdef ? RtPort(shared_from_this(), index) : RtPort();
    }

    size_t numPorts() const
    {
        return nodedef()->numElements();
    }

    static void connect(const RtPort& source, const RtPort& dest);

    static void disconnect(const RtPort& source, const RtPort& dest);

    // Short syntax getter for convenience.
    PrvNodeDef* nodedef() { return (PrvNodeDef*)_nodedef.get(); }
    const PrvNodeDef* nodedef() const { return (PrvNodeDef*)_nodedef.get(); }

protected:
    PrvObjectHandle _nodedef;
    vector<RtValue> _values;
    vector<RtPortVec> _connections;
    friend class RtPort;
};

}

#endif
