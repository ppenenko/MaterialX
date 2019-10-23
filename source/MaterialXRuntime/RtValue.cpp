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
    else if (v->isA<Color2>())
    {
        asColor2() = v->asA<Color2>();
    }
    else if (v->isA<Color3>())
    {
        asColor3() = v->asA<Color3>();
    }
    else if (v->isA<Color4>())
    {
        asColor4() = v->asA<Color4>();
    }
    else if (v->isA<Vector2>())
    {
        asVector2() = v->asA<Vector2>();
    }
    else if (v->isA<Vector3>())
    {
        asVector3() = v->asA<Vector3>();
    }
    else if (v->isA<Vector4>())
    {
        asVector4() = v->asA<Vector4>();
    }
    else if (v->isA<string>())
    {
        asToken() = v->asA<string>();
    }
    else
    {
//        throw ExceptionRuntimeError("Not implemented!");
    }
}

string RtValue::getValueString(const RtToken& type) const
{
    std::stringstream ss;
    if (type == RtType::BOOLEAN)
    {
        ss << asBool();
    }
    else if (type == RtType::FLOAT)
    {
        ss << asFloat();
    }
    else if (type == RtType::INTEGER)
    {
        ss << asInt();
    }
    else if (type == RtType::COLOR2)
    {
        const Color2& v = asColor2();
        ss << v[0] << ", " << v[1];
    }
    else if (type == RtType::COLOR3)
    {
        const Color3& v = asColor3();
        ss << v[0] << ", " << v[1] << ", " << v[2];
    }
    else if (type == RtType::COLOR4)
    {
        const Color4& v = asColor4();
        ss << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3];
    }
    else if (type == RtType::VECTOR2)
    {
        const Vector2& v = asVector2();
        ss << v[0] << ", " << v[1];
    }
    else if (type == RtType::VECTOR3)
    {
        const Vector3& v = asVector3();
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
