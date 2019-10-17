//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTTRAVERSAL_H
#define MATERIALX_RTTRAVERSAL_H

/// @file
/// TODO: Docs

#include <MaterialXRuntime/RtObject.h>

namespace MaterialX
{

/// Filter function type used for filtering objects during traversal.
using RtTraversalFilter = std::function<bool(const RtObject & obj)>;

/// Traversal filter for specific object types.
template<RtObjType T>
struct RtObjectFilter
{
    bool operator()(const RtObject& obj)
    {
        return obj.getObjType() == T;
    }
};

/// Traversal filter for specific API support.
template<RtApiType T>
struct RtApiFilter
{
    bool operator()(const RtObject& obj)
    {
        return obj.hasApi(T);
    }
};


/// @class RtStageIterator
/// TODO: Docs
class RtStageIterator : public RtApiBase
{
public:
    /// EMpty constructor.
    RtStageIterator();

    /// Constructor, setting the stage to iterate on
    /// and optionally a filter restricting the set of 
    /// returned objects.
    RtStageIterator(RtObject stage, RtTraversalFilter filter = nullptr);

    /// Copy constructor.
    RtStageIterator(const RtStageIterator& other);

    /// Destructor.
    ~RtStageIterator();

    /// Return the type for this API.
    RtApiType getApiType() const override;

    /// Equality operator.
    bool operator==(const RtStageIterator& other) const;

    /// Inequality operator.
    bool operator!=(const RtStageIterator& other) const;

    /// Iterate to the next element in the traversal.
    RtStageIterator& operator++();

    /// Dereference this iterator, returning the current object
    /// in the traversal.
    RtObject operator*() const;

    /// Return true if there are no more objects in the traversal.
    bool isDone() const;

    /// Force the iterator to terminate the traversal.
    void abort();

private:
    void* _ptr;
};


/// @class RtTreeIterator
/// TODO: Docs
class RtTreeIterator : public RtApiBase
{
public:
    /// Constructor
    RtTreeIterator();

    /// Constructor
    RtTreeIterator(RtObject root, RtTraversalFilter filter = nullptr);

    /// Copy constructor.
    RtTreeIterator(const RtTreeIterator& other);

    /// Destructor
    ~RtTreeIterator();

    /// Return the type for this API.
    RtApiType getApiType() const override;

    /// Equality operator.
    bool operator==(const RtTreeIterator& other) const;

    /// Inequality operator.
    bool operator!=(const RtTreeIterator& other) const;

    /// Iterate to the next element in the traversal.
    RtTreeIterator& operator++();

    /// Dereference this iterator, returning the current element in the
    /// traversal.
    RtObject operator*() const;

    /// Return true if there are no more elements in the interation.
    bool isDone() const;

    /// Force the iterator to terminate the traversal.
    void abort();

private:
    void* _ptr;
};

}

#endif
