//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTOBJECT_H
#define MATERIALX_RTOBJECT_H

#include <MaterialXRuntime/Library.h>
#include <MaterialXRuntime/RtToken.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class RtObjectData;

// A handle to private data
using RtDataHandle = RtObjectData*;

/// Type identifiers for scene objects.
enum class RtObjType
{
    INVALID,
    ATTRIBUTE,
    NODEDEF,
    NODE,
    NODEGRAPH,
    STAGE,
    NUM_TYPES
};

/// @class RtObject
/// Handle class for all scene objects.
class RtObject
{
public:
    /// Default constructor.
    RtObject();

    /// Copy constructor.
    RtObject(const RtObject& other);

    /// Destructor.
    ~RtObject();

    /// Return the type for this object.
    RtObjType getObjType() const;

    /// Query if the given API type is supported by this object.
    bool hasApi(RtObjType type) const;

    /// Return true if the object is valid.
    bool isValid() const;

    /// Return true if the object is valid.
    bool operator()() const
    {
        return isValid();
    }

    /// Return true if the objects are equal.
    bool operator==(const RtObject& other) const
    {
        return _data == other._data;
    }

private:
    RtDataHandle _data;

    RtObject(RtDataHandle data);

    friend class RtApiBase;
};


/// Type identifiers for API attachable to objects.
enum class RtApiType
{
    ELEMENT,
    ATTRIBUTE,
    NODEDEF,
    NODE,
    NODEGRAPH,
    STAGE,
    NUM_TYPES
};

class RtApiBase
{
public:
    /// Destructor.
    virtual ~RtApiBase() {};

    /// Return the type for this object.
    virtual RtApiType getApiType() const = 0;

    /// Query if the given object type is supported by this API.
    /// Derived classes should override this.
    virtual bool isSupported(RtObjType type) const = 0;

    /// Query if the given object is supported by this API.
    bool isSupported(const RtObject& obj) const
    {
        return isSupported(obj.getObjType());
    }

    /// Attach an object to this API.
    void setObject(const RtObject& obj);

    /// Return the object attached to this API.
    RtObject getObject();

    /// Return true if the API object is valid.
    bool isValid() const;

    /// Return true if the API object is valid.
    bool operator()() const
    {
        return isValid();
    }

    /// Return true if the attached objects are equal.
    bool operator==(const RtApiBase& other) const
    {
        return _data == other._data;
    }

protected:
    /// Default constructor.
    RtApiBase();

    /// Construct from a data handle.
    RtApiBase(RtDataHandle data);

    /// Construct from an object.
    RtApiBase(const RtObject& obj);

    /// Copy constructor.
    RtApiBase(const RtApiBase& other);

    /// Set data for the API.
    void setData(RtDataHandle data);

    /// Return data set for the API.
    RtDataHandle data() { return _data; }

    /// Return data set for the API.
    const RtDataHandle data() const { return _data; }

    /// Construct an object from a data handle.
    static RtObject object(RtDataHandle data);

    /// Construct a templated API from a data handle.
    template<typename T>
    static T api(RtDataHandle data)
    {
        return T(RtApiBase::object(data));
    }

private:
    /// Internal data attached to the API.
    RtDataHandle _data;
};

}

#endif
