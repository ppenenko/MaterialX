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
    return RtApiBase::object( RtStageData::create(name) );
}

void RtStage::clear()
{
    data()->asA<RtStageData>()->clear();
}

}
