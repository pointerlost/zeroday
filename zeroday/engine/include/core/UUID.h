//
// Created by pointerlost on 9/21/25.
//
#pragma once
#include <cstdint>
#include <functional>


namespace Zeroday {

    class UUID {
    public:
        UUID();
        explicit UUID(uint64_t uuid);
        UUID(const UUID&) = default;
        UUID& operator=(const UUID&) = default;

        operator uint64_t() const noexcept { return m_UUID; }

    private:
        uint64_t m_UUID;
    };
}

namespace std {
    template<>
    struct hash<Zeroday::UUID> {
        std::size_t operator()(const Zeroday::UUID& uuid) const noexcept {
            return std::hash<uint64_t>{}(static_cast<uint64_t>(uuid));
        }
    };
}