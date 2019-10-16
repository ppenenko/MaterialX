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

/// Functor base class for filtering during traversal.
/// New filters should be derived from this base class
/// to implemented custom behaviour.
class RtTraversalFilter
{
public:
    virtual bool operator()(const RtObject& obj) = 0;
};

/// Traversal filter for specific object types.
class RtObjectFilter : public RtTraversalFilter
{
public:
    RtObjectFilter(RtObjType type) : _type(type) {}
    bool operator()(const RtObject& obj) override
    {
        return obj.getObjType() == _type;
    }
protected:
    RtObjType _type;
};

/// Traversal filter for specific API support.
class RtApiFilter : public RtTraversalFilter
{
public:
    RtApiFilter(RtApiType type) : _type(type) {}
    bool operator()(const RtObject& obj) override
    {
        return obj.hasApi(_type);
    }
protected:
    RtApiType _type;
};


/// @class RtStageIterator
/// TODO: Docs
class RtStageIterator : public RtApiBase
{
public:
    /// Constructor
    RtStageIterator();

    /// Constructor
    RtStageIterator(RtObject root, RtTraversalFilter* filter = nullptr);

    /// Copy constructor.
    RtStageIterator(const RtStageIterator& other);

    /// Destructor
    ~RtStageIterator();

    /// Return the type for this API.
    RtApiType getApiType() const override;

    /// Equality operator.
    bool operator==(const RtStageIterator& other) const;

    /// Inequality operator.
    bool operator!=(const RtStageIterator& other) const;

    /// Iterate to the next element in the traversal.
    RtStageIterator& operator++();

    /// Dereference this iterator, returning the current element in the
    /// traversal.
    RtObject operator*() const;

    /// Return true if there are no more elements in the interation.
    bool isDone() const;

private:
    void* _ptr;
};


/// @class RtElementIterator
/// TODO: Docs
class RtElementIterator : public RtApiBase
{
public:
    /// Constructor
    RtElementIterator();

    /// Constructor
    RtElementIterator(RtObject root, RtTraversalFilter* filter = nullptr);

    /// Copy constructor.
    RtElementIterator(const RtElementIterator& other);

    /// Destructor
    ~RtElementIterator();

    /// Return the type for this API.
    RtApiType getApiType() const override;

    /// Equality operator.
    bool operator==(const RtElementIterator& other) const;

    /// Inequality operator.
    bool operator!=(const RtElementIterator& other) const;

    /// Iterate to the next element in the traversal.
    RtElementIterator& operator++();

    /// Dereference this iterator, returning the current element in the
    /// traversal.
    RtObject operator*() const;

    /// Return true if there are no more elements in the interation.
    bool isDone() const;

private:
    void* _ptr;
};

}

#endif
