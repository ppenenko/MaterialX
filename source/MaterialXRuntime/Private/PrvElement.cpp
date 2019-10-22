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

    AttrPtr attr(new RtAttribute(name, type, value));
    _attributes.push_back(attr);
    _attributesByName[name] = attr;
}

void PrvElement::removeAttribute(const RtToken& name)
{
    for (auto it = _attributes.begin(); it != _attributes.end(); ++it)
    {
        if ((*it)->getName() == name)
        {
            _attributes.erase(it);
            break;
        }
    }
    _attributesByName.erase(name);
}


const string PrvCompoundElement::PATH_SEPARATOR = "/";

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

    PrvElement* el = elem->asA<PrvElement>();
    if (_elementsByName.count(el->getName()))
    {
        throw ExceptionRuntimeError("An element named '" + el->getName().str() + "' already exists in '" + getName().str() + "'");
    }

    _elements.push_back(elem);
    _elementsByName[el->getName()] = elem;
}

void PrvCompoundElement::removeElement(const RtToken& name)
{
    for (auto it = _elements.begin(); it != _elements.end(); ++it)
    {
        if ((*it)->asA<PrvElement>()->getName() == name)
        {
            _elements.erase(it);
            break;
        }
    }
    _elementsByName.erase(name);
}

PrvObjectHandle PrvCompoundElement::findElementByName(const RtToken& name) const
{
    auto it = _elementsByName.find(name);
    return it != _elementsByName.end() ? it->second : nullptr;
}

PrvObjectHandle PrvCompoundElement::findElementByPath(const string& path) const
{
    const StringVec elementNames = splitString(path, PATH_SEPARATOR);
    if (elementNames.empty())
    {
        return nullptr;
    }

    size_t i = 0;
    RtToken name = RtToken(elementNames[i++]);
    PrvObjectHandle elem = findElementByName(name);

    while (elem != nullptr && i < elementNames.size())
    {
        if (elem->hasApi(RtApiType::COMPOUND_ELEMENT))
        {
            name = RtToken(elementNames[i]);
            elem = elem->asA<PrvCompoundElement>()->findElementByName(name);
        }
        else if (elem->hasApi(RtApiType::NODE))
        {
            PrvNode* node = elem->asA<PrvNode>();
            PrvNodeDef* nodedef = node->getNodeDef()->asA<PrvNodeDef>();
            name = RtToken(elementNames[i]);
            elem = nodedef->findElementByName(name);
        }
        ++i;
    }

    return elem;
}

}