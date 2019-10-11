//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTTYPES_H
#define MATERIALX_RTTYPES_H

/// @file
/// Type descriptors for a runtime data types.

#include <MaterialXRuntime/RtToken.h>

namespace MaterialX
{

/// Class holding tokens for the built in data types
class RtType
{
public:
    static const RtToken BOOLEAN;
    static const RtToken INTEGER;
    static const RtToken INTEGERARRAY;
    static const RtToken FLOAT;
    static const RtToken FLOATARRAY;
    static const RtToken VECTOR2;
    static const RtToken VECTOR3;
    static const RtToken VECTOR4;
    static const RtToken COLOR2;
    static const RtToken COLOR3;
    static const RtToken COLOR4;
    static const RtToken MATRIX33;
    static const RtToken MATRIX44;
    static const RtToken STRING;
    static const RtToken FILENAME;
    static const RtToken BSDF;
    static const RtToken EDF;
    static const RtToken VDF;
    static const RtToken SURFACESHADER;
    static const RtToken VOLUMESHADER;
    static const RtToken DISPLACEMENTSHADER;
    static const RtToken LIGHTSHADER;
    static const RtToken SURFACEMATERIAL;
    static const RtToken VOLUMEMATERIAL;
};

} // namespace MaterialX

#endif
