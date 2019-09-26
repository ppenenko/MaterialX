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

RtStage RtStage::create(const RtToken& name)
{
    return RtApiBase::api<RtStage>(new RtStageData(name));
}

RtNodeDef RtStage::addNodeDef(const RtToken& name)
{
    RtStageData* ptr = data()->asA<RtStageData>();
    RtNodeDefData* newNodeDef = ptr->addNodeDef(name);
    return RtApiBase::api<RtNodeDef>(newNodeDef);
}

void RtStage::clear()
{
    data()->asA<RtStageData>()->clear();
}

}
