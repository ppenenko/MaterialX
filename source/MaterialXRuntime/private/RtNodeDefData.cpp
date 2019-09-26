//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/private/RtNodeDefData.h>
#include <MaterialXRuntime/private/RtAttributeData.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

const size_t RtNodeDefData::INVALID_INDEX = std::numeric_limits<size_t>::max();

RtNodeDefData::RtNodeDefData(const RtToken& name, const RtToken& category) :
    RtElementData(RtObjType::NODEDEF, name),
    _category(category)
{
}

RtNodeDefData::~RtNodeDefData()
{
    for (RtAttributeData* a : _attributes)
    {
        delete a;
    }
    _attributes.clear();
    _attributesByName.clear();
}

}
