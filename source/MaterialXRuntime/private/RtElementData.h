//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTELEMENTDATA_H
#define MATERIALX_RTELEMENTDATA_H

#include <MaterialXRuntime/private/RtObjectData.h>

#include <MaterialXRuntime/RtElement.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class RtElementData : public RtObjectData
{
public:
    RtElementData(RtObjType objType, const RtToken& name);

    const RtToken& getName() const
    {
        return _name;
    }

    void addAttribute(const RtToken& name, const RtToken& type, const RtValue& value);

    const RtAttribute* getAttribute(const RtToken& name) const
    {
        auto it = _attributesByName.find(name);
        return it != _attributesByName.end() ? &_attributes[it->second] : nullptr;
    }

    RtAttribute* getAttribute(const RtToken& name)
    {
        auto it = _attributesByName.find(name);
        return it != _attributesByName.end() ? &_attributes[it->second] : nullptr;
    }

    const RtAttribute* getAttribute(size_t index) const
    {
        return index < _attributes.size() ? &_attributes[index] : nullptr;
    }

    RtAttribute* getAttribute(size_t index)
    {
        return index < _attributes.size() ? &_attributes[index] : nullptr;
    }

    size_t numAttributes() const
    {
        return _attributes.size();
    }


protected:
    void setName(const RtToken& name)
    {
        _name = name;
    }

    RtToken _name;
    vector<RtAttribute> _attributes;
    RtTokenIndexMap _attributesByName;
    friend class RtStageData;
};

}

#endif
