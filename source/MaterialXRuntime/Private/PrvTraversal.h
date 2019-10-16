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

    /// Constructor, setting the root element to start
    /// the iteration on and an optional filter functor.
    PrvStageIterator(PrvObjectHandle root, RtTraversalFilter* filter = nullptr);

    /// Copy constructor.
    PrvStageIterator(const PrvStageIterator& other);

    bool operator==(const PrvStageIterator& other) const
    {
        return _elem == other._elem &&
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
        return _elem;
    }

    /// Iterate to the next element in the traversal.
    PrvStageIterator& operator++();

    /// Return the current element in the traversal.
    PrvObjectHandle getElement() const
    {
        return _elem;
    }

    /// Return true if there are no more elements in the iteration.
    bool isDone() const
    {
        return _elem == nullptr;
    }

private:
    using StackFrame = std::tuple<PrvStage*, int, int>;

    PrvObjectHandle _elem;
    vector<StackFrame> _stack;
    RtTraversalFilter* _filter;
};


/// @class PrvElementIterator
/// TODO: Docs
class PrvElementIterator
{
public:
    /// Empty constructor.
    PrvElementIterator();

    /// Constructor, setting the root element to start
    /// the iteration on and an optional filter functor.
    PrvElementIterator(PrvObjectHandle root, RtTraversalFilter* filter = nullptr);

    /// Copy constructor.
    PrvElementIterator(const PrvElementIterator& other);

    bool operator==(const PrvElementIterator& other) const
    {
        return _elem == other._elem &&
            _stack == other._stack;
    }
    bool operator!=(const PrvElementIterator& other) const
    {
        return !(*this == other);
    }

    /// Dereference this iterator, returning the current element in the
    /// traversal.
    PrvObjectHandle operator*() const
    {
        return _elem;
    }

    /// Iterate to the next element in the traversal.
    PrvElementIterator& operator++();

    /// Return the current element in the traversal.
    PrvObjectHandle getElement() const
    {
        return _elem;
    }

    /// Return true if there are no more elements in the iteration.
    bool isDone() const
    {
        return _elem == nullptr;
    }

private:
    using StackFrame = std::pair<PrvObjectHandle, size_t>;

    PrvObjectHandle _elem;
    vector<StackFrame> _stack;
    RtTraversalFilter* _filter;
};

}

#endif
