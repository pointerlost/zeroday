#version 460
layout(local_size_x = 64) in;

#include "common/constants.glsl"

// Metadata table (per entity)
layout(std430, binding = BINDING_ENTITY_METADATA) buffer EntityMetadata {
    uint entityVAO[];
    uint entityMaterialId[];
    uint entityTransformId[];
    uint entityIndexCount[];
    uint entityIndexOffset[];
};

// Visible entity list (from culling)
layout(std430, binding = BINDING_VISIBLE_ENTITIES) buffer VisibleEntities {
    uint visibleEntityIndices[];
    uint visibleEntityCount; // Total visible entities
};

// Output: Indirect draw commands (one per visible entity)
layout(std430, binding = BINDING_COMMANDS) buffer IndirectCommands {
    DrawElementsIndirectCommand commands[];
};

// Output: Draw payloads (per draw call data)
layout(std430, binding = BINDING_PAYLOADS) buffer DrawPayloads {
    DrawPayloadGPU payloads[];
};

void main() {
    uint threadIndex = gl_GlobalInvocationID.x;

    // Check if this thread processes a visible entity
    if (threadIndex >= visibleEntityCount) return;

    // Get the actual entity index from visibility list
    uint entityIndex = visibleEntityIndices[threadIndex];

    // Each visible entity gets its own draw command
    uint commandIndex = threadIndex;

    // Fill indirect draw command
    commands[commandIndex].count         = entityIndexCount[entityIndex];
    commands[commandIndex].instanceCount = 1;
    commands[commandIndex].firstIndex    = entityIndexOffset[entityIndex];
    commands[commandIndex].baseVertex    = 0;
    commands[commandIndex].baseInstance  = commandIndex; // Points to payload

    // Fill payload data for vertex shader
    payloads[commandIndex].transformIndex = int(entityTransformId[entityIndex]);
    payloads[commandIndex].materialIndex  = int(entityMaterialId[entityIndex]);
    payloads[commandIndex].meshID         = int(entityVAO[entityIndex]);
    payloads[commandIndex].entityID       = int(entityIndex);
}