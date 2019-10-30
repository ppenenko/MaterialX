﻿//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXTest/Catch/catch.hpp>

#include <MaterialXCore/Document.h>
#include <MaterialXCore/UnitConverter.h>
#include <MaterialXFormat/File.h>
#include <MaterialXFormat/XmlIo.h>
#include <MaterialXGenShader/TypeDesc.h>
#include <MaterialXGenShader/Util.h>

#include <cmath>

namespace mx = MaterialX;

const float EPSILON = 1e-4f;

TEST_CASE("UnitAttribute", "[units]")
{
    mx::DocumentPtr doc = mx::createDocument();
    mx::loadLibrary(mx::FilePath::getCurrentPath() / mx::FilePath("libraries/stdlib/stdlib_defs.mtlx"), doc);
    std::vector<mx::UnitTypeDefPtr> unitTypeDefs = doc->getUnitTypeDefs();
    REQUIRE(!unitTypeDefs.empty());

    mx::NodeGraphPtr nodeGraph = doc->addNodeGraph();
    nodeGraph->setName("graph1");

    // Basic get/set unit testing
    mx::NodePtr constant = nodeGraph->addNode("constant");
    constant->setName("constant1");
    constant->setParameterValue("value", mx::Color3(0.5f));
    mx::ParameterPtr param = constant->getParameter("value");
    param->setName("param1");
    param->setUnitType("distance");
    param->setUnit("meter");
    REQUIRE(param->hasUnit());
    REQUIRE(!param->getUnit().empty());

    // Test for valid unit names
    mx::OutputPtr output = nodeGraph->addOutput();
    output->setConnectedNode(constant);
    output->setUnitType("distance");
    output->setUnit("bad unit");
    REQUIRE(!output->validate());
    output->setUnit("foot");
    REQUIRE(output->hasUnit());
    REQUIRE(!output->getUnit().empty());

    REQUIRE(doc->validate());
}

TEST_CASE("UnitEvaluation", "[units]")
{
    mx::DocumentPtr doc = mx::createDocument();
    mx::loadLibrary(mx::FilePath::getCurrentPath() / mx::FilePath("libraries/stdlib/stdlib_defs.mtlx"), doc);

    mx::UnitTypeDefPtr distanceTypeDef = doc->getUnitTypeDef(mx::DistanceUnitConverter::DISTANCE_UNIT);
    REQUIRE(distanceTypeDef);

    mx::UnitConverterRegistryPtr registry = mx::UnitConverterRegistry::create();
    mx::UnitConverterRegistryPtr registry2 = mx::UnitConverterRegistry::create();
    REQUIRE(registry == registry2);

    mx::DistanceUnitConverterPtr converter = mx::DistanceUnitConverter::create(distanceTypeDef);
    REQUIRE(converter);
    registry->addUnitConverter(distanceTypeDef, converter);
    mx::UnitConverterPtr uconverter = registry->getUnitConverter(distanceTypeDef);
    REQUIRE(uconverter);

    // Use converter to convert
    float result = converter->convert(0.1f, "kilometer", "millimeter");
    REQUIRE((result - 100000.0f) < EPSILON);
    result = converter->convert(2.3f, "meter", "meter");
    REQUIRE((result - 2.3f) < EPSILON);
    result = converter->convert(1.0f, "mile", "meter");
    REQUIRE((result - 1609.344f) < EPSILON);
    result = converter->convert(1.0f, "meter", "mile");
    REQUIRE((result - (1.0 / 0.000621f)) < EPSILON);

    // Use explicit converter values
    const std::unordered_map<std::string, float>& unitScale = converter->getUnitScale();
    result = 0.1f * unitScale.find("kilometer")->second / unitScale.find("millimeter")->second;
    REQUIRE((result - 100000.0f) < EPSILON);
    const std::string& defaultUnit = converter->getDefaultUnit();
    REQUIRE(defaultUnit == distanceTypeDef->getDefault());

    // Test integrer mapping
    unsigned int unitNumber = converter->getUnitAsInteger("mile");
    const std::string& unitName = converter->getUnitFromInteger(unitNumber);
    REQUIRE(unitName == "mile");
}

TEST_CASE("UnitDocument", "[units]")
{
    mx::FilePath libraryPath("libraries/stdlib");
    mx::FilePath examplesPath("resources/Materials/Examples/Units");
    std::string searchPath = libraryPath.asString() +
        mx::PATH_LIST_SEPARATOR +
        examplesPath.asString();

    // Read and validate each example document.
    for (std::string filename : examplesPath.getFilesInDirectory(mx::MTLX_EXTENSION))
    {
        mx::DocumentPtr doc = mx::createDocument();
        mx::readFromXmlFile(doc, filename, searchPath);
        mx::loadLibrary(mx::FilePath::getCurrentPath() / mx::FilePath("libraries/stdlib/stdlib_defs.mtlx"), doc);

        mx::UnitTypeDefPtr distanceTypeDef = doc->getUnitTypeDef(mx::DistanceUnitConverter::DISTANCE_UNIT);
        REQUIRE(distanceTypeDef);

        mx::UnitConverterPtr uconverter = mx::DistanceUnitConverter::create(distanceTypeDef);
        REQUIRE(uconverter);
        mx::UnitConverterRegistryPtr registry = mx::UnitConverterRegistry::create();
        registry->addUnitConverter(distanceTypeDef, uconverter);
        uconverter = registry->getUnitConverter(distanceTypeDef);
        REQUIRE(uconverter);

        // Traverse the document tree
        for (mx::ElementPtr elem : doc->traverseTree())
        {
            // If we have nodes with inputs
            mx::NodePtr pNode = elem->asA<mx::Node>();
            if (pNode) 
            {
                if (pNode->getInputCount()) {
                    for (mx::InputPtr input : pNode->getInputs()) {
                        const mx::TypeDesc* type = mx::TypeDesc::get(input->getType());
                        const mx::ValuePtr value = input->getValue();
                        if (input->hasUnit() && value) {

                            if (type->isScalar())
                            {
                                float originalval = value->asA<float>();
                                float convertedValue = uconverter->convert(originalval, input->getUnit(), distanceTypeDef->getDefault());
                                float reconvert = uconverter->convert(convertedValue, distanceTypeDef->getDefault(), input->getUnit());
                                REQUIRE((originalval - reconvert) < EPSILON);
                            }
                            else if (type->isFloat2())
                            {
                                mx::Vector2 originalval = value->asA<mx::Vector2>();
                                mx::Vector2 convertedValue = uconverter->convert(originalval, input->getUnit(), distanceTypeDef->getDefault());
                                mx::Vector2 reconvert = uconverter->convert(convertedValue, distanceTypeDef->getDefault(), input->getUnit());
                                REQUIRE(originalval == reconvert);
                            }
                            else if (type->isFloat3())
                            {
                                mx::Vector3 originalval = value->asA<mx::Vector3>();
                                mx::Vector3 convertedValue = uconverter->convert(originalval, input->getUnit(), distanceTypeDef->getDefault());
                                mx::Vector3 reconvert = uconverter->convert(convertedValue, distanceTypeDef->getDefault(), input->getUnit());
                                REQUIRE(originalval == reconvert);
                            }
                            else if (type->isFloat4())
                            {
                                mx::Vector4 originalval = value->asA<mx::Vector4>();
                                mx::Vector4 convertedValue = uconverter->convert(originalval, input->getUnit(), distanceTypeDef->getDefault());
                                mx::Vector4 reconvert = uconverter->convert(convertedValue, distanceTypeDef->getDefault(), input->getUnit());
                                REQUIRE(originalval == reconvert);
                            }
                        }
                    }
                }

                if (pNode->getParameterCount()) {
                    for (mx::ParameterPtr param: pNode->getParameters()) {
                        const mx::TypeDesc* type = mx::TypeDesc::get(param->getType());
                        const mx::ValuePtr value = param->getValue();
                        if (param->hasUnit() && value) {

                            if (type->isScalar())
                            {
                                float originalval = value->asA<float>();
                                float convertedValue = uconverter->convert(originalval, param->getUnit(), distanceTypeDef->getDefault());
                                float reconvert = uconverter->convert(convertedValue, distanceTypeDef->getDefault(), param->getUnit());
                                REQUIRE((originalval - reconvert) < EPSILON);
                            }
                            else if (type->isFloat2())
                            {
                                mx::Vector2 originalval = value->asA<mx::Vector2>();
                                mx::Vector2 convertedValue = uconverter->convert(originalval, param->getUnit(), distanceTypeDef->getDefault());
                                mx::Vector2 reconvert = uconverter->convert(convertedValue, distanceTypeDef->getDefault(), param->getUnit());
                                REQUIRE(originalval == reconvert);
                            }
                            else if (type->isFloat3())
                            {
                                mx::Vector3 originalval = value->asA<mx::Vector3>();
                                mx::Vector3 convertedValue = uconverter->convert(originalval, param->getUnit(), distanceTypeDef->getDefault());
                                mx::Vector3 reconvert = uconverter->convert(convertedValue, distanceTypeDef->getDefault(), param->getUnit());
                                REQUIRE(originalval == reconvert);
                            }
                            else if (type->isFloat4())
                            {
                                mx::Vector4 originalval = value->asA<mx::Vector4>();
                                mx::Vector4 convertedValue = uconverter->convert(originalval, param->getUnit(), distanceTypeDef->getDefault());
                                mx::Vector4 reconvert = uconverter->convert(convertedValue, distanceTypeDef->getDefault(), param->getUnit());
                                REQUIRE(originalval == reconvert);
                            }
                        }
                    }
                }

            }
        }
    }
}