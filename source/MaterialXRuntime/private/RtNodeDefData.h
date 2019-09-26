//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RT_NODEDEFDATA_H
#define MATERIALX_RT_NODEDEFDATA_H

#include <MaterialXRuntime/private/RtElementData.h>
#include <MaterialXRuntime/private/RtAttributeData.h>

#include <MaterialXRuntime/RtObject.h>
#include <MaterialXRuntime/RtValue.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class RtNodeDefData : public RtElementData
{
public:
    RtNodeDefData(const RtToken& name, const RtToken& category);

    ~RtNodeDefData();

    const RtToken& getCategory() const
    {
        return _category;
    }

    const RtAttributeData* addAttribute(const RtToken& name, const RtToken& type, const RtValue& value, uint32_t flags)
    {
        auto it = _attributesByName.find(name);
        if (it != _attributesByName.end())
        {
            throw ExceptionRuntimeError("An attribute named '" + name + "' already exists for nodedef '" + getName() + "'");
        }

        RtAttributeData* attr = new RtAttributeData(name, type, value, flags);
        _attributesByName[name] = _attributes.size();
        _attributes.push_back(attr);

        return attr;
    }

    size_t numAttributes() const
    {
        return _attributes.size();
    }

    const RtAttributeData* getAttribute(const RtToken& name) const
    {
        auto it = _attributesByName.find(name);
        return it != _attributesByName.end() ? _attributes[it->second] : nullptr;
    }

    const RtAttributeData* getAttribute(size_t index) const
    {
        return _attributes.size() < index ? _attributes[index] : nullptr;
    }

    size_t getAttributeIndex(const RtToken& name) const
    {
        auto it = _attributesByName.find(name);
        return it != _attributesByName.end() ? it->second : INVALID_INDEX;
    }

    static const size_t INVALID_INDEX;

protected:
    RtToken _category;
    vector<RtAttributeData*> _attributes;
    std::unordered_map<RtToken, size_t> _attributesByName;
};

}

#endif
