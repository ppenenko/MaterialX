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
    _category(category),
    _numOutputs(0)
{
}

PrvObjectHandle PrvNodeDef::createNew(const RtToken& name, const RtToken& category)
{
    return std::make_shared<PrvNodeDef>(name, category);
}

void PrvNodeDef::addPort(PrvObjectHandle portdef)
{
    if (!portdef->hasApi(RtApiType::PORTDEF))
    {
        throw ExceptionRuntimeError("Given object is not a valid portdef");
    }

    PrvPortDef* port = portdef->asA<PrvPortDef>();
    if (_elementsByName.count(port->getName()))
    {
        throw ExceptionRuntimeError("A port named '" + port->getName().str() + "' already exists for nodedef '" + getName().str() + "'");
    }

    // We want to preserve the ordering of having all outputs stored before any inputs.
    // So if inputs are already stored we need to handled inserting the a new output in
    // the right place.
    if (port->isOutput() && _elements.size() && !_elements.back()->asA<PrvPortDef>()->isOutput())
    {
        // Insert the new output after the last output.
        for (auto it = _elements.begin(); it != _elements.end(); ++it)
        {
            if (!(*it)->asA<PrvPortDef>()->isOutput())
            {
                _elements.insert(it, portdef);
                break;
            }
        }
    }
    else
    {
        _elements.push_back(portdef);
    }
    _elementsByName[port->getName()] = portdef;
    _numOutputs += port->isOutput();
}

void PrvNodeDef::removePort(const RtToken& name)
{
    PrvPortDef* port = portdef(name);
    if (port)
    {
        _numOutputs -= port->isOutput();
        removeElement(name);
    }
}

}
