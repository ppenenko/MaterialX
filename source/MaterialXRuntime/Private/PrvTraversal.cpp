//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/Private/PrvTraversal.h>
#include <MaterialXRuntime/Private/PrvElement.h>

namespace MaterialX
{

PrvStageIterator::PrvStageIterator() :
    _current(nullptr),
    _filter(nullptr)
{
}

PrvStageIterator::PrvStageIterator(PrvObjectHandle root, RtTraversalFilter filter) :
    _current(nullptr),
    _filter(filter)
{
    if (root->hasApi(RtApiType::STAGE))
    {
        // Initialize the stack and start iteration to the first element.
        PrvStage* stage = root->asA<PrvStage>();
        _stack.push_back(std::make_tuple(stage, -1, -1));
        ++*this;
    }
}

PrvStageIterator::PrvStageIterator(const PrvStageIterator& other) :
    _current(other._current),
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
            abort();
            return *this;
        }

        StackFrame& frame = _stack.back();
        PrvStage* stage = std::get<0>(frame);
        int& elemIndex = std::get<1>(frame);
        int& stageIndex = std::get<2>(frame);

        bool pop = true;

        if (elemIndex + 1 < int(stage->getElements().size()))
        {
            _current = stage->getElements()[++elemIndex];
            if (!_filter || _filter(RtObject(_current)))
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
                _current = refStage->getElements()[0];
                if (!_filter || _filter(RtObject(_current)))
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


PrvTreeIterator::PrvTreeIterator() :
    _current(nullptr),
    _filter(nullptr)
{
}

PrvTreeIterator::PrvTreeIterator(PrvObjectHandle root, RtTraversalFilter filter) :
    _current(nullptr),
    _filter(filter)
{
    if (root->hasApi(RtApiType::COMPOUND_ELEMENT))
    {
        // Initialize the stack and start iteration to the first element.
        PrvCompoundElement* comp = root->asA<PrvCompoundElement>();
        _stack.push_back(std::make_tuple(comp, -1, -1));
        ++*this;
    }
}

PrvTreeIterator::PrvTreeIterator(const PrvTreeIterator& other) :
    _current(other._current),
    _stack(other._stack),
    _filter(other._filter)
{
}

PrvTreeIterator& PrvTreeIterator::operator++()
{
    while (true)
    {
        if (_stack.empty())
        {
            // Traversal is complete.
            abort();
            return *this;
        }

        if (_current && 
            _current->hasApi(RtApiType::COMPOUND_ELEMENT) &&
            !_current->hasApi(RtApiType::STAGE))
        {
            PrvCompoundElement* comp = _current->asA<PrvCompoundElement>();
            _stack.push_back(std::make_tuple(comp, 0, -1));
            _current = comp->getElements()[0];
            if (!_filter || _filter(RtObject(_current)))
            {
                return *this;
            }
        }

        StackFrame& frame = _stack.back();
        PrvCompoundElement* comp = std::get<0>(frame);
        int& elemIndex = std::get<1>(frame);
        int& stageIndex = std::get<2>(frame);

        bool filterUsed = false;

        if (elemIndex + 1 < int(comp->getElements().size()))
        {
            _current = comp->getElements()[++elemIndex];
            if (!_filter || _filter(RtObject(_current)))
            {
                return *this;
            }
            filterUsed = true;
        }
        else if (comp->hasApi(RtApiType::STAGE))
        {
            PrvStage* stage = comp->asA<PrvStage>();
            if (stageIndex + 1 < int(stage->getReferencedStages().size()))
            {
                PrvStage* refStage = stage->getReferencedStages()[++stageIndex]->asA<PrvStage>();
                if (!refStage->getElements().empty())
                {
                    _stack.push_back(std::make_tuple(refStage, 0, stageIndex));
                    _current = refStage->getElements()[0];
                    if (!_filter || _filter(RtObject(_current)))
                    {
                        return *this;
                    }
                    filterUsed = true;
                }
            }
        }

        if (!filterUsed)
        {
            // We got here without the filter being used.
            // So the current stack frame has been completed
            // and we can unroll the stack to previous frame.
            _stack.pop_back();
        }
    }
}



PrvGraphIterator::PrvGraphIterator() :
    _filter(nullptr)
{
}

PrvGraphIterator::PrvGraphIterator(RtPort root, RtTraversalFilter filter) :
    _filter(filter)
{
    if (root.isOutput())
    {
        _current.first = root;
    }
    else
    {
        _current.first = root.getSourcePort();
        _current.second = root;
    }
}

PrvGraphIterator::PrvGraphIterator(const PrvGraphIterator& other) :
    _current(other._current),
    _stack(other._stack),
    _filter(other._filter)
{
}

PrvGraphIterator& PrvGraphIterator::operator++()
{
    if (_current.first.data())
    {
        PrvNode* node = _current.first.data()->asA<PrvNode>();

        // Check if we have any inputs.
        if (node->numPorts() > node->numOutputs())
        {
            // Traverse to the first upstream edge of this element.
            const size_t inputIndex = node->numOutputs();
            _stack.push_back(StackFrame(_current.first, inputIndex));

            RtPort input = node->getPort(inputIndex);
            RtPort output = input.getSourcePort();

            if (output)
            {
                extendPathUpstream(output, input);
                return *this;
            }
        }
    }

    while (true)
    {
        if (_current.first.data())
        {
            returnPathDownstream(_current.first);
        }

        if (_stack.empty())
        {
            // Traversal is complete.
            abort();
            return *this;
        }

        // Traverse to our siblings.
        StackFrame& parentFrame = _stack.back();
        PrvNode* node = parentFrame.first.data()->asA<PrvNode>();
        while (parentFrame.second + 1 < node->numPorts())
        {
            RtPort input = node->getPort(++parentFrame.second);
            RtPort output = input.getSourcePort();
            if (output)
            {
                extendPathUpstream(output, input);
                return *this;
            }
        }

        // Traverse to our parent's siblings.
        returnPathDownstream(parentFrame.first);
        _stack.pop_back();
    }

    return *this;
}

void PrvGraphIterator::extendPathUpstream(const RtPort& upstream, const RtPort& downstream)
{
    // Check for cycles.
    if (_path.count(upstream))
    {
        throw ExceptionRuntimeError("Encountered cycle at element: " + upstream.data()->asA<PrvNode>()->getName().str() + "." + upstream.getName().str());
    }

    // Extend the current path to the new element.
    _path.insert(upstream);
    _current.first = upstream;
    _current.second = downstream;
}

void PrvGraphIterator::returnPathDownstream(const RtPort& upstream)
{
    _path.erase(upstream);
    _current.first = RtPort();
    _current.second = RtPort();
}

}
