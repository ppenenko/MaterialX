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

RtValue& RtAttribute::getValue()
{
    return data()->asA<RtAttributeData>()->getValue();
}

void RtAttribute::setValue(const RtValue& v)
{
    data()->asA<RtAttributeData>()->getValue() = v;
}

void RtAttribute::setValue(bool v)
{
    data()->asA<RtAttributeData>()->getValue().asBool() = v;
}

void RtAttribute::setValue(int v)
{
    data()->asA<RtAttributeData>()->getValue().asInt() = v;
}

void RtAttribute::setValue(unsigned int v)
{
    data()->asA<RtAttributeData>()->getValue().asUInt() = v;
}

void RtAttribute::setValue(float v)
{
    data()->asA<RtAttributeData>()->getValue().asFloat() = v;
}

void RtAttribute::setValue(const Color3& v)
{
    data()->asA<RtAttributeData>()->getValue().asColor3() = v;
}

void RtAttribute::setValue(const Vector4& v)
{
    data()->asA<RtAttributeData>()->getValue().asVector4() = v;
}

void RtAttribute::setValue(void* v)
{
    data()->asA<RtAttributeData>()->getValue().asPtr() = v;
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

}
