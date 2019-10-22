//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_PRVNODEDEF_H
#define MATERIALX_PRVNODEDEF_H

#include <MaterialXRuntime/Private/PrvElement.h>
#include <MaterialXRuntime/Private/PrvPortDef.h>

#include <MaterialXRuntime/RtObject.h>
#include <MaterialXRuntime/RtValue.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class PrvNodeDef : public PrvCompoundElement
{
public:
    PrvNodeDef(const RtToken& name, const RtToken& category);

    static PrvObjectHandle createNew(const RtToken& name, const RtToken& category);

    const RtToken& getCategory() const
    {
        return _category;
    }

    void addPort(PrvObjectHandle portdef);
    void removePort(const RtToken& name);

    size_t numPorts() const
    {
        return numElements();
    }

    size_t numOutputs() const
    {
        return _numOutputs;
    }

    // Short syntax getter for convenience.
    PrvPortDef* port(const RtToken& name) const { return findElementByName(name)->asA<PrvPortDef>(); }
    PrvPortDef* port(size_t index) const { return getElement(index)->asA<PrvPortDef>(); }

protected:
    RtToken _category;
    size_t _numOutputs;
    friend class PrvNode;
    friend class PrvNodeGraph;
    friend class RtPort;
};

}

#endif
