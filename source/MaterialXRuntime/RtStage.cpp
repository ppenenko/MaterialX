//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtStage.h>

#include <MaterialXRuntime/private/RtStageData.h>

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
    return RtApiBase::object(RtStageData::create(name));
}

void RtStage::addElement(RtObject elem)
{
    data()->asA<RtStageData>()->addElement(RtApiBase::data(elem));
}

void RtStage::removeElement(const RtToken& name)
{
    data()->asA<RtStageData>()->removeElement(name);
}

RtObject RtStage::getElement(const RtToken& name) const
{
    RtDataHandle elem = data()->asA<RtStageData>()->getElement(name);
    return RtApiBase::object(elem);
}

RtObject RtStage::getElement(size_t index) const
{
    RtDataHandle elem = data()->asA<RtStageData>()->getElement(index);
    return RtApiBase::object(elem);
}

size_t RtStage::numElements() const
{
    return data()->asA<RtStageData>()->numElements();
}

RtObject RtStage::findElement(const RtString& path) const
{
    RtDataHandle elem = data()->asA<RtStageData>()->findElement(path);
    return RtApiBase::object(elem);
}

void RtStage::addReference(RtObject stage)
{
    throw ExceptionRuntimeError("Not implemented");
}

void RtStage::removeReference(RtObject stage)
{
    throw ExceptionRuntimeError("Not implemented");
}

}
