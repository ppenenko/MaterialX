//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXTest/Catch/catch.hpp>

#include <MaterialXCore/Document.h>
#include <MaterialXCore/Observer.h>

#include <MaterialXFormat/XmlIo.h>
#include <MaterialXFormat/File.h>

#include <MaterialXRuntime/RtValue.h>
#include <MaterialXRuntime/RtStage.h>
#include <MaterialXRuntime/RtNodeDef.h>
#include <MaterialXRuntime/RtNode.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

namespace mx = MaterialX;

TEST_CASE("Runtime: Values", "[runtime]")
{
    mx::RtValue v1, v2, v3;
    v1.asInt() = 42;
    v2.asFloat() = 1.0f;
    v3.asFloat() = 2.0f;
    REQUIRE(v1.asInt() == 42);
    REQUIRE(v2.asFloat() + v3.asFloat() == 3.0f);

    mx::RtValue color3;
    color3.asColor3() = mx::Color3(1.0f, 0.0f, 0.0f);
    REQUIRE(color3.asColor3()[0] == 1.0f);
    REQUIRE(color3.asFloat() == 1.0f);

    mx::RtValue vector4;
    vector4.asVector4() = mx::Vector4(4.0f, 3.0f, 2.0f, 1.0f);
    REQUIRE(vector4.asVector4()[0] == 4.0f);
    REQUIRE(vector4.asVector4()[1] == 3.0f);
    REQUIRE(vector4.asVector4()[2] == 2.0f);
    REQUIRE(vector4.asVector4()[3] == 1.0f);

    mx::RtValue v4 = vector4;
    REQUIRE(v4.asA<mx::Vector4>()[0] == 4.0f);

    mx::RtValue ptr;
    ptr.asPtr() = &vector4;
    REQUIRE(ptr.asPtr() == &vector4);

    ptr.clear();
    REQUIRE(ptr.asPtr() == (void*)0);
}

TEST_CASE("Runtime: Nodes", "[runtime]")
{
    mx::RtStage stage = mx::RtStage::create("root");

    mx::RtObject nodedefObj = mx::RtNodeDef::create("ND_add_float", "add", stage.getObject());
    REQUIRE(nodedefObj.hasApi(mx::RtApiType::NODEDEF));
    mx::RtNodeDef nodedef(nodedefObj);

    mx::RtAttribute attr = mx::RtAttribute::create("custom", "string", mx::RtValue());
    nodedef.addAttribute(attr.getObject());
    REQUIRE(nodedef.numAttributes() == 1);

    mx::RtAttribute in1 = mx::RtAttribute::create("in1", "float", mx::RtValue(1.0f), mx::RtAttrFlag::INPUT | mx::RtAttrFlag::CONNECTABLE);
    mx::RtAttribute in2 = mx::RtAttribute::create("in2", "float", mx::RtValue(42.0f), mx::RtAttrFlag::INPUT | mx::RtAttrFlag::CONNECTABLE);
    mx::RtAttribute out = mx::RtAttribute::create("out", "float", mx::RtValue(0.0f), mx::RtAttrFlag::OUTPUT | mx::RtAttrFlag::CONNECTABLE);
    REQUIRE(out.isValid());
    REQUIRE(out.getType() == "float");
    REQUIRE(out.getValue().asFloat() == 0.0f);

    nodedef.addPortDef(in1.getObject());
    nodedef.addPortDef(in2.getObject());
    nodedef.addPortDef(out.getObject());
    REQUIRE(nodedef.numPorts() == 3);

    mx::RtAttribute fooAttr = nodedef.getPortDef("foo");
    REQUIRE(!fooAttr.isValid());

    mx::RtAttribute tmp = nodedef.getPortDef("in1");
    REQUIRE(tmp.isValid());
    REQUIRE(tmp == in1);

    in1.setValue(7.0f);
    REQUIRE(in1.getValue().asFloat() == 7.0f);

    REQUIRE_THROWS(mx::RtNode::create("foo", mx::RtObject(), stage.getObject()));

    mx::RtNode add1 = mx::RtNode::create("add1", nodedef.getObject(), stage.getObject());
    mx::RtNode add2 = mx::RtNode::create("add2", nodedef.getObject(), stage.getObject());
    REQUIRE(add1.isValid());
    REQUIRE(add2.isValid());

    mx::RtPort add1_in1 = add1.getPort("in1");
    mx::RtPort add1_in2 = add1.getPort("in2");
    mx::RtPort add1_out = add1.getPort("out");
    mx::RtPort add2_in1 = add2.getPort("in1");
    mx::RtPort add2_in2 = add2.getPort("in2");
    mx::RtPort add2_out = add2.getPort("out");

    mx::RtNode::connect(add1_out, add2_in1);
    REQUIRE(add1_out.isConnected());
    REQUIRE(add2_in1.isConnected());
    REQUIRE_THROWS(mx::RtNode::connect(add1_out, add2_in1));

    mx::RtNode::disconnect(add1_out, add2_in1);
    REQUIRE(!add1_out.isConnected());
    REQUIRE(!add2_in1.isConnected());

    mx::RtNode::connect(add1_out, add2_in1);
    mx::RtNode::connect(add1_out, add2_in2);
    REQUIRE(add2_in1.getConnectionSource() != mx::RtPort());
    REQUIRE(add1_out.getConnectionDestinations().size() == 2);
}
