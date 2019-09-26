//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtNodeDef.h>
#include <MaterialXRuntime/RtAttribute.h>
#include <MaterialXRuntime/RtObject.h>

#include <MaterialXRuntime/private/RtNodeDefData.h>

namespace MaterialX
{

RtNodeDef::RtNodeDef(const RtObject& obj) :
    RtElement(obj)
{
}

RtApiType RtNodeDef::getApiType() const
{
    return RtApiType::NODEDEF;
}

RtAttribute RtNodeDef::addAttribute(const RtToken& name, const RtToken& type, uint32_t flags, const RtValue& value)
{
    RtNodeDefData* ptr = data()->asA<RtNodeDefData>();
    const RtAttributeData* newAttr = ptr->addAttribute(name, type, value, flags);
    return RtApiBase::api<RtAttribute>(const_cast<RtAttributeData*>(newAttr));
}

RtAttribute RtNodeDef::getAttribute(const RtToken& name) const
{
    RtNodeDefData* ptr = data()->asA<RtNodeDefData>();
    const RtAttributeData* attr = ptr->getAttribute(name);
    return RtApiBase::api<RtAttribute>(const_cast<RtAttributeData*>(attr));
}

}
