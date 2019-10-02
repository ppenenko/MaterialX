//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/private/RtNodeDefData.h>
#include <MaterialXRuntime/private/RtPortDefData.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

RtNodeDefData::RtNodeDefData(const RtToken& name, const RtToken& category) :
    RtElementData(RtObjType::NODEDEF, name),
    _category(category)
{
}

RtDataHandle RtNodeDefData::create(const RtToken& name, const RtToken& category)
{
    return std::make_shared<RtNodeDefData>(name, category);
}

void RtNodeDefData::addPortDef(RtDataHandle portdef)
{
    if (!portdef->hasApi(RtApiType::PORTDEF))
    {
        throw ExceptionRuntimeError("Given object is not a valid portdef");
    }
    RtPortDefData* pd = portdef->asA<RtPortDefData>();
    auto it = _portdefsByName.find(pd->getName());
    if (it != _portdefsByName.end())
    {
        throw ExceptionRuntimeError("A port named '" + pd->getName() + "' already exists for nodedef '" + getName() + "'");
    }
    _portdefsByName[pd->getName()] = _portdefs.size();
    _portdefs.push_back(portdef);
}


}
