//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtNodeDef.h>
#include <MaterialXRuntime/RtObject.h>

#include <MaterialXRuntime/private/RtNodeDefData.h>
#include <MaterialXRuntime/private/RtStageData.h>

namespace MaterialX
{

RtNodeDef::RtNodeDef(const RtObject& obj) :
    RtElement(obj)
{
}

RtObject RtNodeDef::create(const RtToken& name, const RtToken& category, RtObject stage)
{
    if (!stage.hasApi(RtApiType::STAGE))
    {
        throw ExceptionRuntimeError("Given object is not a valid stage");
    }

    RtStageData* stagedata = RtApiBase::data(stage)->asA<RtStageData>();
    // TODO: Check if name exists

    RtDataHandle nodedef = RtNodeDefData::create(name, category);
    stagedata->addNodeDef(nodedef);

    return RtApiBase::object(nodedef);
}

RtApiType RtNodeDef::getApiType() const
{
    return RtApiType::NODEDEF;
}

const RtToken& RtNodeDef::getCategory() const
{
    return data()->asA<RtNodeDefData>()->getCategory();
}

void RtNodeDef::addPortDef(RtObject attr)
{
    return data()->asA<RtNodeDefData>()->addPortDef(RtApiBase::data(attr));
}

RtObject RtNodeDef::getPortDef(const RtToken& name) const
{
    RtDataHandle portdef = data()->asA<RtNodeDefData>()->getPortDef(name);
    return RtApiBase::object(portdef);
}

RtObject RtNodeDef::getPortDef(size_t index) const
{
    RtDataHandle portdef = data()->asA<RtNodeDefData>()->getPortDef(index);
    return RtApiBase::object(portdef);
}

size_t RtNodeDef::numPorts() const
{
    return data()->asA<RtNodeDefData>()->numPorts();
}

}
