//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtNodeDef.h>
#include <MaterialXRuntime/RtAttribute.h>
#include <MaterialXRuntime/RtObject.h>

#include <MaterialXRuntime/private/RtNodeDefData.h>
#include <MaterialXRuntime/private/RtStageData.h>

namespace MaterialX
{

RtNodeDef::RtNodeDef(const RtObject& obj) :
    RtElement(obj)
{
}

RtApiType RtNodeDef::getApiType() const
{
    return RtApiType::NODEDEF;
}

const RtToken& RtNodeDef::getCategory() const
{
    return data()->asA<RtNodeDefData>()->getCategory();
}

void RtNodeDef::addAttribute(RtObject attr)
{
    RtNodeDefData* nodedef = data()->asA<RtNodeDefData>();
    nodedef->addAttribute(RtApiBase::data(attr));
}

RtObject RtNodeDef::getAttribute(const RtToken& name) const
{
    RtNodeDefData* nodedef = data()->asA<RtNodeDefData>();
    return RtApiBase::object(nodedef->getAttribute(name));
}

size_t RtNodeDef::numAttributes() const
{
    return data()->asA<RtNodeDefData>()->numAttributes();
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

}
