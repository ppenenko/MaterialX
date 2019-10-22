//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_PRVOBJECT_H
#define MATERIALX_PRVOBJECT_H

#include <MaterialXRuntime/RtObject.h>
#include <MaterialXRuntime/RtToken.h>

#include <unordered_map>
#include <memory>

/// @file
/// TODO: Docs

namespace MaterialX
{

class PrvObject : public std::enable_shared_from_this<PrvObject>
{
public:
    PrvObject(RtObjType type);

    virtual ~PrvObject();

    /// Return the type for this object.
    RtObjType getObjType() const
    {
        return _objType;
    }

    /// Query if the given API type is supported by this object.
    bool hasApi(RtApiType type) const;

    /// Casting the object to a given type.
    /// NOTE: no type check if performed so the templated type 
    /// must be a type supported by the object.
    template<class T> T* asA()
    {
        return static_cast<T*>(this);
    }

protected:
    RtObjType _objType;
};

}

#endif