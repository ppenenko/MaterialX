#include <MaterialXShaderGen/ShaderGenerators/GlslShaderGenerator.h>
#include <MaterialXShaderGen/ShaderGenerators/GlslSyntax.h>

namespace MaterialX
{

GlslShaderGenerator::GlslShaderGenerator()
    : ShaderGenerator(std::make_shared<GlslSyntax>())
{
}

void GlslShaderGenerator::emitTextureNodes(Shader& shader)
{
    // Emit function calls for all texturing nodes
    for (const SgNode& node : shader.getNodes())
    {
        // Emit only unconditional nodes, since any node within a conditional 
        // branch is emitted by the conditional node itself
        if (node.hasClassification(SgNode::Classification::TEXTURE) && !node.referencedConditionally())
        {
            emitFunctionCall(node, shader);
        }
    }
}

void GlslShaderGenerator::emitSurfaceBsdf(const SgNode& surfaceShaderNode, const string& incident, const string& outgoing, Shader& shader, string& bsdf)
{
    vector<string> lightDirections = { incident, outgoing };
    const SgNode* last = nullptr;

    // Emit function calls for all BSDF nodes used by this shader
    // The last node will hold the final result
    for (const SgNode& node : shader.getNodes())
    {
        if (node.hasClassification(SgNode::Classification::BSDF) && surfaceShaderNode.isUsedClosure(&node))
        {
            emitFunctionCall(node, shader, &lightDirections);
            last = &node;
        }
    }

    if (last)
    {
        bsdf = _syntax->getVariableName(*last->getNodePtr());
    }
}

void GlslShaderGenerator::emitSurfaceEmission(const SgNode& surfaceShaderNode, Shader& shader, string& emission)
{
    emission = "vec3(0.0)";

    const SgNode* last = nullptr;

    // Emit function calls for all EDF nodes used by this shader
    // The last node will hold the final result
    for (const SgNode& node : shader.getNodes())
    {
        if (node.hasClassification(SgNode::Classification::EDF) && surfaceShaderNode.isUsedClosure(&node))
        {
            emitFunctionCall(node, shader);
            last = &node;
        }
    }

    if (last)
    {
        emission = _syntax->getVariableName(*last->getNodePtr());
    }
}

}
