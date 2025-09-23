#ifndef CONSTANTS_GLSL
#define CONSTANTS_GLSL

// Phase-based binding points (reuse them)
const int BINDING_PHASE_INPUT      = 0; // For CPU->GPU data
const int BINDING_PHASE_OUTPUT     = 1; // For GPU->CPU data
const int BINDING_PHASE_METADATA   = 2; // For metadata
const int BINDING_PHASE_SCRATCH    = 3; // For temporary data
const int BINDING_ENTITY_METADATA  = 4;
const int BINDING_VISIBLE_ENTITIES = 5;
const int BINDING_COMMANDS         = 6;
const int BINDING_PAYLOADS         = 7;

#define PI 3.14159265359

#define MAX_ENTITIES 65536

#endif