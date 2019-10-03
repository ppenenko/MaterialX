//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/private/RtObjectData.h>

#include <set>

/// @file
/// TODO: Docs

namespace MaterialX
{

namespace
{
    static const std::set<RtApiType> OBJ_TO_API_RTTI[int(RtObjType::NUM_TYPES)] =
    {
        // INVALID
        {
        },
        // PORTDEF
        {
            RtApiType::ELEMENT,
            RtApiType::PORTDEF
        },
        // NODEDEF
        {
            RtApiType::ELEMENT,
            RtApiType::COMPOUND_ELEMENT,
            RtApiType::NODEDEF
        },
        // NODE
        {
            RtApiType::ELEMENT,
            RtApiType::NODE
        },
        // NODEGRAPH
        {
            RtApiType::ELEMENT,
            RtApiType::COMPOUND_ELEMENT,
            RtApiType::NODEGRAPH
        },
        // STAGE
        {
            RtApiType::ELEMENT,
            RtApiType::COMPOUND_ELEMENT,
            RtApiType::STAGE
        }
    };
}

RtObjectData::RtObjectData(RtObjType type) :
    _objType(type)
{
}

RtObjectData::~RtObjectData()
{
}

bool RtObjectData::hasApi(RtApiType type) const
{
    return OBJ_TO_API_RTTI[int(_objType)].count(type) != 0;
}

}
