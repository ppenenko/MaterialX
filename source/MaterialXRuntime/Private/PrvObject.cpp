//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/Private/PrvObject.h>

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
            RtApiType::PORTDEF,
            RtApiType::ELEMENT_ITERATOR,
        },
        // NODEDEF
        {
            RtApiType::ELEMENT,
            RtApiType::COMPOUND_ELEMENT,
            RtApiType::NODEDEF,
            RtApiType::ELEMENT_ITERATOR,
        },
        // NODE
        {
            RtApiType::ELEMENT,
            RtApiType::NODE,
            RtApiType::ELEMENT_ITERATOR,
        },
        // NODEGRAPH
        {
            RtApiType::ELEMENT,
            RtApiType::COMPOUND_ELEMENT,
            RtApiType::NODEGRAPH,
            RtApiType::ELEMENT_ITERATOR,
        },
        // STAGE
        {
            RtApiType::ELEMENT,
            RtApiType::COMPOUND_ELEMENT,
            RtApiType::STAGE,
            RtApiType::CORE_IO,
            RtApiType::ELEMENT_ITERATOR,
        }
    };
}

PrvObject::PrvObject(RtObjType type) :
    _objType(type)
{
}

PrvObject::~PrvObject()
{
}

bool PrvObject::hasApi(RtApiType type) const
{
    return OBJ_TO_API_RTTI[int(_objType)].count(type) != 0;
}

}
