//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTSTAGEDATA_H
#define MATERIALX_RTSTAGEDATA_H

#include <MaterialXRuntime/private/RtElementData.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class RtStageData : public RtCompoundElementData
{
public:
    RtStageData(const RtToken& name);

    static RtDataHandle create(const RtToken& name);
};

}

#endif
