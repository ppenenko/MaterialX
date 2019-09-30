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

    static RtDataHandle create(const RtToken& name, const RtToken& category);

    const RtToken& getCategory() const
    {
        return _category;
    }

    void addAttribute(RtDataHandle attr)
    {
        if (!attr->hasApi(RtApiType::ATTRIBUTE))
        {
            throw ExceptionRuntimeError("Given object is not a valid attribute");
        }
        RtAttributeData* a = attr->asA<RtAttributeData>();
        auto it = _attributesByName.find(a->getName());
        if (it != _attributesByName.end())
        {
            throw ExceptionRuntimeError("An attribute named '" + a->getName() + "' already exists for nodedef '" + getName() + "'");
        }
        _attributesByName[a->getName()] = _attributes.size();
        _attributes.push_back(attr);
    }

    size_t numAttributes() const
    {
        return _attributes.size();
    }

    RtDataHandle getAttribute(const RtToken& name) const
    {
        auto it = _attributesByName.find(name);
        return it != _attributesByName.end() ? _attributes[it->second] : nullptr;
    }

    RtDataHandle getAttribute(size_t index) const
    {
        return index < _attributes.size() ? _attributes[index] : nullptr;
    }

    size_t getAttributeIndex(const RtToken& name) const
    {
        auto it = _attributesByName.find(name);
        return it != _attributesByName.end() ? it->second : INVALID_INDEX;
    }

    static const size_t INVALID_INDEX;

protected:
    // Short syntax attribute getters for convenience.
    inline RtAttributeData* attribute(const RtToken& name) { return (RtAttributeData*)getAttribute(name).get(); }
    inline RtAttributeData* attribute(size_t index) { return (RtAttributeData*)getAttribute(index).get(); }

    RtToken _category;
    RtDataHandleArray _attributes;
    RtDataHandleNameMap _attributesByName;
    friend class RtNodeData;
    friend class RtPort;
};

}

#endif
