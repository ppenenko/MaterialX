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
        {},                                         // INVALID
        {RtApiType::ELEMENT, RtApiType::PORTDEF},   // PORTDEF
        {RtApiType::ELEMENT, RtApiType::NODEDEF},   // NODEDEF
        {RtApiType::ELEMENT, RtApiType::NODE},      // NODE
        {RtApiType::ELEMENT, RtApiType::NODEGRAPH}, // NODEGRAPH
        {RtApiType::ELEMENT, RtApiType::STAGE}      // STAGE
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
