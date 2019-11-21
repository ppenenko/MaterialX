//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTFILEIO_H
#define MATERIALX_RTFILEIO_H

#include <MaterialXRuntime/Library.h>
#include <MaterialXRuntime/RtStage.h>

#include <MaterialXCore/Document.h>
#include <MaterialXFormat/File.h>
#include <MaterialXFormat/XmlIo.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

/// API for read and write of data from MaterialX files.
class RtFileIo : public RtApiBase
{
public:
    /// Filter function type used for filtering elements during read.
    /// If the filter returns false the element will not be read.
    using ReadFilter = std::function<bool(const ElementPtr& elem)>;

    /// Filter function type used for filtering objects during write.
    /// If the filter returns false the object will not be written.
    using WriteFilter = std::function<bool(const RtObject& obj)>;

public:
    /// Constructor attaching this API to a stage.
    RtFileIo(RtObject stage);

    /// Return the type for this API.
    RtApiType getApiType() const override;

    /// Read contents from a file path.
    /// If a filter is used only elements accepted by the filter
    /// will be read from the document.
    void read(const FilePath& documentPath, const FileSearchPath& searchPaths, ReadFilter filter = nullptr);

    /// Write all stage contents to a document.
    /// If a filter is used only elements accepted by the filter
    /// will be written to the document.
    void write(const FilePath& documentPath, const XmlWriteOptions* writeOptions = nullptr, WriteFilter filter = nullptr);

    /// Read all contents from one or more libraries.
    /// All MaterialX files found inside the given libraries will be read.
    void loadLibraries(const StringVec& libraryPaths, const FileSearchPath& searchPaths);
};

}

#endif
