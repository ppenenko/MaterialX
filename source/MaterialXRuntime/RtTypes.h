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
    static const RtToken FLOAT;
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
    static const RtToken INTEGERARRAY;
    static const RtToken FLOATARRAY;
    static const RtToken BSDF;
    static const RtToken EDF;
    static const RtToken VDF;
    static const RtToken SURFACESHADER;
    static const RtToken VOLUMESHADER;
    static const RtToken DISPLACEMENTSHADER;
    static const RtToken LIGHTSHADER;
    static const RtToken SURFACEMATERIAL;
    static const RtToken VOLUMEMATERIAL;
    static const RtToken AUTO;
};

using ChannelMap = std::unordered_map<char, int>;

/// @class RtTypeDesc
/// A type descriptor for MaterialX data types.
/// All types need to have a type descriptor registered in order for the runtime
/// to know about the type. A unique type descriptor pointer can be used for type
/// comparisons as well as getting more information about the type.
/// All standard library data types are registered by default. Custom types can be
/// registered using RtTypeDesc::registerType().
class RtTypeDesc
{
public:
    enum BaseType
    {
        BASETYPE_NONE,
        BASETYPE_BOOLEAN,
        BASETYPE_INTEGER,
        BASETYPE_FLOAT,
        BASETYPE_STRING,
        BASETYPE_STRUCT,
        BASETYPE_LAST
    };

    enum Semantic
    {
        SEMANTIC_NONE,
        SEMANTIC_COLOR,
        SEMANTIC_VECTOR,
        SEMANTIC_MATRIX,
        SEMANTIC_FILENAME,
        SEMANTIC_CLOSURE,
        SEMANTIC_SHADER,
        SEMANTIC_LAST
    };

public:
    /// Constructor.
    RtTypeDesc(const string& name, unsigned char basetype, unsigned char semantic, size_t size);

    /// Destructor.
    ~RtTypeDesc();

    /// Return the name of the type.
    const string& getName() const;

    /// Return the basetype for the type.
    unsigned char getBaseType() const;

    /// Return the semantic for the type.
    unsigned char getSemantic() const;

    /// Return the number of elements the type is composed of.
    /// Will return 1 for scalar types and a size greater than 1 for aggregate type.
    /// For array types 0 is returned since the number of elements is undefined
    /// until an array is instantiated.
    size_t getSize() const;

    /// Returns the channel index for the supplied channel name.
    /// Will return -1 on failure to find a matching index.
    int getChannelIndex(char channel) const;

    /// Set the channel index for the supplied channel name.
    void setChannelIndex(char channel, int index);

    /// Return true if the type is a scalar type.
    bool isScalar() const
    {
        return getSize() == 1;
    }

    /// Return true if the type is an aggregate type.
    bool isAggregate() const
    {
        return getSize() > 1;
    }

    /// Return true if the type is an array type.
    bool isArray() const
    {
        return getSize() == 0;
    }

    /// Return true if the type is an aggregate of 2 floats.
    bool isFloat2() const 
    { 
        return getSize() == 2 && (getSemantic() == SEMANTIC_COLOR || getSemantic() == SEMANTIC_VECTOR);
    }

    /// Return true if the type is an aggregate of 3 floats.
    bool isFloat3() const
    {
        return getSize() == 3 && (getSemantic() == SEMANTIC_COLOR || getSemantic() == SEMANTIC_VECTOR);
    }

    /// Return true if the type is an aggregate of 4 floats.
    bool isFloat4() const
    {
        return getSize() == 4 && (getSemantic() == SEMANTIC_COLOR || getSemantic() == SEMANTIC_VECTOR);
    }

    /// Register a type descriptor for a MaterialX data type.
    /// Throws an exception if a type with the same name is already registered.
    static RtTypeDesc* registerType(const RtToken& name, unsigned char basetype,
                                    unsigned char semantic = SEMANTIC_NONE, size_t size = 1);

    /// Get a type descriptor for given type name.
    /// Returns nullptr if no such type is registered.
    static const RtTypeDesc* findType(const RtToken& name);

private:
    void* _ptr;
};

} // namespace MaterialX

#endif
