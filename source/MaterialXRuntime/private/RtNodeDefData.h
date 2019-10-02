//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTNODEDEFDATA_H
#define MATERIALX_RTNODEDEFDATA_H

#include <MaterialXRuntime/private/RtElementData.h>
#include <MaterialXRuntime/private/RtPortDefData.h>

#include <MaterialXRuntime/RtObject.h>
#include <MaterialXRuntime/RtValue.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class RtNodeDefData : public RtElementData
{
public:
    RtNodeDefData(const RtToken& name, const RtToken& category);

    static RtDataHandle create(const RtToken& name, const RtToken& category);

    const RtToken& getCategory() const
    {
        return _category;
    }

    void addPortDef(RtDataHandle attr);

    RtDataHandle getPortDef(const RtToken& name) const
    {
        auto it = _portdefsByName.find(name);
        return it != _portdefsByName.end() ? _portdefs[it->second] : nullptr;
    }

    RtDataHandle getPortDef(size_t index) const
    {
        return index < _portdefs.size() ? _portdefs[index] : nullptr;
    }

    size_t numPorts() const
    {
        return _portdefs.size();
    }

    size_t getPortIndex(const RtToken& name) const
    {
        auto it = _portdefsByName.find(name);
        return it != _portdefsByName.end() ? it->second : INVALID_INDEX;
    }

protected:
    // Short syntax getter for convenience.
    inline RtPortDefData* portdef(size_t index) { return (RtPortDefData*)getPortDef(index).get(); }

    RtToken _category;
    RtDataHandleArray _portdefs;
    RtTokenIndexMap _portdefsByName;
    friend class RtNodeData;
    friend class RtNodeGraphData;
    friend class RtPort;
};

}

#endif
