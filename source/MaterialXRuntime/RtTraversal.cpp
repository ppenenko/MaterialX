//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtTraversal.h>

#include <MaterialXRuntime/Private/PrvTraversal.h>

namespace MaterialX
{

RtStageIterator::RtStageIterator() :
    RtApiBase(),
    _ptr(new PrvStageIterator())
{
}

RtStageIterator::RtStageIterator(RtObject root, RtTraversalFilter* filter) :
    RtApiBase(root),
    _ptr(new PrvStageIterator(RtApiBase::data(root), filter))
{
}

RtStageIterator::RtStageIterator(const RtStageIterator& other) :
    RtApiBase(other),
    _ptr(new PrvStageIterator(*static_cast<PrvStageIterator*>(other._ptr)))
{
}

RtStageIterator::~RtStageIterator()
{
    PrvStageIterator* it = static_cast<PrvStageIterator*>(_ptr);
    delete it;
}

RtApiType RtStageIterator::getApiType() const
{
    return RtApiType::ELEMENT_ITERATOR;
}

bool RtStageIterator::operator==(const RtStageIterator& other) const
{
    PrvStageIterator* lhs = static_cast<PrvStageIterator*>(_ptr);
    PrvStageIterator* rhs = static_cast<PrvStageIterator*>(other._ptr);
    return lhs->operator==(*rhs);
}
bool RtStageIterator::operator!=(const RtStageIterator& other) const
{
    return !(*this == other);
}

RtObject RtStageIterator::operator*() const
{
    PrvStageIterator* it = static_cast<PrvStageIterator*>(_ptr);
    return RtObject(it->operator*());
}

RtStageIterator& RtStageIterator::operator++()
{
    PrvStageIterator* it = static_cast<PrvStageIterator*>(_ptr);
    it->operator++();
    return *this;
}

bool RtStageIterator::isDone() const
{
    PrvStageIterator* it = static_cast<PrvStageIterator*>(_ptr);
    return it->isDone();
}


RtElementIterator::RtElementIterator() :
    RtApiBase(),
    _ptr(new PrvElementIterator())
{
}

RtElementIterator::RtElementIterator(RtObject root, RtTraversalFilter* filter) :
    RtApiBase(root),
    _ptr(new PrvElementIterator(RtApiBase::data(root), filter))
{
}

RtElementIterator::RtElementIterator(const RtElementIterator& other) :
    RtApiBase(other),
    _ptr(new PrvElementIterator(*static_cast<PrvElementIterator*>(other._ptr)))
{
}

RtElementIterator::~RtElementIterator()
{
    PrvElementIterator* it = static_cast<PrvElementIterator*>(_ptr);
    delete it;
}

RtApiType RtElementIterator::getApiType() const
{
    return RtApiType::ELEMENT_ITERATOR;
}

bool RtElementIterator::operator==(const RtElementIterator& other) const
{
    PrvElementIterator* lhs = static_cast<PrvElementIterator*>(_ptr);
    PrvElementIterator* rhs = static_cast<PrvElementIterator*>(other._ptr);
    return lhs->operator==(*rhs);
}
bool RtElementIterator::operator!=(const RtElementIterator& other) const
{
    return !(*this==other);
}

RtObject RtElementIterator::operator*() const
{
    PrvElementIterator* it = static_cast<PrvElementIterator*>(_ptr);
    return RtObject(it->operator*());
}

RtElementIterator& RtElementIterator::operator++()
{
    PrvElementIterator* it = static_cast<PrvElementIterator*>(_ptr);
    it->operator++();
    return *this;
}

bool RtElementIterator::isDone() const
{
    PrvElementIterator* it = static_cast<PrvElementIterator*>(_ptr);
    return it->isDone();
}

}
