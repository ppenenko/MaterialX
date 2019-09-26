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

TEST_CASE("Runtime: NodeDef", "[runtime]")
{
    mx::RtStage stage = mx::RtStage::create("root");
    mx::RtNodeDef nodeDef = stage.addNodeDef("ND_add_float", "add");

    mx::RtAttribute out = nodeDef.addAttribute("out", "float", mx::RtAttrFlag::OUTPUT | mx::RtAttrFlag::CONNECTABLE);
    mx::RtAttribute in1 = nodeDef.addAttribute("in1", "float", mx::RtAttrFlag::INPUT | mx::RtAttrFlag::CONNECTABLE);
    mx::RtAttribute in2 = nodeDef.addAttribute("in2", "float", mx::RtAttrFlag::INPUT | mx::RtAttrFlag::CONNECTABLE);
    REQUIRE(out.isValid());
    REQUIRE(out.getType() == "float");
    REQUIRE(out.getValue().asFloat() == 0.0f);

    mx::RtAttribute foo = nodeDef.getAttribute("foo");
    REQUIRE(!foo.isValid());

    mx::RtAttribute tmp = nodeDef.getAttribute("in1");
    REQUIRE(tmp.isValid());
    REQUIRE(tmp == in1);
    REQUIRE(tmp.getObject() == in1.getObject());



    in1.setValue(42.0f);
    REQUIRE(in1.getValue().asFloat() == 42.0f);

    in1.addMetaData("uiName", "string", mx::RtValue());
}
