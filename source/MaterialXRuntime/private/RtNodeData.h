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

class RtNodeData;

class RtPortData
{
public:
    RtPortData();
    RtPortData(RtNodeData* node, size_t index);

    bool isValid() const
    {
        return _node != nullptr;
    }

    const RtAttributeData* getAttribute() const;

    RtValue& getValue();

    void setValue(const RtValue& v);
    void setValue(bool v);
    void setValue(int v);
    void setValue(unsigned int v);
    void setValue(float v);
    void setValue(const Color3& v);
    void setValue(const Vector4& v);
    void setValue(void* v);

    void connectTo(RtPortData& other);
    void disconnect();

protected:
    RtNodeData* _node;
    size_t _index;
};

class RtNodeData : public RtElementData
{
public:
    RtNodeData(const RtToken& name, const RtNodeDefData* nodedef);

    const RtNodeDefData* getNodeDef() const
    {
        return _nodedef;
    }

    RtPortData getInputPort(const RtToken& name)
    {
        const size_t index = _nodedef->getAttributeIndex(name);
        const RtAttributeData* attr = _nodedef->getAttribute(index);
        return attr && attr->isInput() ? RtPortData(this, index) : RtPortData();
    }

    RtPortData getOutputPort(const RtToken& name)
    {
        const size_t index = _nodedef->getAttributeIndex(name);
        const RtAttributeData* attr = _nodedef->getAttribute(index);
        return attr && attr->isOutput() ? RtPortData(this, index) : RtPortData();
    }

protected:
    friend class RtPortData;
    const RtNodeDefData* _nodedef;
    vector<RtValue> _values;
    vector<RtPortData> _connections;
};

}

#endif
