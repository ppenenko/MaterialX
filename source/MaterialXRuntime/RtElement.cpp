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

void RtElement::addAttribute(RtObject attr)
{
    RtElementData* elem = data()->asA<RtElementData>();
    elem->addAttribute(RtApiBase::data(attr));
}

RtObject RtElement::getAttribute(const RtToken& name) const
{
    RtElementData* elem = data()->asA<RtElementData>();
    return RtApiBase::object(elem->getAttribute(name));
}

size_t RtElement::numAttributes() const
{
    return data()->asA<RtElementData>()->numAttributes();
}

}
