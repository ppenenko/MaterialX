//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTPORTDEF_H
#define MATERIALX_RTPORTDEF_H

/// @file
/// TODO: Docs

#include <MaterialXRuntime/RtElement.h>

namespace MaterialX
{

class RtValue;

/// @class RtPortFlag
/// TODO: Docs
class RtPortFlag
{
public:
    static const uint32_t INPUT         = 0x00000001;
    static const uint32_t OUTPUT        = 0x00000002;
    static const uint32_t UNCONNECTABLE = 0x00000004;
    static const uint32_t UNIFORM       = 0x00000008;
    static const uint32_t INTERFACE     = 0x00000010;
};

/// @class RtPortDef
/// TODO: Docs
class RtPortDef : public RtElement
{
public:
    /// Constructor attaching and object to the API.
    RtPortDef(const RtObject& obj);

    /// Create a new portdef and add it to a parent 
    /// object if specified.
    /// The parent must be a nodedef object.
    static RtObject createNew(const RtToken& name, const RtToken& type, const RtValue& value, uint32_t flags, RtObject parent = RtObject());

    /// Return the type for this API.
    RtApiType getApiType() const override;

    /// Return the data type for this port.
    const RtToken& getType() const;

    /// Return the default value for this port.
    const RtValue& getValue() const;

    /// Return the default value for this port.
    RtValue& getValue();

    /// Set a new default value on the port.
    void setValue(const RtValue& v);

    /// Return the default color space for this port.
    const RtToken& getColorSpace() const;

    /// Set the default color space for this port.
    void setColorSpace(const RtToken& colorspace);

    /// Return the default unit for this port.
    const RtToken& getUnit() const;

    /// Set the default unit for this port.
    void setUnit(const RtToken& unit);

    /// Return the flags for this port.
    int32_t getFlags() const;

    /// Return true if this is an input port.
    bool isInput() const;

    /// Return true if this is an output port.
    bool isOutput() const;

    /// Return true if this port is connectable.
    bool isConnectable() const;

    /// Return true if this port is uniform.
    bool isUniform() const;

    /// Return true if this is a graph interface port.
    bool isInterface() const;
};

}

#endif
