//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RT_OBJECTDATA_H
#define MATERIALX_RT_OBJECTDATA_H

#include <MaterialXRuntime/RtObject.h>
#include <MaterialXRuntime/RtToken.h>

#include <unordered_map>
#include <set>

/// @file
/// TODO: Docs

namespace MaterialX
{

static const std::set<RtApiType> objToApiRTTI[int(RtObjType::NUM_TYPES)] =
{
    {},                                         // INVALID
    {RtApiType::ELEMENT, RtApiType::ATTRIBUTE}, // ATTRIBUTE
    {RtApiType::ELEMENT, RtApiType::NODEDEF},   // NODEDEF
    {RtApiType::ELEMENT, RtApiType::NODE},      // NODE
    {RtApiType::ELEMENT, RtApiType::NODEGRAPH}, // NODEGRAPH
    {RtApiType::ELEMENT, RtApiType::STAGE}      // STAGE
};

class RtObjectData : public std::enable_shared_from_this<RtObjectData>
{
public:
    RtObjectData(RtObjType type) : 
        _objType(type)
    {
    }

    /// Return the type for this object.
    RtObjType getObjType() const
    {
        return _objType;
    }

    /// Query if the given API type is supported by this object.
    bool hasApi(RtApiType type) const
    {
        return objToApiRTTI[int(_objType)].count(type) != 0;
    }

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

using RtDataHandleArray = vector<RtDataHandle>;
using RtDataHandleNameMap = std::unordered_map<RtToken, size_t>;

}

#endif
