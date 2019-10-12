//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtValue.h>
#include <MaterialXRuntime/RtTypes.h>

#include <sstream>

namespace MaterialX
{

RtValue::RtValue(const ValuePtr& v) : 
    _data{0,0}
{
    if (!v)
    {
        return;
    }
    if (v->isA<bool>())
    {
        asBool() = v->asA<bool>();
    }
    else if (v->isA<float>())
    {
        asFloat() = v->asA<float>();
    }
    else if (v->isA<int>())
    {
        asInt() = v->asA<int>();
    }
    else if (v->isA<Color3>())
    {
        asColor3() = v->asA<Color3>();
    }
    else if (v->isA<Vector4>())
    {
        asVector4() = v->asA<Vector4>();
    }
}


string RtValue::getValueString(const RtToken& type) const
{
    std::stringstream ss;
    if (type == RtType::FLOAT)
    {
        ss << asFloat();
    }
    else if (type == RtType::INTEGER)
    {
        ss << asInt();
    }
    else if (type == RtType::BOOLEAN)
    {
        ss << asBool();
    }
    else if (type == RtType::BOOLEAN)
    {
        ss << asBool();
    }
    else if (type == RtType::COLOR3)
    {
        const Color3& v = asColor3();
        ss << v[0] << ", " << v[1] << ", " << v[2];
    }
    else if (type == RtType::VECTOR4)
    {
        const Vector4& v = asVector4();
        ss << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3];
    }
    else if (type == RtType::STRING || type == RtType::FILENAME)
    {
        ss << asToken().str();
    }
    else
    {
        throw ExceptionRuntimeError("Not implemented!");
    }
    return ss.str();
}


}
