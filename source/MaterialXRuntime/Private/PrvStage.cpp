//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/Private/PrvStage.h>
#include <MaterialXRuntime/Private/PrvNodeDef.h>
#include <MaterialXRuntime/Private/PrvNode.h>
#include <MaterialXRuntime/Private/PrvNodeGraph.h>

#include <MaterialXRuntime/RtObject.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

PrvStage::PrvStage(const RtToken& name) :
    PrvElement(RtObjType::STAGE, name)
{
}

PrvObjectHandle PrvStage::create(const RtToken& name)
{
    return std::make_shared<PrvStage>(name);
}

void PrvStage::addReference(PrvObjectHandle refStage)
{
    if (!refStage->hasApi(RtApiType::STAGE))
    {
        throw ExceptionRuntimeError("Given object is not a valid stage");
    }
    _refStages.push_back(refStage);
}

void PrvStage::removeReference(const RtToken& name)
{
    for (auto it = _refStages.begin(); it != _refStages.end(); ++it)
    {
        PrvStage* stage = (*it)->asA<PrvStage>();
        if (stage->getName() == name)
        {
            _refStages.erase(it);
            break;
        }
    }
}

void PrvStage::addElement(PrvObjectHandle elem)
{
    if (!elem->hasApi(RtApiType::ELEMENT))
    {
        throw ExceptionRuntimeError("Given object is not a valid element");
    }
    PrvElement* e = elem->asA<PrvElement>();
    if (getElement(e->getName()) != nullptr)
    {
        throw ExceptionRuntimeError("An element named '" + e->getName() + "' already exists in stage '" + getName() + "'");
    }
    _elementsByName[e->getName()] = _elements.size();
    _elements.push_back(elem);
}

void PrvStage::removeElement(const RtToken& name)
{
    const size_t index = getElementIndex(name);
    if (index == INVALID_INDEX)
    {
        // Element not found in this stage
        for (auto it : _refStages)
        {
            PrvStage* refStage = it->asA<PrvStage>();
            if (refStage->getElement(name))
            {
                throw ExceptionRuntimeError("Element '" + name + "' is in a referenced read-only stage and connot be removed'");
            }
        }
        throw ExceptionRuntimeError("An element named '" + name + "' doesn't exists in stage '" + getName() + "'");
    }
    _elements.erase(_elements.begin() + index);
    _elementsByName.erase(name);
}

PrvObjectHandle PrvStage::getElement(const RtToken& name) const
{
    auto it = _elementsByName.find(name);
    if (it != _elementsByName.end())
    {
        return _elements[it->second];
    }
    for (auto rs : _refStages)
    {
        PrvStage* refStage = rs->asA<PrvStage>();
        PrvObjectHandle elem = refStage->getElement(name);
        if (elem)
        {
            return elem;
        }
    }
    return nullptr;
}

PrvObjectHandle PrvStage::findElement(const RtString& path) const
{
    const StringVec elementNames = splitString(path, PATH_SEPARATOR);
    if (elementNames.empty())
    {
        return nullptr;
    }

    size_t i = 0;
    PrvObjectHandle elem = getElement(elementNames[i++]);

    while (elem && i < elementNames.size())
    {
        if (elem->hasApi(RtApiType::COMPOUND_ELEMENT))
        {
            elem = elem->asA<PrvCompoundElement>()->getElement(elementNames[i]);
        }
        else if (elem->hasApi(RtApiType::NODE))
        {
            PrvNode* node = elem->asA<PrvNode>();
            PrvNodeDef* nodedef = node->getNodeDef()->asA<PrvNodeDef>();
            elem = nodedef->getElement(elementNames[i]);
        }
        ++i;
    }

    if (!elem || i < elementNames.size())
    {
        // The full path was not found so search
        // any referenced stages as well.
        for (auto it : _refStages)
        {
            PrvStage* refStage = it->asA<PrvStage>();
            elem = refStage->findElement(path);
            if (elem)
            {
                break;
            }
        }
    }

    return elem;
}

}
