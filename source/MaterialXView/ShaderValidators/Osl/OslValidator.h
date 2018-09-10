#ifndef MATERIALX_OslValidator_H
#define MATERIALX_OslValidator_H

#include <MaterialXView/ShaderValidators/ShaderValidator.h>
#include <MaterialXView/ShaderValidators/ExceptionShaderValidationError.h>
#include <MaterialXView/Handlers/ImageHandler.h>
#include <vector>
#include <string>
#include <map>

namespace MaterialX
{
// Shared pointer to an OslValidator
using OslValidatorPtr = std::shared_ptr<class OslValidator>;

/// @class @OslValidator
/// Helper class to perform validation of OSL source code generated by an OSL code generator.
///
/// The main services provided are:
///     - Validation: Via oslc 
///     - Introspection: None at this time.
///     - Binding: None at this time.
///     - Rendering: None at this time.
///
class OslValidator : public ShaderValidator
{
  public:
    /// Create an OSL validator instance
    static OslValidatorPtr create();

    /// Destructor
    virtual ~OslValidator();

    /// @name Setup
    /// @{

    /// Internal initialization required for program validation and rendering.
    /// An exception is thrown on failure.
    /// The exception will contain a list of initialization errors.
    void initialize() override;

    /// @}
    /// @name Validation
    /// @{

    /// Validate creation of an OSL program based on an input shader
    /// @param shader Input shader
    void validateCreation(const ShaderPtr shader) override;

    /// Validate creation of an OSL program based upon a shader string for a given
    /// shader "stage". There is only one shader stage for OSL thus
    /// @param stages List of shader strings. Only first string in list is examined.
    void validateCreation(const std::vector<std::string>& stages) override;

    /// Validate inputs for the compiled OSL program. 
    /// Note: Currently no validation has been implemented.
    void validateInputs() override;

    /// Validate that an appropriate rendered result is produced.
    /// Note: Currently no validation has been implemented.
    /// @param orthographicView Render orthographically
    void validateRender(bool orthographicView=true) override;

    /// @}
    /// @name Utilities
    /// @{

    /// Save the current contents a rendering to disk.
    /// @param fileName Name of file to save rendered image to.
    void save(const std::string& fileName) override;
    
    /// @}
    /// @name Compilation settings
    /// @{

    /// Set the OSL executable path string. Note that it is assumed that this
    /// references the location of the oslc executable.
    /// @param executable Full path to OSL compiler executable
    void setOslCompilerExecutable(const std::string executable)
    {
        _oslCompilerExecutable = executable;
    }

    /// Set the OSL include path string. 
    /// @param includePathString Include path(s) for the OSL compiler. This should include the
    /// path to stdosl.h    
    void setOslIncludePath(const std::string includePathString)
    {
        _oslIncludePathString = includePathString;
    }

    /// @}

  protected:
    ///
    /// Compile OSL code stored in a file. Return errors in result string 
    /// @param oslFileName Name of OSL file
    void compileOSL(const std::string& oslFileName);

    /// Constructor
    OslValidator();

  private:
    std::string _oslCompilerExecutable;
    std::string _oslIncludePathString;
};

} // namespace MaterialX
#endif