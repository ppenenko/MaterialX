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
    RtCompoundElementData(RtObjType::NODEDEF, name),
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
    auto it = _elementsByName.find(pd->getName());
    if (it != _elementsByName.end())
    {
        throw ExceptionRuntimeError("A port named '" + pd->getName() + "' already exists for nodedef '" + getName() + "'");
    }
    _elementsByName[pd->getName()] = _elements.size();
    _elements.push_back(portdef);
}

}
