//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RT_STAGEDATA_H
#define MATERIALX_RT_STAGEDATA_H

#include <MaterialXRuntime/private/RtElementData.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class RtStageData : public RtElementData
{
public:
    RtStageData(const RtToken& name);

    void addNodeDef(RtDataHandle nodedef);
    void addNode(RtDataHandle node);

    void clear();

    static RtDataHandle create(const RtToken& name);

protected:
    RtDataHandleArray _nodedefs;
    RtDataHandleNameMap _nodedefsByName;

    RtDataHandleArray _nodes;
    RtDataHandleNameMap _nodesByName;
};

}

#endif
