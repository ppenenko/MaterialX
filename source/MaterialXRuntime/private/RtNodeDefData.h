//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RT_NODEDEFDATA_H
#define MATERIALX_RT_NODEDEFDATA_H

#include <MaterialXRuntime/private/RtElementData.h>
#include <MaterialXRuntime/private/RtAttributeData.h>

#include <MaterialXRuntime/RtObject.h>
#include <MaterialXRuntime/RtValue.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class RtNodeDefData : public RtElementData
{
public:
    RtNodeDefData(const RtToken& name, const RtToken& category) :
        RtElementData(RtObjType::NODEDEF, name),
        _category(category)
    {
    }

    ~RtNodeDefData()
    {
        for (RtAttributeData* i : _inputs)
        {
            delete i;
        }
        _inputs.clear();
        _inputsByName.clear();

        for (RtAttributeData* o : _outputs)
        {
            delete o;
        }
        _outputs.clear();
        _outputsByName.clear();
    }

    const RtToken& getCategory() const
    {
        return _category;
    }

    RtAttributeData* addInputAttr(const RtToken& name, const RtToken& type, const RtValue& value, uint32_t flags)
    {
        auto it = _inputsByName.find(name);
        if (it != _inputsByName.end())
        {
            throw ExceptionRuntimeError("An input named '" + name + "' already exists for nodedef '" + getName() + "'");
        }

        RtAttributeData* attr = new RtAttributeData(name, type, value, flags | RtAttrFlag::INPUT);
        _inputsByName[name] = _inputs.size();
        _inputs.push_back(attr);

        return attr;
    }

    RtAttributeData* addOutputAttr(const RtToken& name, const RtToken& type, const RtValue& value, uint32_t flags)
    {
        auto it = _outputsByName.find(name);
        if (it != _outputsByName.end())
        {
            throw ExceptionRuntimeError("An output named '" + name + "' already exists for nodedef '" + getName() + "'");
        }

        RtAttributeData* attr = new RtAttributeData(name, type, value, flags | RtAttrFlag::OUTPUT);
        _outputsByName[name] = _outputs.size();
        _outputs.push_back(attr);

        return attr;
    }

    RtAttributeData* getInputAttr(const RtToken& name)
    {
        auto it = _inputsByName.find(name);
        return it != _inputsByName.end() ? _inputs[it->second] : nullptr;
    }

    RtAttributeData* getOutputAttr(const RtToken& name)
    {
        auto it = _outputsByName.find(name);
        return it != _outputsByName.end() ? _outputs[it->second] : nullptr;
    }

protected:
    RtToken _category;
    vector<RtAttributeData*> _inputs;
    std::unordered_map<RtToken, size_t> _inputsByName;
    vector<RtAttributeData*> _outputs;
    std::unordered_map<RtToken, size_t> _outputsByName;
};

}

#endif
