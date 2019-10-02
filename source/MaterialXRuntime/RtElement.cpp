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

void RtElement::addAttribute(const RtToken& name, const RtToken& type, const RtValue& value)
{
    RtElementData* elem = data()->asA<RtElementData>();
    elem->addAttribute(name, type, value);
}

const RtAttribute* RtElement::getAttribute(const RtToken& name) const
{
    RtElementData* elem = data()->asA<RtElementData>();
    return elem->getAttribute(name);
}

RtAttribute* RtElement::getAttribute(const RtToken& name)
{
    RtElementData* elem = data()->asA<RtElementData>();
    return elem->getAttribute(name);
}

const RtAttribute* RtElement::getAttribute(size_t index) const
{
    RtElementData* elem = data()->asA<RtElementData>();
    return elem->getAttribute(index);
}

RtAttribute* RtElement::getAttribute(size_t index)
{
    RtElementData* elem = data()->asA<RtElementData>();
    return elem->getAttribute(index);
}

size_t RtElement::numAttributes() const
{
    return data()->asA<RtElementData>()->numAttributes();
}

}
