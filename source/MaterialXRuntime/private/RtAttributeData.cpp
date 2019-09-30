//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/private/RtAttributeData.h>

namespace MaterialX
{

RtAttributeData::RtAttributeData() :
    RtObjectData(RtObjType::ATTRIBUTE),
    _flags(0)
{
}

RtAttributeData::RtAttributeData(const RtToken& name, const RtToken& type, const RtValue& value, uint32_t flags) :
    RtObjectData(RtObjType::ATTRIBUTE),
    _name(name),
    _type(type),
    _value(value),
    _flags(flags)
{
}

RtDataHandle RtAttributeData::create(const RtToken& name, const RtToken& type, const RtValue& value, uint32_t flags)
{
    return std::make_shared<RtAttributeData>(name, type, value, flags);
}

}
