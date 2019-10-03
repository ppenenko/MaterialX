//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/private/RtStageData.h>
#include <MaterialXRuntime/private/RtNodeDefData.h>
#include <MaterialXRuntime/private/RtNodeData.h>
#include <MaterialXRuntime/private/RtNodeGraphData.h>

#include <MaterialXRuntime/RtObject.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

RtStageData::RtStageData(const RtToken& name) :
    RtCompoundElementData(RtObjType::STAGE, name)
{
}

RtDataHandle RtStageData::create(const RtToken& name)
{
    return std::make_shared<RtStageData>(name);
}

}
