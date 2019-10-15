//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTTRAVERSAL_H
#define MATERIALX_RTTRAVERSAL_H

/// @file
/// TODO: Docs

#include <MaterialXRuntime/RtElement.h>

namespace MaterialX
{

/// @class RtElementIterator
/// TODO: Docs
class RtElementIterator
{
public:
    /// Functor base class for predicate used for object filtering.
    /// New predicates should be derived from this base class to
    /// implemented custom behaviour.
    class Filter
    {
    public:
        virtual bool operator()(const RtObject& obj) = 0;
    };

    class ObjectFilter : public Filter
    {
    public:
        ObjectFilter(RtObjType type) : _type(type) {}
        bool operator()(const RtObject& obj) override
        {
            return obj.getObjType() == _type;
        }
    protected:
        RtObjType _type;
    };

    class ApiFilter : public Filter
    {
    public:
        ApiFilter(RtApiType type) : _type(type) {}
        bool operator()(const RtObject& obj) override
        {
            return obj.hasApi(_type);
        }
    protected:
        RtApiType _type;
    };

public:
    /// Constructor
    RtElementIterator(RtObject root, Filter* filter = nullptr);

};

}

#endif
