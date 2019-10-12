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
    PrvObjectHandle findElement(const string& path) const;

    size_t numOwnElements() const
    {
        return _elements.size();
    }

    PrvObjectHandle getOwnElement(const RtToken& name) const
    {
        return getOwnElement(getOwnElementIndex(name));
    }

    PrvObjectHandle getOwnElement(size_t index) const
    {
        return index < _elements.size() ? _elements[index] : nullptr;
    }

    size_t getOwnElementIndex(const RtToken& name) const
    {
        auto it = _elementsByName.find(name);
        return it != _elementsByName.end() ? it->second : INVALID_INDEX;
    }

protected:
    PrvObjectHandleVec _refStages;
    PrvObjectHandleVec _elements;
    RtTokenMap<size_t> _elementsByName;
};

}

#endif
