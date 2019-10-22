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
#include <MaterialXRuntime/RtTraversal.h>
#include <MaterialXRuntime/RtTypes.h>

#include <MaterialXGenShader/Util.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

namespace mx = MaterialX;

TEST_CASE("Runtime: Token", "[runtime]")
{
    mx::RtToken tok1 = "hej";
    mx::RtToken tok2 = "hey";
    mx::RtToken tok3 = "hej";
    REQUIRE(tok1 != tok2);
    REQUIRE(tok1 == tok3);
    REQUIRE(tok1 == "hej");
    REQUIRE(tok1 == std::string("hej"));
    REQUIRE("hej" == tok1);
    REQUIRE(std::string("hej") == tok1);
}

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
    mx::RtObject stageObj = mx::RtStage::createNew("root");
    mx::RtStage stage(stageObj);

    // Test validity/bool operators
    REQUIRE(stageObj);
    REQUIRE(!stageObj == false);
    REQUIRE(stage);
    REQUIRE(!stage == false);

    // Create a new nodedef object for defining an add node
    mx::RtObject addNodeObj = mx::RtNodeDef::createNew("ND_add_float", "add", stageObj);
    REQUIRE(addNodeObj.isValid());
    REQUIRE(addNodeObj.hasApi(mx::RtApiType::NODEDEF));

    // Attach the nodedef API to this object
    mx::RtNodeDef addNode(addNodeObj);

    // Add an attribute to the nodedef
    addNode.addAttribute("version", "float", mx::RtValue(1.0f));
    REQUIRE(addNode.numAttributes() == 1);

    // Add ports to the nodedef
    mx::RtPortDef::createNew("in1", "float", mx::RtValue(1.0f), mx::RtPortFlag::INPUT, addNodeObj);
    mx::RtPortDef::createNew("in2", "float", mx::RtValue(42.0f), mx::RtPortFlag::INPUT, addNodeObj);
    mx::RtPortDef::createNew("out", "float", mx::RtValue(0.0f), mx::RtPortFlag::OUTPUT, addNodeObj);
    REQUIRE(addNode.numPorts() == 3);

    // Test the new ports
    mx::RtPortDef out = addNode.findPort("out");
    REQUIRE(out.isValid());
    REQUIRE(out.isOutput());
    REQUIRE(out.isConnectable());
    REQUIRE(!out.isUniform());
    REQUIRE(out.getType() == "float");
    REQUIRE(out.getValue().asFloat() == 0.0f);
    mx::RtPortDef foo = addNode.findPort("foo");
    REQUIRE(!foo.isValid());
    mx::RtPortDef in1 = addNode.findPort("in1");
    REQUIRE(in1.isValid());
    REQUIRE(in1.isInput());
    REQUIRE(in1.isConnectable());
    in1.setValue(7.0f);
    REQUIRE(in1.getValue().asFloat() == 7.0f);

    // Try to create a node from an invalid nodedef object
    REQUIRE_THROWS(mx::RtNode::createNew("foo", mx::RtObject(), stageObj));

    // Create two new node instances from the add nodedef
    mx::RtObject add1Obj = mx::RtNode::createNew("add1", addNodeObj, stageObj);
    mx::RtObject add2Obj = mx::RtNode::createNew("add2", addNodeObj, stageObj);
    REQUIRE(add1Obj.isValid());
    REQUIRE(add2Obj.isValid());

    // Attach the node API to these objects
    mx::RtNode add1(add1Obj);
    mx::RtNode add2(add2Obj);

    // Get the node instance ports
    mx::RtPort add1_in1 = add1.findPort("in1");
    mx::RtPort add1_in2 = add1.findPort("in2");
    mx::RtPort add1_out = add1.findPort("out");
    mx::RtPort add2_in1 = add2.findPort("in1");
    mx::RtPort add2_in2 = add2.findPort("in2");
    mx::RtPort add2_out = add2.findPort("out");

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

    // Make more port connections, now testing
    // the port connectTo method
    add1_out.connectTo(add2_in1);
    add1_out.connectTo(add2_in2);
    size_t numDest = add1_out.numDestinationPorts();
    REQUIRE(numDest == 2);
    REQUIRE(add1_out.getDestinationPort(0) == add2_in1);
    REQUIRE(add1_out.getDestinationPort(1) == add2_in2);
    REQUIRE(add2_in1.getSourcePort() == add1_out);
    REQUIRE(add2_in2.getSourcePort() == add1_out);

    // Create a nodegraph object
    mx::RtObject graph1Obj = mx::RtNodeGraph::createNew("graph1", stageObj);
    REQUIRE(graph1Obj.isValid());

    // Attach the nodegraph API to the object
    mx::RtNodeGraph graph1(graph1Obj);

    // Create two node instance in the graph
    mx::RtObject add3Obj = mx::RtNode::createNew("add3", addNodeObj, graph1Obj);
    mx::RtObject add4Obj = mx::RtNode::createNew("add4", addNodeObj, graph1Obj);
    REQUIRE(graph1.numNodes() == 2);

    // Create a new nodedef object for defining the graph interface
    mx::RtObject bobNodeObj = mx::RtNodeDef::createNew("ND_bob", "bob", stageObj);
    REQUIRE(bobNodeObj.hasApi(mx::RtApiType::NODEDEF));

    // Attach the nodedef API to this object
    mx::RtNodeDef bobNode(bobNodeObj);

    // Add ports to the nodedef
    mx::RtPortDef::createNew("a", "float", mx::RtValue(0.0f), mx::RtPortFlag::INPUT, bobNodeObj);
    mx::RtPortDef::createNew("b", "float", mx::RtValue(0.0f), mx::RtPortFlag::INPUT, bobNodeObj);
    mx::RtPortDef::createNew("out", "float", mx::RtValue(0.0f), mx::RtPortFlag::OUTPUT, bobNodeObj);
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
    mx::RtNode::connect(inputs.findPort("a"), add3.findPort("in1"));
    mx::RtNode::connect(inputs.findPort("b"), add3.findPort("in2"));
    mx::RtNode::connect(add3.findPort("out"), add4.findPort("in1"));
    mx::RtNode::connect(inputs.findPort("a"), add4.findPort("in2"));
    mx::RtNode::connect(add4.findPort("out"), outputs.findPort("out"));
    REQUIRE(inputs.findPort("a").numDestinationPorts() == 2);
    REQUIRE(inputs.findPort("b").numDestinationPorts() == 1);
    REQUIRE(outputs.findPort("out").getSourcePort() == add4.findPort("out"));

    // Find object by path
    mx::RtObject elem1 = stage.findElementByPath("/add1/in2");
    REQUIRE(elem1.isValid());
    REQUIRE(elem1.hasApi(mx::RtApiType::PORTDEF));
    REQUIRE(mx::RtPortDef(elem1).getName() == "in2");
    REQUIRE(mx::RtPortDef(elem1).isInput());

    // Find a port by path
    mx::RtObject node = stage.findElementByPath("/graph1/add4");
    mx::RtObject portdef = stage.findElementByPath("/graph1/add4/out");
    REQUIRE(node.isValid());
    REQUIRE(portdef.isValid());
    REQUIRE(node.hasApi(mx::RtApiType::NODE));
    REQUIRE(portdef.hasApi(mx::RtApiType::PORTDEF));
    // Get a port instance from node and portdef
    mx::RtPort port1(node, portdef);
    REQUIRE(port1 == add4.findPort("out"));
}

TEST_CASE("Runtime: CoreIo", "[runtime]")
{
    // Create a document.
    mx::DocumentPtr doc = mx::createDocument();

    // Load in stdlib
    mx::FilePath searchPath = mx::FilePath::getCurrentPath() / mx::FilePath("libraries");
    loadLibraries({ "stdlib" }, searchPath, doc);

    // Create a stage and load the document data.
    mx::RtObject stageObj = mx::RtStage::createNew("test");
    mx::RtCoreIo stageIo(stageObj);
    stageIo.read(doc);

    // Get a nodegraph and write a dot file for inspection.
    mx::RtStage stage(stageObj);
    mx::RtNodeGraph graph = stage.findElementByName("NG_tiledimage_float");
    REQUIRE(graph);
    std::ofstream dotfile;
    dotfile.open(graph.getName().str() + ".dot");
    dotfile << graph.asStringDot();
    dotfile.close();

    // Get a nodedef and create two new instances of it.
    mx::RtObject multiplyObj = stage.findElementByName("ND_multiply_color3");
    REQUIRE(multiplyObj);
    mx::RtNode mul1 = mx::RtNode::createNew("mul1", multiplyObj, stageObj);
    mx::RtNode mul2 = mx::RtNode::createNew("mul2", multiplyObj, stageObj);
    REQUIRE(mul1);
    REQUIRE(mul2);
    mul2.findPort("in1").setValue(mx::Color3(0.3f, 0.5f, 0.4f));
    mul2.findPort("in2").setValue(mx::Color3(0.6f, 0.3f, 0.5f));
    mul2.findPort("in2").setColorSpace("srgb_texture");

    // Write the stage to another document.
    mx::DocumentPtr doc2 = mx::createDocument();
    stageIo.write(doc2);
    mx::writeToXmlFile(doc2, stage.getName().str() + ".mtlx");
}

TEST_CASE("Runtime: Stage References", "[runtime]")
{
    // Create a main stage.
    mx::RtObject mainStageObj = mx::RtStage::createNew("main");
    mx::RtStage mainStage(mainStageObj);

    // Load in stdlib in another stage.
    mx::RtObject libStageObj = mx::RtStage::createNew("libs");
    mx::DocumentPtr doc = mx::createDocument();
    mx::FilePath searchPath = mx::FilePath::getCurrentPath() / mx::FilePath("libraries");
    loadLibraries({ "stdlib", "pbrlib" }, searchPath, doc);
    mx::RtCoreIo libStageIo(libStageObj);
    libStageIo.read(doc);

    // Reference the library stage.
    mainStage.addReference(libStageObj);

    // Test access and usage of contents from the referenced library.
    mx::RtNodeDef nodedef = mainStage.findElementByName("ND_complex_ior");
    REQUIRE(nodedef.isValid());
    mx::RtObject nodeObj = mx::RtNode::createNew("complex1", nodedef.getObject(), mainStage.getObject());
    REQUIRE(nodeObj.isValid());

    // Write the stage to a new document, 
    // writing only the non-referenced content.
    mx::DocumentPtr doc2 = mx::createDocument();
    mx::RtCoreIo mainStageIo(mainStageObj);
    mainStageIo.write(doc2);
    mx::writeToXmlFile(doc2, mainStage.getName().str() + ".mtlx");

    // Make sure removal of the non-referenced node works
    const mx::RtToken nodeName = mx::RtNode(nodeObj).getName();
    mainStage.removeElement(nodeName);
    nodeObj = mainStage.findElementByName(nodeName);
    REQUIRE(!nodeObj.isValid());
}

TEST_CASE("Runtime: Traversal", "[runtime]")
{
    // Create a main stage.
    mx::RtStage mainStage = mx::RtStage::createNew("main");

    // Load stdlib in a seperate stage.
    mx::RtStage stdlibStage = mx::RtStage::createNew("stdlib");
    mx::DocumentPtr doc = mx::createDocument();
    mx::FilePath searchPath = mx::FilePath::getCurrentPath() / mx::FilePath("libraries");
    loadLibraries({ "stdlib", "pbrlib" }, searchPath, doc);
    mx::RtCoreIo coreIO(stdlibStage.getObject());
    coreIO.read(doc);

    // Count elements traversing the full stdlib stage
    size_t nodeCount = 0, nodeDefCount = 0, nodeGraphCount = 0;
    for (auto it = stdlibStage.traverseStage(); !it.isDone(); ++it)
    {
        switch ((*it).getObjType())
        {
        case mx::RtObjType::NODE:
            nodeCount++;
            break;
        case mx::RtObjType::NODEDEF:
            nodeDefCount++;
            break;
        case mx::RtObjType::NODEGRAPH:
            nodeGraphCount++;
            break;
        }
    }
    REQUIRE(nodeCount == doc->getNodes().size());
    REQUIRE(nodeDefCount == doc->getNodeDefs().size());
    REQUIRE(nodeGraphCount == doc->getNodeGraphs().size());

    mainStage.addReference(stdlibStage.getObject());
    
    mx::RtNodeDef nodedef = mainStage.findElementByName("ND_subtract_vector3");
    REQUIRE(nodedef);
    mx::RtObject nodeObj = mx::RtNode::createNew("sub1", nodedef.getObject(), mainStage.getObject());
    REQUIRE(nodeObj);

    // Travers using a filter to return only node objects.
    mx::RtObjectFilter<mx::RtObjType::NODE> nodeFilter;
    nodeCount = 0, nodeDefCount = 0, nodeGraphCount = 0;
    for (auto it = mainStage.traverseStage(nodeFilter); !it.isDone(); ++it)
    {
        switch ((*it).getObjType())
        {
        case mx::RtObjType::NODE:
            nodeCount++;
            break;
        case mx::RtObjType::NODEDEF:
            nodeDefCount++;
            break;
        case mx::RtObjType::NODEGRAPH:
            nodeGraphCount++;
            break;
        }
    }
    REQUIRE(nodeCount == 1);
    REQUIRE(nodeDefCount == 0);
    REQUIRE(nodeGraphCount == 0);

    // Travers using a filter to return only objects supporting the nodegraph API.
    mx::RtApiFilter<mx::RtApiType::NODEGRAPH> apiFilter;
    nodeCount = 0, nodeDefCount = 0, nodeGraphCount = 0;
    for (auto it = mainStage.traverseStage(apiFilter); !it.isDone(); ++it)
    {
        switch ((*it).getObjType())
        {
        case mx::RtObjType::NODE:
            nodeCount++;
            break;
        case mx::RtObjType::NODEDEF:
            nodeDefCount++;
            break;
        case mx::RtObjType::NODEGRAPH:
            nodeGraphCount++;
            break;
        }
    }
    REQUIRE(nodeCount == 0);
    REQUIRE(nodeDefCount == 0);
    REQUIRE(nodeGraphCount == 65);

    // Travers a nodegraph using tree traversal.
    mx::RtNodeGraph nodegraph = mainStage.findElementByName("NG_tiledimage_float");
    REQUIRE(nodegraph);
    nodeCount = 0;
    for (auto it = nodegraph.traverseTree(); !it.isDone(); ++it)
    {
        switch ((*it).getObjType())
        {
        case mx::RtObjType::NODE:
            nodeCount++;
            break;
        }
    }
    REQUIRE(nodeCount == 3);

    // Filter for finding input ports.
    auto inputFilter = [](const mx::RtObject& obj) -> bool
    {
        if (obj.hasApi(mx::RtApiType::PORTDEF))
        {
            mx::RtPortDef portdef(obj);
            return portdef.isInput();
        }
        return false;
    };

    // Travers a nodedef finding all its inputs.
    mx::RtNodeDef generalized_schlick_brdf = mainStage.findElementByName("ND_generalized_schlick_brdf");
    REQUIRE(generalized_schlick_brdf);
    size_t inputCount = 0;
    for (auto it = generalized_schlick_brdf.traverseTree(inputFilter); !it.isDone(); ++it)
    {
        inputCount++;
    }
    REQUIRE(inputCount == 9);

    // Filter for finding nodedefs of BSDF nodes.
    auto bsdfFilter = [](const mx::RtObject& obj) -> bool
    {
        if (obj.hasApi(mx::RtApiType::NODEDEF))
        {
            mx::RtNodeDef nodedef(obj);
            return nodedef.numOutputs() == 1 && mx::RtPortDef(nodedef.getPort(0)).getType() == mx::RtType::BSDF;
        }
        return false;
    };

    // Travers to find all nodedefs for BSDF nodes.
    size_t bsdfCount = 0;
    for (auto it = mainStage.traverseStage(bsdfFilter); !it.isDone(); ++it)
    {
        bsdfCount++;
    }
    REQUIRE(bsdfCount == 14);

    // Find the output port on the nodegraph above,
    // and test traversing the graph upstream from 
    // this output.
    mx::RtNode outSockets = nodegraph.getOutputsNode();
    REQUIRE(outSockets.numPorts() == 1);
    mx::RtPort outSocket = outSockets.getPort(0);
    REQUIRE(outSocket.isInput());
    REQUIRE(outSocket.isConnected());
    REQUIRE(outSocket.getSourcePort());
    size_t numEdges = 0;
    for (auto it = outSocket.traverseUpstream(); !it.isDone(); ++it)
    {
        ++numEdges;
    }
    REQUIRE(numEdges == 12);
}
