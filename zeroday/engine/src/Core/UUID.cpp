//
// Created by pointerlost on 9/21/25.
//
#include "Core/UUID.h"
#include <random>


namespace Zeroday {

    UUID::UUID() {
        GenerateV4();
    }

    UUID::UUID(uint64_t uuid) : m_UUID(uuid) {}

    void UUID::GenerateV4() {
        std::random_device rd;
        std::mt19937_64 engine(rd());
        std::uniform_int_distribution<uint64_t> dist;

        m_UUID = dist(engine);

        // Set version 4 bits (random UUID)
        m_UUID = (m_UUID & 0xFFFFFFFFFFFF0FFFULL) | 0x0000000000004000ULL;
    }
}
