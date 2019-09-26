//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RT_STAGEDATA_H
#define MATERIALX_RT_STAGEDATA_H

#include <MaterialXRuntime/private/RtElementData.h>
#include <MaterialXRuntime/private/RtNodeDefData.h>

#include <MaterialXRuntime/RtObject.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class RtStageData : public RtElementData
{
public:
    RtStageData(const RtToken& name) :
        RtElementData(RtObjType::STAGE, name)
    {
    }

    RtNodeDefData* addNodeDef(const RtToken& name, const RtToken& category)
    {
        auto it = _nodedefsByName.find(name);
        if (it != _nodedefsByName.end())
        {
            throw ExceptionRuntimeError("An nodedef named '" + name + "' already exists for stage '" + getName() + "'");
        }

        _nodedefsByName[name] = _nodedefs.size();
        _nodedefs.push_back(RtNodeDefData(name, category));
        return &_nodedefs.back();
    }

    void clear()
    {
        _nodedefs.clear();
        _nodedefsByName.clear();
    }

protected:
    vector<RtNodeDefData> _nodedefs;
    std::unordered_map<RtToken, size_t> _nodedefsByName;
};

}

#endif
