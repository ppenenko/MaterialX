//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/Private/PrvElement.h>
#include <MaterialXRuntime/Private/PrvNode.h>
#include <MaterialXRuntime/Private/PrvNodeDef.h>

#include <MaterialXCore/Util.h>

namespace MaterialX
{

const string PrvElement::PATH_SEPARATOR = "/";

PrvElement::PrvElement(RtObjType objType, const RtToken& name) :
    PrvObject(objType),
    _name(name)
{
}

void PrvElement::addAttribute(const RtToken& name, const RtToken& type, const RtValue& value)
{
    auto it = _attributesByName.find(name);
    if (it != _attributesByName.end())
    {
        throw ExceptionRuntimeError("An attribute named '" + name.str() + "' already exists for '" + getName().str() + "'");
    }
    _attributesByName[name] = _attributes.size();
    _attributes.push_back(RtAttribute(name, type, value));
}


PrvCompoundElement::PrvCompoundElement(RtObjType objType, const RtToken& name) :
    PrvElement(objType, name)
{
}

void PrvCompoundElement::addElement(PrvObjectHandle elem)
{
    if (!elem->hasApi(RtApiType::ELEMENT))
    {
        throw ExceptionRuntimeError("Given object is not a valid element");
    }
    PrvElement* e = elem->asA<PrvElement>();
    auto it = _elementsByName.find(e->getName());
    if (it != _elementsByName.end())
    {
        throw ExceptionRuntimeError("An element named '" + e->getName().str() + "' already exists in '" + getName().str() + "'");
    }
    _elementsByName[e->getName()] = _elements.size();
    _elements.push_back(elem);
}

void PrvCompoundElement::removeElement(const RtToken& name)
{
    const size_t index = getElementIndex(name);
    if (index == INVALID_INDEX)
    {
        throw ExceptionRuntimeError("An element named '" + name.str() + "' doesn't exists in '" + getName().str() + "'");
    }
    _attributes.erase(_attributes.begin() + index);
    _attributesByName.erase(name);
}

PrvObjectHandle PrvCompoundElement::findElement(const string& path) const
{
    const StringVec elementNames = splitString(path, PATH_SEPARATOR);
    if (elementNames.empty())
    {
        return nullptr;
    }

    size_t i = 0;
    RtToken name = RtToken(elementNames[i++]);
    PrvObjectHandle elem = getElement(name);

    while (elem != nullptr && i < elementNames.size())
    {
        if (elem->hasApi(RtApiType::COMPOUND_ELEMENT))
        {
            name = RtToken(elementNames[i]);
            elem = elem->asA<PrvCompoundElement>()->getElement(name);
        }
        else if (elem->hasApi(RtApiType::NODE))
        {
            PrvNode* node = elem->asA<PrvNode>();
            PrvNodeDef* nodedef = node->getNodeDef()->asA<PrvNodeDef>();
            name = RtToken(elementNames[i]);
            elem = nodedef->getElement(name);
        }
        ++i;
    }

    return elem;
}

}
