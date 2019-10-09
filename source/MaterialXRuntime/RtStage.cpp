//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtStage.h>

#include <MaterialXRuntime/Private/PrvStage.h>

namespace MaterialX
{

RtStage::RtStage(const RtObject& obj) :
    RtElement(obj)
{
}

RtApiType RtStage::getApiType() const
{
    return RtApiType::STAGE;
}

RtObject RtStage::create(const RtToken& name)
{
    return RtApiBase::object(PrvStage::create(name));
}

void RtStage::addElement(RtObject elem)
{
    data()->asA<PrvStage>()->addElement(RtApiBase::data(elem));
}

void RtStage::removeElement(const RtToken& name)
{
    data()->asA<PrvStage>()->removeElement(name);
}

RtObject RtStage::getElement(const RtToken& name) const
{
    PrvObjectHandle elem = data()->asA<PrvStage>()->getElement(name);
    return RtApiBase::object(elem);
}

RtObject RtStage::findElement(const RtString& path) const
{
    PrvObjectHandle elem = data()->asA<PrvStage>()->findElement(path);
    return RtApiBase::object(elem);
}

void RtStage::addReference(RtObject stage)
{
    data()->asA<PrvStage>()->addReference(RtApiBase::data(stage));
}

void RtStage::removeReference(const RtToken& name)
{
    data()->asA<PrvStage>()->removeReference(name);
}

}
