//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtAttribute.h>

#include <MaterialXRuntime/private/RtAttributeData.h>

namespace MaterialX
{

RtAttribute::RtAttribute(const RtObject& obj) : 
    RtElement(obj)
{
}

RtApiType RtAttribute::getApiType() const
{
    return RtApiType::ATTRIBUTE;
}

const RtToken& RtAttribute::getType() const
{
    return data()->asA<RtAttributeData>()->getType();
}

const RtValue& RtAttribute::getValue() const
{
    return data()->asA<RtAttributeData>()->getValue();
}

int32_t RtAttribute::getFlags() const
{
    return data()->asA<RtAttributeData>()->getFlags();
}

bool RtAttribute::isInput() const
{
    return data()->asA<RtAttributeData>()->isInput();
}

bool RtAttribute::isOutput() const
{
    return data()->asA<RtAttributeData>()->isOutput();
}

bool RtAttribute::isConnectable() const
{
    return data()->asA<RtAttributeData>()->isConnectable();
}

bool RtAttribute::isConnectableTo(const RtAttribute& other) const
{
    return data()->asA<RtAttributeData>()->isConnectableTo(other.data()->asA<RtAttributeData>());
}

}
