//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/private/RtElementData.h>
#include <MaterialXRuntime/private/RtAttributeData.h>

namespace MaterialX
{

RtElementData::RtElementData(RtObjType objType, const RtToken& name) :
    RtObjectData(objType),
    _name(name)
{
}

void RtElementData::addAttribute(RtDataHandle attr)
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

}
