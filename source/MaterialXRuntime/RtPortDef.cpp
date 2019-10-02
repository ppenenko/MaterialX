//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtPortDef.h>

#include <MaterialXRuntime/private/RtPortDefData.h>
#include <MaterialXRuntime/private/RtNodeDefData.h>

namespace MaterialX
{

RtPortDef::RtPortDef(const RtObject& obj) : 
    RtElement(obj)
{
}

RtObject RtPortDef::create(const RtToken& name, const RtToken& type, const RtValue& value, uint32_t flags, RtObject nodedef)
{
    if (!nodedef.hasApi(RtApiType::NODEDEF))
    {
        throw ExceptionRuntimeError("Given nodedef object is not a valid nodedef");
    }

    RtDataHandle portdef = RtPortDefData::create(name, type, value, flags);
    RtApiBase::data(nodedef)->asA<RtNodeDefData>()->addPortDef(portdef);

    return RtApiBase::object(portdef);
}

RtApiType RtPortDef::getApiType() const
{
    return RtApiType::PORTDEF;
}

const RtToken& RtPortDef::getType() const
{
    return data()->asA<RtPortDefData>()->getType();
}

const RtValue& RtPortDef::getValue() const
{
    return data()->asA<RtPortDefData>()->getValue();
}

RtValue& RtPortDef::getValue()
{
    return data()->asA<RtPortDefData>()->getValue();
}

void RtPortDef::setValue(const RtValue& v)
{
    return data()->asA<RtPortDefData>()->setValue(v);
}

void RtPortDef::setValue(bool v)
{
    return data()->asA<RtPortDefData>()->setValue(v);
}

void RtPortDef::setValue(int v)
{
    return data()->asA<RtPortDefData>()->setValue(v);
}

void RtPortDef::setValue(unsigned int v)
{
    return data()->asA<RtPortDefData>()->setValue(v);
}

void RtPortDef::setValue(float v)
{
    return data()->asA<RtPortDefData>()->setValue(v);
}

void RtPortDef::setValue(const Color3& v)
{
    return data()->asA<RtPortDefData>()->setValue(v);
}

void RtPortDef::setValue(const Vector4& v)
{
    return data()->asA<RtPortDefData>()->setValue(v);
}

void RtPortDef::setValue(void* v)
{
    return data()->asA<RtPortDefData>()->setValue(v);
}

int32_t RtPortDef::getFlags() const
{
    return data()->asA<RtPortDefData>()->getFlags();
}

bool RtPortDef::isInput() const
{
    return data()->asA<RtPortDefData>()->isInput();
}

bool RtPortDef::isOutput() const
{
    return data()->asA<RtPortDefData>()->isOutput();
}

bool RtPortDef::isConnectable() const
{
    return data()->asA<RtPortDefData>()->isConnectable();
}

bool RtPortDef::isUniform() const
{
    return data()->asA<RtPortDefData>()->isUniform();
}

}
