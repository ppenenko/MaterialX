//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTCOREIO_H
#define MATERIALX_RTCOREIO_H

#include <MaterialXCore/Document.h>

#include <MaterialXRuntime/RtStage.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class RtCoreIo : public RtApiBase
{
public:
    RtCoreIo(RtObject stage);

    /// Return the type for this API.
    RtApiType getApiType() const override;

    /// Read contents from a document.
    void read(const Document& doc, bool allNodeDefs = true);

    /// Write all stage contents to a document.
    void write(Document& doc);
};

}

#endif
