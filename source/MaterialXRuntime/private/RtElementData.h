//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RT_ELEMENTDATA_H
#define MATERIALX_RT_ELEMENTDATA_H

#include <MaterialXRuntime/private/RtObjectData.h>

#include <MaterialXRuntime/RtObject.h>
#include <MaterialXRuntime/RtElement.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class RtElementData : public RtObjectData
{
public:
    RtElementData(RtObjType objType) :
        RtObjectData(objType)
    {
    }

    RtElementData(RtObjType objType, const RtToken& name) :
        RtObjectData(objType),
        _name(name)
    {
    }

    ~RtElementData()
    {
        for (auto it : _metaData)
        {
            delete it.second;
        }
    }

    const RtToken& getName() const
    {
        return _name;
    }

    void setName(const RtToken& name)
    {
        _name = name;
    }

    RtMetaData* getMetaData(const RtToken& name)
    {
        auto it = _metaData.find(name);
        return it != _metaData.end() ? it->second : nullptr;
    }

    RtMetaData* addMetaData(const RtToken& name, const RtToken& type, const RtValue& value)
    {
        auto it = _metaData.find(name);
        if (it != _metaData.end())
        {
            throw ExceptionRuntimeError("Meta data named '" + name + "' already exists for element '" + getName() + "'");
        }

        RtMetaData* md = new RtMetaData();
        md->type = type;
        md->value = value;
        _metaData[name] = md;

        return md;
    }

protected:
    RtToken _name;
    std::unordered_map<RtToken, RtMetaData*> _metaData;
};

}

#endif
