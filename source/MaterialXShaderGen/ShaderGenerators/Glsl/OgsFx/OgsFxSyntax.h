#ifndef MATERIALX_OGSFXSYNTAX_H
#define MATERIALX_OGSFXSYNTAX_H

#include <MaterialXShaderGen/ShaderGenerators/Glsl/GlslSyntax.h>

namespace MaterialX
{

/// Syntax class for OgsFx
class OgsFxSyntax : public GlslSyntax
{
public:
    OgsFxSyntax();

    static SyntaxPtr creator() { return std::make_shared<OgsFxSyntax>(); }
};

} // namespace MaterialX

#endif
