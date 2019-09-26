//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RT_ELEMENT_H
#define MATERIALX_RT_ELEMENT_H

/// @file
/// TODO: Docs

#include <MaterialXRuntime/RtObject.h>
#include <MaterialXRuntime/RtValue.h>

namespace MaterialX
{

/// @struct RtMetaData
/// Meta data attachable to any element.
struct RtMetaData
{
    RtToken type;
    RtValue value;
};

/// @class RtElement
/// TODO: Docs
class RtElement : public RtApiBase
{
public:
    /// Constructor attaching an object to the API.
    RtElement(const RtObject& obj);

    /// Return the type for this object.
    RtApiType getApiType() const override;

    /// Query if the given object type is supported by this API.
    bool isSupported(RtObjType type) const override;

    /// Get element name.
    const RtToken& getName() const;

    /// Return meta data of the given name, or nullptr
    /// if no such data is attached to the element.
    RtMetaData* getMetaData(const RtToken& name);

    /// Attach meta data to the element.
    RtMetaData* addMetaData(const RtToken& name, const RtToken& type, const RtValue& value);
};

}

#endif
