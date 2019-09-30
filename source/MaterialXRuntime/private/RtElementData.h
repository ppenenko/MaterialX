//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RT_ELEMENTDATA_H
#define MATERIALX_RT_ELEMENTDATA_H

#include <MaterialXRuntime/private/RtObjectData.h>
#include <MaterialXRuntime/private/RtAttributeData.h>

#include <MaterialXRuntime/RtObject.h>

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

    void setName(const RtToken& name)
    {
        _name = name;
    }

    void addAttribute(RtDataHandle attr);

    size_t numAttributes() const
    {
        return _attributes.size();
    }

    RtDataHandle getAttribute(const RtToken& name) const
    {
        auto it = _attributesByName.find(name);
        return it != _attributesByName.end() ? _attributes[it->second] : nullptr;
    }

protected:
    // Short syntax attribute getter for convenience.
    RtAttributeData* attribute(const RtToken& name) { return (RtAttributeData*)getAttribute(name).get(); }

    RtToken _name;
    RtDataHandleArray _attributes;
    RtDataHandleNameMap _attributesByName;
};

}

#endif
