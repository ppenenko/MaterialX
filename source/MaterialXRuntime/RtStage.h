//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTSTAGE_H
#define MATERIALX_RTSTAGE_H

/// @file
/// TODO: Docs

#include <MaterialXRuntime/RtElement.h>
#include <MaterialXRuntime/RtNodeDef.h>

namespace MaterialX
{

/// @class RtStage
/// TODO: Docs
class RtStage : public RtElement
{
public:
    /// Constructor attaching a stage object to the API.
    RtStage(const RtObject& obj);

    /// Return the type for this object.
    RtApiType getApiType() const override;

    /// Create a new empty stage.
    static RtObject create(const RtToken& name);

    /// Clear the stage from all contents,
    /// deallocating all of its contained data.
    void clear();
};

}

#endif
