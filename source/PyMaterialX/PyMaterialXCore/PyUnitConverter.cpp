//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <PyMaterialX/PyMaterialX.h>

#include <MaterialXCore/UnitConverter.h>

namespace py = pybind11;
namespace mx = MaterialX;

class PyUnitConverter : public mx::UnitConverter 
{
  public:
      explicit PyUnitConverter() :
        mx::UnitConverter()
    {
    }

    float convert(float input, const std::string& inputUnit, const std::string& outputUnit) const override
    {
        PYBIND11_OVERLOAD_PURE(
            float, 
            mx::UnitConverter,
            convert,
            input,
            inputUnit,
            outputUnit
        );
    }

    mx::Vector2 convert(mx::Vector2 input, const std::string& inputUnit, const std::string& outputUnit) const override
    {
        PYBIND11_OVERLOAD_PURE(
            mx::Vector2,
            mx::UnitConverter,
            convert,
            input,
            inputUnit,
            outputUnit
        );
    }

    mx::Vector3 convert(mx::Vector3 input, const std::string& inputUnit, const std::string& outputUnit) const override
    {
        PYBIND11_OVERLOAD_PURE(
            mx::Vector3,
            mx::UnitConverter,
            convert,
            input,
            inputUnit,
            outputUnit
        );
    }

    mx::Vector4 convert(mx::Vector4 input, const std::string& inputUnit, const std::string& outputUnit) const override
    {
        PYBIND11_OVERLOAD_PURE(
            mx::Vector4,
            mx::UnitConverter,
            convert,
            input,
            inputUnit,
            outputUnit
        );
    }
};

PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);

void bindPyUnitConverters(py::module& mod)
{    
    py::class_<mx::UnitConverter, PyUnitConverter, mx::UnitConverterPtr>(mod, "UnitConverter")
        .def("convert", (float       (mx::UnitConverter::*)(float      , const std::string&, const std::string&)const) &mx::UnitConverter::convert)
        .def("convert", (mx::Vector2 (mx::UnitConverter::*)(mx::Vector2, const std::string&, const std::string&)const) &mx::UnitConverter::convert)
        .def("convert", (mx::Vector3 (mx::UnitConverter::*)(mx::Vector3, const std::string&, const std::string&)const) &mx::UnitConverter::convert)
        .def("convert", (mx::Vector4 (mx::UnitConverter::*)(mx::Vector4, const std::string&, const std::string&)const) &mx::UnitConverter::convert)
        .def("getUnitAsInteger", &mx::UnitConverter::getUnitAsInteger)
        .def("getUnitFromInteger", &mx::UnitConverter::getUnitFromInteger);

    py::class_<mx::DistanceUnitConverter, mx::UnitConverter, mx::DistanceUnitConverterPtr>(mod, "DistanceUnitConverter")
        .def_static("create", &mx::DistanceUnitConverter::create)
        .def("getUnitScale", &mx::DistanceUnitConverter::getUnitScale)
        .def("getDefaultUnit", &mx::DistanceUnitConverter::getDefaultUnit)
        .def("convert", (float       (mx::DistanceUnitConverter::*)(float, const std::string&, const std::string&)const) &mx::DistanceUnitConverter::convert)
        .def("convert", (mx::Vector2 (mx::DistanceUnitConverter::*)(mx::Vector2, const std::string&, const std::string&)const) &mx::DistanceUnitConverter::convert)
        .def("convert", (mx::Vector3 (mx::DistanceUnitConverter::*)(mx::Vector3, const std::string&, const std::string&)const) &mx::DistanceUnitConverter::convert)
        .def("convert", (mx::Vector4 (mx::DistanceUnitConverter::*)(mx::Vector4, const std::string&, const std::string&)const) &mx::DistanceUnitConverter::convert)
        .def("getUnitAsInteger", &mx::DistanceUnitConverter::getUnitAsInteger)
        .def("getUnitFromInteger", &mx::DistanceUnitConverter::getUnitFromInteger);

    py::class_<mx::UnitConverterRegistry, std::unique_ptr<mx::UnitConverterRegistry, py::nodelete>>(mod, "UnitConverterRegistry")
        .def_static("create", &mx::UnitConverterRegistry::create)
        .def("addUnitConverter", &mx::UnitConverterRegistry::addUnitConverter)
        .def("removeUnitConverter", &mx::UnitConverterRegistry::removeUnitConverter)
        .def("getUnitConverter", &mx::UnitConverterRegistry::getUnitConverter)
        .def("clearUnitConverters", &mx::UnitConverterRegistry::clearUnitConverters);
}