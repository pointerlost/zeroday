//
// Created by pointerlost on 8/7/25.
//
#include "Math/Math.h"
#include <cmath>

namespace MATH {

    glm::vec3 wrapAngles(glm::vec3 angles) {
        glm::vec3 wrapped;
        for (int i = 0; i < 3; i++) {
            wrapped[i] = std::fmod(angles[i], 360.f);
            if (wrapped[i] < 0.0f)
                wrapped[i] += 360.f;
        }
        return wrapped;
    }
}
