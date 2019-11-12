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

PrvNodeDef::PrvNodeDef(const RtToken& name, const RtToken& nodeName) :
    PrvValueStoringElement(RtObjType::NODEDEF, name),
    _nodeName(nodeName),
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

    PrvPortDef* p = portdef->asA<PrvPortDef>();
    if (_childrenByName.count(p->getName()))
    {
        throw ExceptionRuntimeError("A port named '" + p->getName().str() + "' already exists for nodedef '" + getName().str() + "'");
    }

    // We want to preserve the ordering of having all outputs stored before any inputs.
    // So if inputs are already stored we need to handled inserting the new output in
    // the right place.
    if (p->isOutput() && numPorts() > numOutputs())
    {
        // Insert the new output after the last output.
        auto it = _children.begin() + numOutputs();
        _children.insert(it, 1, portdef);
    }
    else
    {
        _children.push_back(portdef);
    }

    _childrenByName[p->getName()] = portdef;
    _numOutputs += p->isOutput();
    rebuildPortIndex();
}

void PrvNodeDef::removePort(const RtToken& name)
{
    PrvPortDef* p = findPort(name);
    if (p)
    {
        _numOutputs -= p->isOutput();
        removeChild(name);
        rebuildPortIndex();
    }
}

void PrvNodeDef::rebuildPortIndex()
{
    for (size_t i = 0; i < numPorts(); ++i)
    {
        _portIndex[getPort(i)->getName()] = i;
    }
}

}
