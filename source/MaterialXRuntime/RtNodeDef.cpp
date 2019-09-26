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

RtAttribute RtNodeDef::addInputAttr(const RtToken& name, const RtToken& type, const RtValue& value, bool connectable)
{
    RtNodeDefData* ptr = data()->asA<RtNodeDefData>();
    const uint32_t flags = RtAttrFlag::CONNECTABLE & uint32_t(connectable);
    RtAttributeData* newAttr = ptr->addInputAttr(name, type, value, flags);
    return RtApiBase::api<RtAttribute>(newAttr);
}

RtAttribute RtNodeDef::addOutputAttr(const RtToken& name, const RtToken& type, const RtValue& value, bool connectable)
{
    RtNodeDefData* ptr = data()->asA<RtNodeDefData>();
    const uint32_t flags = RtAttrFlag::CONNECTABLE & uint32_t(connectable);
    RtAttributeData* newAttr = ptr->addOutputAttr(name, type, value, flags);
    return RtApiBase::api<RtAttribute>(newAttr);
}

RtAttribute RtNodeDef::getInputAttr(const RtToken& name) const
{
    RtNodeDefData* ptr = data()->asA<RtNodeDefData>();
    return RtApiBase::api<RtAttribute>(ptr->getInputAttr(name));
}

RtAttribute RtNodeDef::getOutputAttr(const RtToken& name) const
{
    RtNodeDefData* ptr = data()->asA<RtNodeDefData>();
    return RtApiBase::api<RtAttribute>(ptr->getOutputAttr(name));
}

}
