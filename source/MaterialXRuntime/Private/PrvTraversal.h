//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_PRVTRAVERSAL_H
#define MATERIALX_PRVTRAVERSAL_H

/// @file
/// TODO: Docs

#include <MaterialXRuntime/RtTraversal.h>

#include <MaterialXRuntime/Private/PrvStage.h>

namespace MaterialX
{

/// @class PrvStageIterator
/// TODO: Docs
class PrvStageIterator
{
public:
    /// Empty constructor.
    PrvStageIterator();

    /// Constructor, setting the stage to iterate on
    /// and optionally a filter restricting the set of 
    /// returned objects.
    PrvStageIterator(PrvObjectHandle stage, RtTraversalFilter filter = nullptr);

    /// Copy constructor.
    PrvStageIterator(const PrvStageIterator& other);

    bool operator==(const PrvStageIterator& other) const
    {
        return _current == other._current &&
            _stack == other._stack;
    }

    bool operator!=(const PrvStageIterator& other) const
    {
        return !(*this == other);
    }

    /// Dereference this iterator, returning the current element in the
    /// traversal.
    PrvObjectHandle operator*() const
    {
        return _current;
    }

    /// Iterate to the next element in the traversal.
    PrvStageIterator& operator++();

    /// Return true if there are no more elements in the iteration.
    bool isDone() const
    {
        return _current == nullptr;
    }

    /// Force the iterator to terminate the traversal.
    void abort()
    {
        _current = nullptr;
    }

private:
    using StackFrame = std::tuple<PrvStage*, int, int>;

    PrvObjectHandle _current;
    vector<StackFrame> _stack;
    RtTraversalFilter _filter;
};


/// @class PrvTreeIterator
/// TODO: Docs
class PrvTreeIterator
{
public:
    /// Empty constructor.
    PrvTreeIterator();

    /// Constructor, setting the root element to start
    /// the iteration on and an optional filter functor.
    PrvTreeIterator(PrvObjectHandle root, RtTraversalFilter filter = nullptr);

    /// Copy constructor.
    PrvTreeIterator(const PrvTreeIterator& other);

    bool operator==(const PrvTreeIterator& other) const
    {
        return _current == other._current &&
            _stack == other._stack;
    }

    bool operator!=(const PrvTreeIterator& other) const
    {
        return !(*this == other);
    }

    /// Dereference this iterator, returning the current element in the
    /// traversal.
    PrvObjectHandle operator*() const
    {
        return _current;
    }

    /// Iterate to the next element in the traversal.
    PrvTreeIterator& operator++();

    /// Return true if there are no more elements in the iteration.
    bool isDone() const
    {
        return _current == nullptr;
    }

    /// Force the iterator to terminate the traversal.
    void abort()
    {
        _current = nullptr;
    }

private:
    using StackFrame = std::tuple<PrvCompoundElement*, int, int>;

    PrvObjectHandle _current;
    vector<StackFrame> _stack;
    RtTraversalFilter _filter;
};

}

#endif
