//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/private/RtElementData.h>
#include <MaterialXRuntime/private/RtNodeData.h>
#include <MaterialXRuntime/private/RtNodeDefData.h>

#include <MaterialXCore/Util.h>

namespace MaterialX
{

const string PATH_SEPARATOR = "/";

RtElementData::RtElementData(RtObjType objType, const RtToken& name) :
    RtObjectData(objType),
    _name(name)
{
}

void RtElementData::addAttribute(const RtToken& name, const RtToken& type, const RtValue& value)
{
    auto it = _attributesByName.find(name);
    if (it != _attributesByName.end())
    {
        throw ExceptionRuntimeError("An attribute named '" + name + "' already exists for '" + getName() + "'");
    }
    _attributesByName[name] = _attributes.size();
    _attributes.push_back(RtAttribute(name, type, value));
}


RtCompoundElementData::RtCompoundElementData(RtObjType objType, const RtToken& name) :
    RtElementData(objType, name)
{
}

void RtCompoundElementData::addElement(RtDataHandle elem)
{
    if (!elem->hasApi(RtApiType::ELEMENT))
    {
        throw ExceptionRuntimeError("Given object is not a valid element");
    }
    RtElementData* e = elem->asA<RtElementData>();
    auto it = _elementsByName.find(e->getName());
    if (it != _elementsByName.end())
    {
        throw ExceptionRuntimeError("An element named '" + e->getName() + "' already exists in '" + getName() + "'");
    }
    _elementsByName[e->getName()] = _elements.size();
    _elements.push_back(elem);
}

void RtCompoundElementData::removeElement(const RtToken& name)
{
    const size_t index = getElementIndex(name);
    if (index == INVALID_INDEX)
    {
        throw ExceptionRuntimeError("An element named '" + name + "' doesn't exists in '" + getName() + "'");
    }
    _attributes.erase(_attributes.begin() + index);
    _attributesByName.erase(name);
}

RtDataHandle RtCompoundElementData::findElement(const RtString& path) const
{
    const StringVec elementNames = splitString(path, PATH_SEPARATOR);
    if (elementNames.empty())
    {
        return nullptr;
    }

    size_t i = 0;
    RtDataHandle elem = getElement(elementNames[i++]);

    while (elem != nullptr && i < elementNames.size())
    {
        if (elem->hasApi(RtApiType::COMPOUND_ELEMENT))
        {
            elem = elem->asA<RtCompoundElementData>()->getElement(elementNames[i]);
        }
        else if (elem->hasApi(RtApiType::NODE))
        {
            RtNodeData* node = elem->asA<RtNodeData>();
            RtNodeDefData* nodedef = node->getNodeDef()->asA<RtNodeDefData>();
            elem = nodedef->getElement(elementNames[i]);
        }
        ++i;
    }

    return elem;
}

}
