//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/Private/PrvTraversal.h>
#include <MaterialXRuntime/Private/PrvElement.h>

namespace MaterialX
{

PrvStageIterator::PrvStageIterator() :
    _elem(nullptr),
    _filter(nullptr)
{
}

PrvStageIterator::PrvStageIterator(PrvObjectHandle root, RtTraversalFilter* filter) :
    _elem(nullptr),
    _filter(filter)
{
    if (root->hasApi(RtApiType::STAGE))
    {
        PrvStage* stage = root->asA<PrvStage>();
        _stack.push_back(std::make_tuple(stage, -1, -1));
        ++*this;
    }
}

PrvStageIterator::PrvStageIterator(const PrvStageIterator& other) :
    _elem(other._elem),
    _stack(other._stack),
    _filter(other._filter)
{
}

PrvStageIterator& PrvStageIterator::operator++()
{
    while (true)
    {
        if (_stack.empty())
        {
            // Traversal is complete.
            _elem = PrvObjectHandle();
            return *this;
        }

        StackFrame& frame = _stack.back();
        PrvStage* stage = std::get<0>(frame);
        int& elemIndex = std::get<1>(frame);
        int& stageIndex = std::get<2>(frame);

        bool pop = true;

        if (elemIndex + 1 < int(stage->getElements().size()))
        {
            _elem = stage->getElements()[++elemIndex];
            if (!_filter || (*_filter)(RtObject(_elem)))
            {
                return *this;
            }
            pop = false;
        }
        else if (stageIndex + 1 < int(stage->getReferencedStages().size()))
        {
            PrvStage* refStage = stage->getReferencedStages()[++stageIndex]->asA<PrvStage>();
            if (!refStage->getElements().empty())
            {
                _stack.push_back(std::make_tuple(refStage, 0, stageIndex));
                _elem = refStage->getElements()[0];
                if (!_filter || (*_filter)(RtObject(_elem)))
                {
                    return *this;
                }
                pop = false;
            }
        }

        if (pop)
        {
            _stack.pop_back();
        }
    }
}


PrvElementIterator::PrvElementIterator() :
    _elem(nullptr),
    _filter(nullptr)
{
}

PrvElementIterator::PrvElementIterator(PrvObjectHandle root, RtTraversalFilter* filter) :
    _elem(root),
    _filter(filter)
{
    // Move into the root
    ++*this;
}

PrvElementIterator::PrvElementIterator(const PrvElementIterator& other) :
    _elem(other._elem),
    _stack(other._stack),
    _filter(other._filter)
{
}

PrvElementIterator& PrvElementIterator::operator++()
{
    if (_elem && _elem->hasApi(RtApiType::COMPOUND_ELEMENT))
    {
        PrvCompoundElement* comp = _elem->asA<PrvCompoundElement>();
        if (!comp->getElements().empty())
        {
            // Traverse to the first child of this element.
            _stack.push_back(StackFrame(_elem, 0));
            _elem = comp->getElements()[0];
            return *this;
        }
    }

    while (true)
    {
        if (_stack.empty())
        {
            // Traversal is complete.
            _elem = PrvObjectHandle();
            return *this;
        }

        // Traverse to our siblings.
        StackFrame& parentFrame = _stack.back();
        PrvCompoundElement* comp = parentFrame.first->asA<PrvCompoundElement>();
        const PrvObjectHandleVec& siblings = comp->getElements();
        if (parentFrame.second + 1 < siblings.size())
        {
            _elem = siblings[++parentFrame.second];
            return *this;
        }

        // Traverse to our parent's siblings.
        _stack.pop_back();
    }
}


}
