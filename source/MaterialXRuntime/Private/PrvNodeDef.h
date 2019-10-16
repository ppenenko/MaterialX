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

    static PrvObjectHandle create(const RtToken& name, const RtToken& category);

    const RtToken& getCategory() const
    {
        return _category;
    }

    void addPort(PrvObjectHandle portdef);

    // Short syntax getter for convenience.
    PrvPortDef* portdef(size_t index) { return (PrvPortDef*)getElement(index).get(); }

protected:
    RtToken _category;
    friend class PrvNode;
    friend class PrvNodeGraph;
    friend class RtPort;
};

}

#endif
