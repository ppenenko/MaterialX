//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtNodeDef.h>
#include <MaterialXRuntime/RtObject.h>

#include <MaterialXRuntime/Private/PrvNodeDef.h>
#include <MaterialXRuntime/Private/PrvStage.h>

namespace MaterialX
{

RtNodeDef::RtNodeDef(const RtObject& obj) :
    RtElement(obj)
{
}

RtObject RtNodeDef::create(const RtToken& name, const RtToken& category, RtObject parent)
{
    PrvObjectHandle nodedef = PrvNodeDef::create(name, category);

    if (parent.isValid())
    {
        if (!parent.hasApi(RtApiType::STAGE))
        {
            throw ExceptionRuntimeError("Given parent object is not a valid stage");
        }
        RtApiBase::data(parent)->asA<PrvStage>()->addElement(nodedef);
    }

    return RtApiBase::object(nodedef);
}

RtApiType RtNodeDef::getApiType() const
{
    return RtApiType::NODEDEF;
}

const RtToken& RtNodeDef::getCategory() const
{
    return data()->asA<PrvNodeDef>()->getCategory();
}

void RtNodeDef::addPortDef(RtObject attr)
{
    return data()->asA<PrvNodeDef>()->addPortDef(RtApiBase::data(attr));
}

RtObject RtNodeDef::getPortDef(const RtToken& name) const
{
    PrvObjectHandle portdef = data()->asA<PrvNodeDef>()->getElement(name);
    return RtApiBase::object(portdef);
}

RtObject RtNodeDef::getPortDef(size_t index) const
{
    PrvObjectHandle portdef = data()->asA<PrvNodeDef>()->getElement(index);
    return RtApiBase::object(portdef);
}

size_t RtNodeDef::numPorts() const
{
    return data()->asA<PrvNodeDef>()->numElements();
}

size_t RtNodeDef::getPortIndex(const RtToken& name) const
{
    return data()->asA<PrvNodeDef>()->getElementIndex(name);
}

}
