//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtObject.h>
#include <MaterialXRuntime/private/RtObjectData.h>

namespace MaterialX
{

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
    return _data->getObjType();
}

bool RtObject::hasApi(RtObjType type) const
{
    return type == _data->getObjType();
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
    _data = data && isSupported(data->getObjType()) ? data : nullptr;
}

RtObject RtApiBase::object(RtDataHandle data)
{
    return RtObject(data);
}

}
