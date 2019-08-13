#ifndef MATERIALX_MAYA_OGSFRAGMENT_H
#define MATERIALX_MAYA_OGSFRAGMENT_H

/// @file
/// OGS fragment wrapper.

#include <MaterialXCore/Document.h>
#include <MaterialXGenShader/Shader.h>
#include <MaterialXRender/ImageHandler.h>

namespace mx = MaterialX;

namespace MaterialXMaya
{

/// @class OgsFragment
/// Wraps an OGS fragment generated for a specific MaterialX element.
/// The generated source is in an XML format specifying the fragment's inputs
/// and outputs and embedding source code in one or potentially multiple target
/// shading languages (GLSL is the only such language currently supported).
///
class OgsFragment
{
  public:
    OgsFragment(mx::DocumentPtr document,
                mx::ElementPtr element,
                const mx::FileSearchPath& librarySearchPath);

    OgsFragment(const OgsFragment&) = delete;
    OgsFragment(OgsFragment&&) = delete;

    ~OgsFragment();

    /// Return the path to the renderable MaterialX element within the document
    /// that this fragment was created for.
    std::string getElementPath() const
    {
        return _element ? _element->getNamePath() : mx::EMPTY_STRING;
    }

    OgsFragment& operator=(const OgsFragment&) = delete;
    OgsFragment& operator=(OgsFragment&&) = delete;

    /// Return the MaterialX document.
    mx::DocumentPtr getDocument() const
    {
        return _document;
    }

    /// Return the source of the OGS fragment as a string.
    const std::string& getFragmentSource() const;

    /// Return the name of shader fragment. The name is automatically generated
    /// from the name of the MaterialX element and a hash of the generated
    /// fragment source. Can be used to register the fragment in VP2.
    const std::string& getFragmentName() const;

    /// Maps XML element paths of MaterialX inputs to their names in the generated shader.
    const mx::StringMap& getPathInputMap() const;

    /// Return whether the element to render represents a surface shader graph
    /// as opposed to a texture graph.
    bool elementIsAShader() const;

    /// Get image sampling properties for a given file parameter.
    mx::ImageSamplingProperties getImageSamplngProperties(const std::string& fileParameterName) const;

    /// Return whether the fragment represents a transparent surface, as
    /// determined by MaterialX at generation time.
    bool isTransparent() const { return _isTransparent; }

    /// Derive a matrix4 parameter name from a matrix3 parameter name.
    /// Required because OGS doesn't support matrix3 parameters.
    static std::string getMatrix4Name(const std::string& matrix3Name);

  private:
    /// Generate the fragment.
    void generateFragment(const mx::FileSearchPath& librarySearchPath);
    
    mx::DocumentPtr _document;  ///< The MaterialX document.
    mx::ElementPtr _element;    ///< The MaterialX element.
    
    std::string _fragmentName;  ///< An automatically generated fragment name.

    std::string _fragmentSource;    ///< The ganerated fragment source.
    
    mx::StringMap _pathInputMap; ///< Maps MaterialX element paths to fragment input names.

    mx::ShaderPtr _shader;      ///< The MaterialX shader.

    bool _isTransparent = false;    ///< Whether the fragment represents a transparent surface.
};

} // namespace MaterialXMaya

#endif // MATERIALX_DATA_H
