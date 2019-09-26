//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RT_OBJECTDATA_H
#define MATERIALX_RT_OBJECTDATA_H

#include <MaterialXRuntime/RtObject.h>
#include <MaterialXRuntime/RtToken.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class RtObjectData
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

    template<class T> T* asA()
    {
        // TODO: Runtime type check
        return static_cast<T*>(this);
    }

private:
    RtObjType _objType;
};

}

#endif
