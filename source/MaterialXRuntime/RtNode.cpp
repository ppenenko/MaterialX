//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtNode.h>
#include <MaterialXRuntime/RtObject.h>

#include <MaterialXRuntime/private/RtNodeData.h>

namespace MaterialX
{

RtNode::RtNode(const RtObject& obj) :
    RtElement(obj)
{
}

RtApiType RtNode::getApiType() const
{
    return RtApiType::NODE;
}

}
