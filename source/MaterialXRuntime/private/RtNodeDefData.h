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

class RtNodeDefData : public RtCompoundElementData
{
public:
    RtNodeDefData(const RtToken& name, const RtToken& category);

    static RtDataHandle create(const RtToken& name, const RtToken& category);

    const RtToken& getCategory() const
    {
        return _category;
    }

    void addPortDef(RtDataHandle attr);

protected:
    // Short syntax getter for convenience.
    inline RtPortDefData* portdef(size_t index) { return (RtPortDefData*)getElement(index).get(); }

    RtToken _category;
    friend class RtNodeData;
    friend class RtNodeGraphData;
    friend class RtPort;
};

}

#endif
