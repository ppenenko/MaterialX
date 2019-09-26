//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtElement.h>

#include <MaterialXRuntime/private/RtElementData.h>

namespace MaterialX
{

RtElement::RtElement(const RtObject& obj)
{
    setObject(obj);
}

RtApiType RtElement::getApiType() const
{
    return RtApiType::ELEMENT;
}

bool RtElement::isSupported(RtObjType type) const
{
    // This API supports all objects
    return type != RtObjType::INVALID;
}

const RtToken& RtElement::getName() const
{
    return data()->asA<RtElementData>()->getName();
}

RtMetaData* RtElement::getMetaData(const RtToken& name)
{
    return data()->asA<RtElementData>()->getMetaData(name);
}

RtMetaData* RtElement::addMetaData(const RtToken& name, const RtToken& type, const RtValue& value)
{
    return data()->asA<RtElementData>()->addMetaData(name, type, value);
}

}
