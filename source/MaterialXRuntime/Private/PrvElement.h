//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_PRVELEMENT_H
#define MATERIALX_PRVELEMENT_H

#include <MaterialXRuntime/Private/PrvObject.h>

#include <MaterialXRuntime/RtElement.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

using PrvObjectHandleVec = vector<PrvObjectHandle>;

class PrvElement : public PrvObject
{
public:
    PrvElement(RtObjType objType, const RtToken& name);

    const RtToken& getName() const
    {
        return _name;
    }

    void addAttribute(const RtToken& name, const RtToken& type, const RtValue& value);

    const RtAttribute* getAttribute(const RtToken& name) const
    {
        auto it = _attributesByName.find(name);
        return it != _attributesByName.end() ? &_attributes[it->second] : nullptr;
    }

    RtAttribute* getAttribute(const RtToken& name)
    {
        auto it = _attributesByName.find(name);
        return it != _attributesByName.end() ? &_attributes[it->second] : nullptr;
    }

    const RtAttribute* getAttribute(size_t index) const
    {
        return index < _attributes.size() ? &_attributes[index] : nullptr;
    }

    RtAttribute* getAttribute(size_t index)
    {
        return index < _attributes.size() ? &_attributes[index] : nullptr;
    }

    size_t numAttributes() const
    {
        return _attributes.size();
    }

protected:
    void setName(const RtToken& name)
    {
        _name = name;
    }

    static const string PATH_SEPARATOR;

    RtToken _name;
    vector<RtAttribute> _attributes;
    RtTokenMap<size_t> _attributesByName;
    friend class PrvStage;
};

class PrvCompoundElement : public PrvElement
{
public:
    PrvCompoundElement(RtObjType objType, const RtToken& name);

    void addElement(PrvObjectHandle elem);

    void removeElement(const RtToken& name);

    PrvObjectHandle getElement(const RtToken& name) const
    {
        auto it = _elementsByName.find(name);
        return it != _elementsByName.end() ? _elements[it->second] : nullptr;
    }

    PrvObjectHandle getElement(size_t index) const
    {
        return index < _elements.size() ? _elements[index] : nullptr;
    }

    size_t numElements() const
    {
        return _elements.size();
    }

    size_t getElementIndex(const RtToken& name) const
    {
        auto it = _elementsByName.find(name);
        return it != _elementsByName.end() ? it->second : INVALID_INDEX;
    }

    PrvObjectHandle findElement(const string& path) const;

    template<typename T>
    size_t findElements(const T& pred, PrvObjectHandleVec& result) const
    {
        for (auto elem : _elements)
        {
            if (pred(elem))
            {
                result.push_back(elem);
            }
        }
        return result.size();
    }

protected:
    PrvObjectHandleVec _elements;
    RtTokenMap<size_t> _elementsByName;
};

struct PrvObjectPredicate
{
    PrvObjectPredicate(RtObjType type) : _type(type) {}

    bool operator()(const PrvObjectHandle& obj) const
    {
        return obj->getObjType() == _type;
    }

    RtObjType _type;
};

struct PrvApiPredicate
{
    PrvApiPredicate(RtApiType type) : _type(type) {}

    bool operator()(const PrvObjectHandle& obj) const
    {
        return obj->hasApi(_type);
    }

    RtApiType _type;
};


}

#endif
