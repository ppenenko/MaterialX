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
    return data()->asA<RtAttributeData>()->setValue(v);
}

void RtAttribute::setValue(bool v)
{
    return data()->asA<RtAttributeData>()->setValue(v);
}

void RtAttribute::setValue(int v)
{
    return data()->asA<RtAttributeData>()->setValue(v);
}

void RtAttribute::setValue(unsigned int v)
{
    return data()->asA<RtAttributeData>()->setValue(v);
}

void RtAttribute::setValue(float v)
{
    return data()->asA<RtAttributeData>()->setValue(v);
}

void RtAttribute::setValue(const Color3& v)
{
    return data()->asA<RtAttributeData>()->setValue(v);
}

void RtAttribute::setValue(const Vector4& v)
{
    return data()->asA<RtAttributeData>()->setValue(v);
}

void RtAttribute::setValue(void* v)
{
    return data()->asA<RtAttributeData>()->setValue(v);
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

RtObject RtAttribute::create(const RtToken& name, const RtToken& type, const RtValue& value, uint32_t flags)
{
    return RtApiBase::object(RtAttributeData::create(name, type, value, flags));
}

}
