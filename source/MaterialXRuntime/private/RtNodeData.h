//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RT_NODEDATA_H
#define MATERIALX_RT_NODEDATA_H

#include <MaterialXRuntime/private/RtNodeDefData.h>

#include <MaterialXRuntime/RtObject.h>
#include <MaterialXRuntime/RtValue.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class RtPortData
{
public:
    RtPortData(RtNodeData* node, size_t index) :
        _node(node),
        _index(index)
    {
    }



    bool connectTo(RtPortData* other)
    {


    }

    RtAttributeData* getAttribute()
    {
        _node->getNodeDef();
    }

protected:
    RtNodeData* _node;
    size_t _index;
};

class RtNodeData : public RtElementData
{
public:
    RtNodeData(const RtToken& name, const RtNodeDefData* nodedef) :
        RtElementData(RtObjType::NODE, name),
        _nodedef(nodedef)
    {
    }

    const RtNodeDefData* getNodeDef() const
    {
        return _nodedef;
    }

    RtPortData* getInputPort(const RtToken& name)
    {
        const RtAttribute* attr = _nodedef->getInputAttr(name);
    }

    RtAttributeData* getOutputPort(const RtToken& name)
    {
    }

protected:
    const RtNodeDefData* _nodedef;
    vector<RtValue> _values;
};

}

#endif
