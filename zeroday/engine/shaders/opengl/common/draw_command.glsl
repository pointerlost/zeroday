#ifndef MDI_COMMANDS
#define MDI_COMMANDS

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

#endif