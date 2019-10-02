//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/private/RtPortDefData.h>

namespace MaterialX
{

RtPortDefData::RtPortDefData(const RtToken& name, const RtToken& type, const RtValue& value, uint32_t flags) :
RtElementData(RtObjType::PORTDEF, name),
_type(type),
_value(value),
_flags(flags)
{
}

RtDataHandle RtPortDefData::create(const RtToken& name, const RtToken& type, const RtValue& value, uint32_t flags)
{
    return std::make_shared<RtPortDefData>(name, type, value, flags);
}

}
