//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtObject.h>
#include <MaterialXRuntime/private/RtObjectData.h>

namespace MaterialX
{

namespace
{

    static const std::set<RtObjType> API_TO_OBJ_RTTI[int(RtApiType::NUM_TYPES)] =
    {
        // RtApiType::ELEMENT
        {
            RtObjType::PORTDEF,
            RtObjType::NODEDEF,
            RtObjType::NODE,
            RtObjType::NODEGRAPH,
            RtObjType::STAGE
        },
        // RtApiType::COMPOUND_ELEMENT
        {
            RtObjType::NODEDEF,
            RtObjType::NODEGRAPH,
            RtObjType::STAGE
        },
        // RtApiType::PORTDEF
        {
            RtObjType::PORTDEF
        },
        // RtApiType::NODEDEF
        {
            RtObjType::NODEDEF
        },
        // RtApiType::NODE
        {
            RtObjType::NODE
        },
        // RtApiType::NODEGRAPH
        {
            RtObjType::NODEGRAPH
        },
        // RtApiType::STAGE
        {
            RtObjType::STAGE
        },
        // RtApiType::STAGE_CORE_IO
        {
            RtObjType::STAGE
        }
    };
}

RtObject::RtObject() :
    _data(RtDataHandle())
{
}

RtObject::RtObject(const RtObject& other) :
    _data(other._data)
{
}

RtObject::RtObject(RtDataHandle data) :
    _data(data)
{
}

RtObject::~RtObject()
{
}

bool RtObject::isValid() const
{
    return _data && _data->getObjType() != RtObjType::INVALID;
}

RtObjType RtObject::getObjType() const
{
    return _data ? _data->getObjType() : RtObjType::INVALID;
}

bool RtObject::hasApi(RtApiType type) const
{
    return _data && _data->hasApi(type);
}


RtApiBase::RtApiBase() :
    _data(RtDataHandle())
{
}

RtApiBase::RtApiBase(RtDataHandle data)
{
    setData(data);
}

RtApiBase::RtApiBase(const RtObject& obj)
{
    setData(obj._data);
}

RtApiBase::RtApiBase(const RtApiBase& other)
{
    setData(other._data);
}

bool RtApiBase::isSupported(RtObjType type) const
{
    return API_TO_OBJ_RTTI[(int)getApiType()].count(type) != 0;
}

void RtApiBase::setObject(const RtObject& obj)
{
    setData(obj._data);
}

RtObject RtApiBase::getObject()
{
    return RtObject(_data);
}

bool RtApiBase::isValid() const
{
    return _data != nullptr;
}

void RtApiBase::setData(RtDataHandle data)
{
    if (data && !isSupported(data->getObjType()))
    {
        throw ExceptionRuntimeError("Given object is not supported by this API");
    }
    _data = data;
}

}
