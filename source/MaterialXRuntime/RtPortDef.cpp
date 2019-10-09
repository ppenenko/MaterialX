//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtPortDef.h>

#include <MaterialXRuntime/Private/PrvPortDef.h>
#include <MaterialXRuntime/Private/PrvNodeDef.h>

namespace MaterialX
{

RtPortDef::RtPortDef(const RtObject& obj) : 
    RtElement(obj)
{
}

RtObject RtPortDef::create(const RtToken& name, const RtToken& type, const RtValue& value, uint32_t flags, RtObject parent)
{
    PrvObjectHandle portdef = PrvPortDef::create(name, type, value, flags);

    if (parent.isValid())
    {
        if (parent.hasApi(RtApiType::NODEDEF))
        {
            PrvObjectHandle parentData = RtApiBase::data(parent);
            parentData->asA<PrvNodeDef>()->addPortDef(portdef);
        }
        else
        {
            throw ExceptionRuntimeError("Parent object must be a nodedef");
        }
    }

    return RtApiBase::object(portdef);
}

RtApiType RtPortDef::getApiType() const
{
    return RtApiType::PORTDEF;
}

const RtToken& RtPortDef::getType() const
{
    return data()->asA<PrvPortDef>()->getType();
}

const RtValue& RtPortDef::getValue() const
{
    return data()->asA<PrvPortDef>()->getValue();
}

RtValue& RtPortDef::getValue()
{
    return data()->asA<PrvPortDef>()->getValue();
}

void RtPortDef::setValue(const RtValue& v)
{
    return data()->asA<PrvPortDef>()->setValue(v);
}

void RtPortDef::setValue(bool v)
{
    return data()->asA<PrvPortDef>()->setValue(v);
}

void RtPortDef::setValue(int v)
{
    return data()->asA<PrvPortDef>()->setValue(v);
}

void RtPortDef::setValue(unsigned int v)
{
    return data()->asA<PrvPortDef>()->setValue(v);
}

void RtPortDef::setValue(float v)
{
    return data()->asA<PrvPortDef>()->setValue(v);
}

void RtPortDef::setValue(const Color3& v)
{
    return data()->asA<PrvPortDef>()->setValue(v);
}

void RtPortDef::setValue(const Vector4& v)
{
    return data()->asA<PrvPortDef>()->setValue(v);
}

void RtPortDef::setValue(void* v)
{
    return data()->asA<PrvPortDef>()->setValue(v);
}

int32_t RtPortDef::getFlags() const
{
    return data()->asA<PrvPortDef>()->getFlags();
}

bool RtPortDef::isInput() const
{
    return data()->asA<PrvPortDef>()->isInput();
}

bool RtPortDef::isOutput() const
{
    return data()->asA<PrvPortDef>()->isOutput();
}

bool RtPortDef::isConnectable() const
{
    return data()->asA<PrvPortDef>()->isConnectable();
}

bool RtPortDef::isUniform() const
{
    return data()->asA<PrvPortDef>()->isUniform();
}

}
