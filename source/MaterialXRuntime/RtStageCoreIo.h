//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTSTAGECOREIO_H
#define MATERIALX_RTSTAGECOREIO_H

#include <MaterialXCore/Document.h>

#include <MaterialXRuntime/RtStage.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class RtStageCoreIo : public RtApiBase
{
private:
    RtStageCoreIo(RtObject stage);

    /// Return the type for this API.
    RtApiType getApiType() const override;

    /// Read contents from a document.
    void read(const Document& doc);
    void read(const NodeDef& nodedef);
    void read(const Node& node);

    /// Read all contents from the stage to a document.
    void write(Document& doc);
};

}

#endif
