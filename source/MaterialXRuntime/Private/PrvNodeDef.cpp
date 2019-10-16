//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/Private/PrvNodeDef.h>
#include <MaterialXRuntime/Private/PrvPortDef.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

PrvNodeDef::PrvNodeDef(const RtToken& name, const RtToken& category) :
    PrvCompoundElement(RtObjType::NODEDEF, name),
    _category(category)
{
}

PrvObjectHandle PrvNodeDef::create(const RtToken& name, const RtToken& category)
{
    return std::make_shared<PrvNodeDef>(name, category);
}

void PrvNodeDef::addPort(PrvObjectHandle portdef)
{
    if (!portdef->hasApi(RtApiType::PORTDEF))
    {
        throw ExceptionRuntimeError("Given object is not a valid portdef");
    }

    PrvPortDef* pd = portdef->asA<PrvPortDef>();
    auto it = _elementsByName.find(pd->getName());
    if (it != _elementsByName.end())
    {
        throw ExceptionRuntimeError("A port named '" + pd->getName().str() + "' already exists for nodedef '" + getName().str() + "'");
    }

    _elements.push_back(portdef);
    _elementsByName[pd->getName()] = portdef;
}

}
