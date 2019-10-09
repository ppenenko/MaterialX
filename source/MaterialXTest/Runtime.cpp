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
#include <MaterialXRuntime/RtPortDef.h>
#include <MaterialXRuntime/RtNodeDef.h>
#include <MaterialXRuntime/RtNode.h>
#include <MaterialXRuntime/RtNodeGraph.h>
#include <MaterialXRuntime/RtCoreIo.h>

#include <MaterialXGenShader/Util.h>

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
    mx::RtObject stageObj = mx::RtStage::create("root");
    mx::RtStage stage(stageObj);

    // Create a new nodedef object for defining an add node
    mx::RtObject addNodeObj = mx::RtNodeDef::create("ND_add_float", "add", stageObj);
    REQUIRE(addNodeObj.hasApi(mx::RtApiType::NODEDEF));

    // Attach the nodedef API to this object
    mx::RtNodeDef addNode(addNodeObj);

    // Add an attribute to the nodedef
    addNode.addAttribute("version", "float", mx::RtValue(1.0f));
    REQUIRE(addNode.numAttributes() == 1);

    // Add ports to the nodedef
    mx::RtPortDef::create("in1", "float", mx::RtValue(1.0f), mx::RtPortFlag::INPUT | mx::RtPortFlag::CONNECTABLE, addNodeObj);
    mx::RtPortDef::create("in2", "float", mx::RtValue(42.0f), mx::RtPortFlag::INPUT | mx::RtPortFlag::CONNECTABLE, addNodeObj);
    mx::RtPortDef::create("out", "float", mx::RtValue(0.0f), mx::RtPortFlag::OUTPUT | mx::RtPortFlag::CONNECTABLE, addNodeObj);
    REQUIRE(addNode.numPorts() == 3);

    // Test the new ports
    mx::RtPortDef out = addNode.getPortDef("out");
    REQUIRE(out.isValid());
    REQUIRE(out.isOutput());
    REQUIRE(out.isConnectable());
    REQUIRE(!out.isUniform());
    REQUIRE(out.getType() == "float");
    REQUIRE(out.getValue().asFloat() == 0.0f);
    mx::RtPortDef foo = addNode.getPortDef("foo");
    REQUIRE(!foo.isValid());
    mx::RtPortDef in1 = addNode.getPortDef("in1");
    REQUIRE(in1.isValid());
    REQUIRE(in1.isInput());
    REQUIRE(in1.isConnectable());
    in1.setValue(7.0f);
    REQUIRE(in1.getValue().asFloat() == 7.0f);

    // Try to create a node from an invalid nodedef object
    REQUIRE_THROWS(mx::RtNode::create("foo", mx::RtObject(), stageObj));

    // Create two new node instances form the add nodedef
    mx::RtObject add1Obj = mx::RtNode::create("add1", addNodeObj, stageObj);
    mx::RtObject add2Obj = mx::RtNode::create("add2", addNodeObj, stageObj);
    REQUIRE(add1Obj.isValid());
    REQUIRE(add2Obj.isValid());

    // Attach the node API to these objects
    mx::RtNode add1(add1Obj);
    mx::RtNode add2(add2Obj);

    // Get the node instance ports
    mx::RtPort add1_in1 = add1.getPort("in1");
    mx::RtPort add1_in2 = add1.getPort("in2");
    mx::RtPort add1_out = add1.getPort("out");
    mx::RtPort add2_in1 = add2.getPort("in1");
    mx::RtPort add2_in2 = add2.getPort("in2");
    mx::RtPort add2_out = add2.getPort("out");

    // Make port connections
    mx::RtNode::connect(add1_out, add2_in1);
    REQUIRE(add1_out.isConnected());
    REQUIRE(add2_in1.isConnected());

    // Try connecting already connected ports
    REQUIRE_THROWS(mx::RtNode::connect(add1_out, add2_in1));

    // Break port connections
    mx::RtNode::disconnect(add1_out, add2_in1);
    REQUIRE(!add1_out.isConnected());
    REQUIRE(!add2_in1.isConnected());

    // Make more port connections
    mx::RtNode::connect(add1_out, add2_in1);
    mx::RtNode::connect(add1_out, add2_in2);
    size_t numDest = add1_out.numDestinationPorts();
    REQUIRE(numDest == 2);
    REQUIRE(add1_out.getDestinationPort(0) == add2_in1);
    REQUIRE(add1_out.getDestinationPort(1) == add2_in2);
    REQUIRE(add2_in1.getSourcePort() == add1_out);
    REQUIRE(add2_in2.getSourcePort() == add1_out);

    // Create a nodegraph object
    mx::RtObject graph1Obj = mx::RtNodeGraph::create("graph1", stageObj);
    REQUIRE(graph1Obj.isValid());

    // Attach the nodegraph API to the object
    mx::RtNodeGraph graph1(graph1Obj);

    // Create two node instance in the graph
    mx::RtObject add3Obj = mx::RtNode::create("add3", addNodeObj, graph1Obj);
    mx::RtObject add4Obj = mx::RtNode::create("add4", addNodeObj, graph1Obj);
    REQUIRE(graph1.numNodes() == 2);

    // Create a new nodedef object for defining the graph interface
    mx::RtObject bobNodeObj = mx::RtNodeDef::create("ND_bob", "bob", stageObj);
    REQUIRE(bobNodeObj.hasApi(mx::RtApiType::NODEDEF));

    // Attach the nodedef API to this object
    mx::RtNodeDef bobNode(bobNodeObj);

    // Add ports to the nodedef
    mx::RtPortDef::create("a", "float", mx::RtValue(0.0f), mx::RtPortFlag::INPUT | mx::RtPortFlag::CONNECTABLE, bobNodeObj);
    mx::RtPortDef::create("b", "float", mx::RtValue(0.0f), mx::RtPortFlag::INPUT | mx::RtPortFlag::CONNECTABLE, bobNodeObj);
    mx::RtPortDef::create("out", "float", mx::RtValue(0.0f), mx::RtPortFlag::OUTPUT | mx::RtPortFlag::CONNECTABLE, bobNodeObj);
    REQUIRE(bobNode.numPorts() == 3);

    // Set the interface and test the interface nodes
    graph1.setInterface(bobNodeObj);
    mx::RtNode inputs = graph1.getInputsNode();
    REQUIRE(inputs.numPorts() == 2);
    mx::RtNode outputs = graph1.getOutputsNode();
    REQUIRE(outputs.numPorts() == 1);

    mx::RtNode add3(add3Obj);
    mx::RtNode add4(add4Obj);

    // Connect the graph nodes to each other and the interface
    mx::RtNode::connect(inputs.getPort("a"), add3.getPort("in1"));
    mx::RtNode::connect(inputs.getPort("b"), add3.getPort("in2"));
    mx::RtNode::connect(add3.getPort("out"), add4.getPort("in1"));
    mx::RtNode::connect(inputs.getPort("a"), add4.getPort("in2"));
    mx::RtNode::connect(add4.getPort("out"), outputs.getPort("out"));
    REQUIRE(inputs.getPort("a").numDestinationPorts() == 2);
    REQUIRE(inputs.getPort("b").numDestinationPorts() == 1);
    REQUIRE(outputs.getPort("out").getSourcePort() == add4.getPort("out"));

    // Find object by path
    mx::RtObject elem1 = stage.findElement("/add1/in2");
    REQUIRE(elem1.isValid());
    REQUIRE(elem1.hasApi(mx::RtApiType::PORTDEF));
    REQUIRE(mx::RtPortDef(elem1).getName() == "in2");
    REQUIRE(mx::RtPortDef(elem1).isInput());

    // Find a port by path
    mx::RtObject node = stage.findElement("/graph1/add4");
    mx::RtObject portdef = stage.findElement("/graph1/add4/out");
    REQUIRE(node.isValid());
    REQUIRE(portdef.isValid());
    REQUIRE(node.hasApi(mx::RtApiType::NODE));
    REQUIRE(portdef.hasApi(mx::RtApiType::PORTDEF));
    // Get a port instance from node and portdef
    mx::RtPort port1(node, portdef);
    REQUIRE(port1 == add4.getPort("out"));
}

TEST_CASE("Runtime: CoreIo", "[runtime]")
{
    // Create a document.
    mx::DocumentPtr doc = mx::createDocument();

    // Load in stdlib
    mx::FilePath searchPath = mx::FilePath::getCurrentPath() / mx::FilePath("libraries");
    loadLibraries({ "stdlib" }, searchPath, doc);

    // Create a stage and load the document data.
    mx::RtObject stageObj = mx::RtStage::create("stage");
    mx::RtCoreIo io(stageObj);
    io.read(*doc, false);

    // Get a nodegraph and write a dot file for inspection.
    mx::RtStage stage(stageObj);
    mx::RtNodeGraph graph = stage.getElement("NG_tiledimage_float");
    REQUIRE(graph.isValid());
    std::ofstream dotfile;
    dotfile.open(graph.getName() + ".dot");
    dotfile << graph.asStringDot();
    dotfile.close();
}

TEST_CASE("Runtime: Stage References", "[runtime]")
{
    // Create a main stage.
    mx::RtObject mainStageObj = mx::RtStage::create("main");
    mx::RtStage mainStage(mainStageObj);

    // Load in stdlib in another stage.
    mx::RtObject libStageObj = mx::RtStage::create("libs");
    mx::DocumentPtr doc = mx::createDocument();
    mx::FilePath searchPath = mx::FilePath::getCurrentPath() / mx::FilePath("libraries");
    loadLibraries({ "stdlib", "pbrlib" }, searchPath, doc);
    mx::RtCoreIo io(libStageObj);
    io.read(*doc, false);

    // Reference the library stage .
    mainStage.addReference(libStageObj);

    // Test access and removal of contents from the referenced library.
    mx::RtNodeDef nodedef = mainStage.getElement("ND_add_float");
    REQUIRE(nodedef.isValid());
    REQUIRE_THROWS(mainStage.removeElement(nodedef.getName()));

    // Test instantiation of a node from a referenced nodedef
    mx::RtObject addObj = mx::RtNode::create("add1", nodedef.getObject(), mainStage.getObject());
    REQUIRE(addObj.isValid());

    // Make sure removal of the non-referenced node works
    const mx::RtToken nodeName = mx::RtNode(addObj).getName();
    mainStage.removeElement(nodeName);
    addObj = mainStage.getElement(nodeName);
    REQUIRE(!addObj.isValid());
}
