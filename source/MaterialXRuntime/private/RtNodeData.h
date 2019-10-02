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

    size_t numPorts() const
    {
        return nodedef()->numPorts();
    }

    RtPort getPort(size_t index);
    RtPort getPort(const RtToken& name);

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
