//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RT_NODE_H
#define MATERIALX_RT_NODE_H

/// @file
/// TODO: Docs

#include <MaterialXRuntime/RtElement.h>
#include <MaterialXRuntime/RtAttribute.h>

namespace MaterialX
{


class RtPort
{
public:
    
    bool connectTo();

private:
    void* _data;
};

/// @class RtNodeDef API
/// TODO: Docs
class RtNode : public RtElement
{
public:
    /// Constructor attaching and object to the API.
    RtNode(const RtObject& obj);

    /// Return the type for this object.
    RtApiType getApiType() const override;

    /// Return the node type for this node.
    const RtToken& getNodeType() const;

    /// Return an input port by name, or a null object
    /// if no such input attribute exists.
    RtPort getInputPort(const RtToken& name) const;

    /// Return an output attribute by name, or a null object
    /// if no such output attribute exists.
    RtPort getOutputPort(const RtToken& name) const;
};

}

#endif
