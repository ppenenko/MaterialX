//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_PRVSTAGE_H
#define MATERIALX_PRVSTAGE_H

#include <MaterialXRuntime/Private/PrvElement.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class PrvStage : public PrvElement
{
public:
    PrvStage(const RtToken& name);

    static PrvObjectHandle create(const RtToken& name);

    void addReference(PrvObjectHandle refStage);
    void removeReference(const RtToken& name);

    void addElement(PrvObjectHandle elem);
    void removeElement(const RtToken& name);

    PrvObjectHandle getElement(const RtToken& name) const;
    PrvObjectHandle findElement(const RtString& path) const;

protected:
    size_t getElementIndex(const RtToken& name) const
    {
        auto it = _elementsByName.find(name);
        return it != _elementsByName.end() ? it->second : INVALID_INDEX;
    }

    PrvObjectArray _refStages;
    PrvObjectArray _elements;
    TokenIndexMap _elementsByName;
};

}

#endif
