//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/private/RtElementData.h>

namespace MaterialX
{

RtElementData::RtElementData(RtObjType objType, const RtToken& name) :
    RtObjectData(objType),
    _name(name)
{
}

void RtElementData::addAttribute(const RtToken& name, const RtToken& type, const RtValue& value)
{
    auto it = _attributesByName.find(name);
    if (it != _attributesByName.end())
    {
        throw ExceptionRuntimeError("An attribute named '" + name + "' already exists for '" + getName() + "'");
    }
    _attributesByName[name] = _attributes.size();
    _attributes.push_back(RtAttribute(name, type, value));
}

}
