//
// Created by pointerlost on 9/21/25.
//
#pragma once
#include <cstdint>
#include <iostream>
#include <functional>

namespace Zeroday {

    class UUID {
    public:
        UUID();
        explicit UUID(uint64_t uuid);
        UUID(const UUID&) = default;

        operator uint64_t() const noexcept { return m_UUID; }

        [[nodiscard]] std::string ToString() const {
            return std::to_string(m_UUID);
        }

        bool operator==(const UUID& other) const {
            return m_UUID == other.m_UUID;
        }

        bool operator!=(const UUID& other) const {
            return m_UUID != other.m_UUID;
        }

    private:
        void GenerateV4();
        uint64_t m_UUID{};
    };
}

namespace std {
    template<>
    struct hash<Zeroday::UUID> {
        std::size_t operator()(const Zeroday::UUID& uuid) const noexcept {
            return std::hash<uint64_t>{}(uuid);
        }
    };
}