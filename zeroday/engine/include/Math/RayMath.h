//
// Created by pointerlost on 8/4/25.
//
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Math {

    inline glm::vec3 screenToWorldRay(double mouseX, double mouseY, int screenWidth, int screenHeight,
                                          const glm::mat4& view, const glm::mat4& projection)
    {
        float x = (2.0f * mouseX) / screenWidth - 1.0f;
        float y = 1.0f - (2.0f * mouseY) / screenHeight;
        glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);

        glm::vec4 rayEye = glm::inverse(projection) * rayClip;
        rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

        glm::vec4 rayWorld4 = glm::inverse(view) * rayEye;
        glm::vec3 rayWorld = glm::normalize(glm::vec3(rayWorld4));

        return rayWorld;
    }

    inline bool RayIntersectsAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDir,
                                   const glm::vec3& aabbMin, const glm::vec3& aabbMax,
                                   float& tMinOut)
    {
        float tMin = (aabbMin.x - rayOrigin.x) / rayDir.x;
        float tMax = (aabbMax.x - rayOrigin.x) / rayDir.x;
        if (tMin > tMax) std::swap(tMin, tMax);

        float tyMin = (aabbMin.y - rayOrigin.y) / rayDir.y;
        float tyMax = (aabbMax.y - rayOrigin.y) / rayDir.y;
        if (tyMin > tyMax) std::swap(tyMin, tyMax);

        if ((tMin > tyMax) || (tyMin > tMax))
            return false;

        if (tyMin > tMin) tMin = tyMin;
        if (tyMax < tMax) tMax = tyMax;

        float tzMin = (aabbMin.z - rayOrigin.z) / rayDir.z;
        float tzMax = (aabbMax.z - rayOrigin.z) / rayDir.z;
        if (tzMin > tzMax) std::swap(tzMin, tzMax);

        if ((tMin > tzMax) || (tzMin > tMax))
            return false;

        if (tzMin > tMin) tMin = tzMin;
        if (tzMax < tMax) tMax = tzMax;

        tMinOut = tMin;
        return true;
    }
}

