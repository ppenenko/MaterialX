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
    void removeAttribute(const RtToken& name);

    const RtAttribute* getAttribute(const RtToken& name) const
    {
        auto it = _attributesByName.find(name);
        return it != _attributesByName.end() ? it->second.get() : nullptr;
    }

    RtAttribute* getAttribute(const RtToken& name)
    {
        auto it = _attributesByName.find(name);
        return it != _attributesByName.end() ? it->second.get() : nullptr;
    }

    const RtAttribute* getAttribute(size_t index) const
    {
        return index < _attributes.size() ? _attributes[index].get() : nullptr;
    }

    RtAttribute* getAttribute(size_t index)
    {
        return index < _attributes.size() ? _attributes[index].get() : nullptr;
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

    using AttrPtr = std::shared_ptr<RtAttribute>;

    RtToken _name;
    vector<AttrPtr> _attributes;
    RtTokenMap<AttrPtr> _attributesByName;
    friend class PrvStage;
};

class PrvCompoundElement : public PrvElement
{
public:
    PrvCompoundElement(RtObjType objType, const RtToken& name);

    virtual ~PrvCompoundElement() {}

    void addElement(PrvObjectHandle elem);
    void removeElement(const RtToken& name);

    size_t numElements() const
    {
        return _elements.size();
    }

    PrvObjectHandle getElement(size_t index) const
    {
        return index < _elements.size() ? _elements[index] : nullptr;
    }

    const PrvObjectHandleVec& getElements() const
    {
        return _elements;
    }

    virtual PrvObjectHandle findElementByName(const RtToken& name) const;
    virtual PrvObjectHandle findElementByPath(const string& path) const;

    static const string PATH_SEPARATOR;

protected:
    PrvObjectHandleVec _elements;
    RtTokenMap<PrvObjectHandle> _elementsByName;
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
