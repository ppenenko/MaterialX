//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTELEMENTDATA_H
#define MATERIALX_RTELEMENTDATA_H

#include <MaterialXRuntime/private/RtObjectData.h>

#include <MaterialXRuntime/RtElement.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class RtElementData : public RtObjectData
{
public:
    RtElementData(RtObjType objType, const RtToken& name);

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

    RtToken _name;
    vector<RtAttribute> _attributes;
    RtTokenIndexMap _attributesByName;
    friend class RtStageData;
};

class RtCompoundElementData : public RtElementData
{
public:
    RtCompoundElementData(RtObjType objType, const RtToken& name);

    void addElement(RtDataHandle elem);

    void removeElement(const RtToken& name);

    RtDataHandle findElement(const RtString& path) const;

    RtDataHandle getElement(const RtToken& name) const
    {
        auto it = _elementsByName.find(name);
        return it != _elementsByName.end() ? _elements[it->second] : nullptr;
    }

    RtDataHandle getElement(size_t index) const
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

protected:
    RtDataHandleArray _elements;
    RtTokenIndexMap _elementsByName;
};

}

#endif
