//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtValue.h>

namespace MaterialX
{

RtValue::RtValue(const ValuePtr& v) : 
    _data{0,0}
{
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
    else if (v->isA<unsigned int>())
    {
        asUInt() = v->asA<unsigned int>();
    }
    else if (v->isA<size_t>())
    {
        asSize() = v->asA<size_t>();
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

}
