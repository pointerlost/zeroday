//
// Created by pointerlost on 9/21/25.
//
#include "core/UUID.h"

#include <random>


namespace Zeroday {

    UUID::UUID()
        : m_UUID([]() {
        static std::random_device rd;
        static std::mt19937_64 engine(rd());
        static std::uniform_int_distribution<uint64_t> dist;
        return dist(engine);
        }())
    {
    }

    UUID::UUID(uint64_t uuid)
        : m_UUID(uuid)
    {
    }

}
