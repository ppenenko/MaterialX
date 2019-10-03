//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTNODEDATA_H
#define MATERIALX_RTNODEDATA_H

#include <MaterialXRuntime/private/RtNodeDefData.h>

#include <MaterialXRuntime/RtNode.h>
#include <MaterialXRuntime/RtValue.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class RtNodeData : public RtElementData
{
public:
    RtNodeData(const RtToken& name, const RtDataHandle& nodedef);

    static RtDataHandle create(const RtToken& name, const RtDataHandle& nodedef);

    RtDataHandle getNodeDef() const
    {
        return _nodedef;
    }

    const RtToken& getCategory() const
    {
        return nodedef()->getCategory();
    }

    RtPort getPort(const RtToken& name)
    {
        RtNodeDefData* nodedef = _nodedef->asA<RtNodeDefData>();
        const size_t index = nodedef->getElementIndex(name);
        RtPortDefData* portdef = nodedef->portdef(index);
        return portdef ? RtPort(shared_from_this(), index) : RtPort();
    }

    RtPort getPort(size_t index)
    {
        RtNodeDefData* nodedef = _nodedef->asA<RtNodeDefData>();
        RtPortDefData* portdef = nodedef->portdef(index);
        return portdef ? RtPort(shared_from_this(), index) : RtPort();
    }

    size_t numPorts() const
    {
        return nodedef()->numElements();
    }

    static void connect(const RtPort& source, const RtPort& dest);

    static void disconnect(const RtPort& source, const RtPort& dest);

protected:
    // Short syntax getter for convenience.
    RtNodeDefData* nodedef() { return (RtNodeDefData*)_nodedef.get(); }
    const RtNodeDefData* nodedef() const { return (RtNodeDefData*)_nodedef.get(); }

    RtDataHandle _nodedef;
    vector<RtValue> _values;
    vector<RtPortArray> _connections;
    friend class RtPort;
};

}

#endif
