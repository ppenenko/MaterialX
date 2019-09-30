//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RT_NODEDATA_H
#define MATERIALX_RT_NODEDATA_H

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

    RtPort getInputPort(const RtToken& name);
    RtPort getOutputPort(const RtToken& name);

protected:
    // Short syntax getter for convenience.
    RtNodeDefData* nodedef() { return (RtNodeDefData*)_nodedef.get(); }
    const RtNodeDefData* nodedef() const { return (RtNodeDefData*)_nodedef.get(); }

    RtDataHandle _nodedef;
    vector<RtValue> _values;
    vector<RtPort> _connections;
    friend class RtPort;
};

}

#endif
