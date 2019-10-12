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

    /// Return the type for this API.
    RtApiType getApiType() const override;

    /// Create a new empty stage.
    static RtObject create(const RtToken& name);

    /// Add a reference to another stage.
    void addReference(RtObject refStage);

    /// Remove a reference to another stage.
    void removeReference(const RtToken& name);

    /// Add an element to the stage.
    void addElement(RtObject elem);

    /// Remove an element from the stage.
    void removeElement(const RtToken& name);

    /// Return an element by name,
    /// or a null object if no such element exists.
    RtObject getElement(const RtToken& name) const;

    /// Find an element by string path.
    /// Paths are element names expressed hierarchicaly and seperated by '/'.
    ///
    /// Exanples:
    ///     "/nodegraph1/add1"
    ///     "/nodegraph2/multiply4/in1"
    ///     "/ND_foo/out"
    ///
    /// When finding ports on node instances an object for the portdef will
    /// be returned. In order to get to the port from the portdef use the
    /// port constructor method: RtPort(RtObject node, RtObject portdef)
    ///
    RtObject findElement(const string& path) const;
};

}

#endif
