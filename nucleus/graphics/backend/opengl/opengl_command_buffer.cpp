/**
 * (c) 2015 Alexandro Sanchez Bach. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#include "opengl_command_buffer.h"
#include "nucleus/graphics/backend/opengl/opengl_target.h"
#include "nucleus/logger/logger.h"
#include "nucleus/assert.h"

namespace gfx {

OpenGLCommandBuffer::OpenGLCommandBuffer() {
}

OpenGLCommandBuffer::~OpenGLCommandBuffer() {
}

bool OpenGLCommandBuffer::reset() {
    commands.clear();
    return true;
}

void OpenGLCommandBuffer::cmdBindPipeline(Pipeline* pipeline) {
    auto glPipeline = static_cast<OpenGLPipeline*>(pipeline);
    if (!glPipeline) {
        logger.error(LOG_GRAPHICS, "OpenGLCommandBuffer::cmdBindPipeline: Invalid pipeline specified");
        return;
    }

    auto* command = new OpenGLCommandBindPipeline();
    command->pipeline = glPipeline;

    commands.push_back(command);
}

void OpenGLCommandBuffer::cmdClearColor(ColorTarget* target, const F32* colorValue) {
    auto glTarget = static_cast<OpenGLColorTarget*>(target);
    if (!glTarget) {
        logger.error(LOG_GRAPHICS, "OpenGLCommandBuffer::cmdClearColor: Invalid target specified");
        return;
    }

    auto* command = new OpenGLCommandClearColor();
    command->target = glTarget;
    command->r = colorValue[0];
    command->g = colorValue[1];
    command->b = colorValue[2];
    command->a = colorValue[3];

    commands.push_back(command);
}

void OpenGLCommandBuffer::cmdClearDepthStencil(DepthStencilTarget* target, F32 depthValue, U8 stencilValue) {
    auto glTarget = static_cast<OpenGLDepthStencilTarget*>(target);
    if (!glTarget) {
        logger.error(LOG_GRAPHICS, "OpenGLCommandBuffer::cmdClearDepthStencil: Invalid target specified");
        return;
    }

    auto* command = new OpenGLCommandClearDepthStencil();
    command->framebuffer = glTarget->framebuffer;
    command->drawbuffer = glTarget->drawbuffer;
    command->depth = depthValue;
    command->stencil = stencilValue;

    commands.push_back(command);
}

void OpenGLCommandBuffer::cmdDraw(U32 firstVertex, U32 vertexCount, U32 firstInstance, U32 instanceCount) {
    assert_false(firstVertex > 0 && firstInstance > 0, "OpenGLCommandBuffer::cmdSetTargets: Unsupported combination of parameters");

    auto* command = new OpenGLCommandDraw();
    command->firstVertex = firstVertex;
    command->vertexCount = vertexCount;
    command->firstInstance = firstInstance;
    command->instanceCount = instanceCount;

    commands.push_back(command);
}

void OpenGLCommandBuffer::cmdDrawIndexed(U32 firstIndex, U32 indexCount, U32 vertexOffset, U32 firstInstance, U32 instanceCount) {
    auto* command = new OpenGLCommandDrawIndexed();
    command->firstIndex = firstIndex;
    command->indexCount = indexCount;
    command->vertexOffset = vertexOffset;
    command->firstInstance = firstInstance;
    command->instanceCount = instanceCount;

    commands.push_back(command);
}

void OpenGLCommandBuffer::cmdSetVertexBuffers(U32 index, VertexBuffer* vtxBuffer) {
    auto* command = new OpenGLCommandSetVertexBuffers();
}

void OpenGLCommandBuffer::cmdSetPrimitiveTopology(PrimitiveTopology topology) {
    GLenum glTopology;
    switch (topology) {
    case TOPOLOGY_POINT_LIST:
        glTopology = GL_POINTS; break;
    case TOPOLOGY_LINE_LIST:
        glTopology = GL_LINES; break;
    case TOPOLOGY_LINE_STRIP:
        glTopology = GL_LINE_LOOP; break;
    case TOPOLOGY_TRIANGLE_LIST:
        glTopology = GL_TRIANGLES; break;
    case TOPOLOGY_TRIANGLE_STRIP:
        glTopology = GL_TRIANGLE_STRIP; break;
    case TOPOLOGY_QUAD_LIST:
        glTopology = GL_QUADS; break;
    case TOPOLOGY_QUAD_STRIP:
        glTopology = GL_QUAD_STRIP; break;
    default:
        logger.error(LOG_GRAPHICS, "Unimplemented primitive topology type");
        return;
    }

    // TODO
}

void OpenGLCommandBuffer::cmdSetTargets(U32 colorCount, ColorTarget** colorTargets, DepthStencilTarget* depthStencilTarget) {
    assert_true(colorCount <= 32, "Unsupported amount of color targets");

    auto* command = new OpenGLCommandSetTargets();

    // Color targets
    command->colorCount = colorCount;
    for (U32 i = 0; i < colorCount; i++) {
        auto* glTarget = static_cast<OpenGLColorTarget*>(colorTargets[i]);
        if (!glTarget) {
            logger.error(LOG_GRAPHICS, "OpenGLCommandBuffer::cmdSetTargets: Invalid color target specified");
            return;
        }
        command->colorTargets[i] = glTarget->drawbuffer;
    }

    // Depth-stencil target
    auto* glTarget = static_cast<OpenGLDepthStencilTarget*>(depthStencilTarget);
    if (glTarget) {
        command->depthStencilTarget = glTarget->drawbuffer;
    } else {
        command->depthStencilTarget = 0;
    }

    commands.push_back(command);
}

void OpenGLCommandBuffer::cmdSetViewports(U32 viewportsCount, const Viewport* viewports) {
    assert_true(viewportsCount == 1, "OpenGLCommandBuffer::cmdSetViewports: Only one viewport is supported");

    auto* command = new OpenGLCommandSetViewports();
    command->x = viewports[0].originX;
    command->y = viewports[0].originY;
    command->width = viewports[0].width;
    command->height = viewports[0].height;

    commands.push_back(command);
}

void OpenGLCommandBuffer::cmdSetScissors(U32 scissorsCount, const Rectangle* scissors) {
    assert_true(scissorsCount == 1, "OpenGLCommandBuffer::cmdSetScissors: Only one scissor rectangle is supported");

    auto* command = new OpenGLCommandSetScissors();
    command->x = scissors[0].left;
    command->y = scissors[0].top;
    command->width = scissors[0].right - scissors[0].left;
    command->height = scissors[0].bottom - scissors[0].top;

    commands.push_back(command);
}

}  // namespace gfx
