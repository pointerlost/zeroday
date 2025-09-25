#ifndef COMMAND_GENERATION_GLSL
#define COMMAND_GENERATION_GLSL

// Metadata table (per entity)
struct RenderCommandMDI {
    uint materialIndex;
    uint transformIndex;
    uint indexCount;
    uint indexOffset;
};

struct DrawElementsIndirectCommand {
    uint count;
    uint instanceCount;
    uint firstIndex;
    uint baseVertex;
    uint baseInstance;
};

struct DrawPayloadGPU {
    int transformIndex;
    int materialIndex;
    int meshId;
    int entityId;
};

layout(std430, binding = 0) buffer EntityDataBuffer {
    RenderCommandMDI entities[];
};

// Indirect draw commands (one per entity)
layout(std430, binding = 1) buffer IndirectCommands {
    DrawElementsIndirectCommand commands[];
};

// Draw payloads (per draw call data)
layout(std430, binding = 2) buffer DrawPayloads {
    DrawPayloadGPU payloads[];
};

#endif