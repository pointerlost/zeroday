//
// Created by pointerlost on 8/24/25.
//
#pragma once
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "ECS/Entity.h"

namespace ECS::REGISTRY {

    template<typename T>
    class FreeList {
    public:
        uint32_t allocate(const T& value) {
            uint32_t idx;
            if (!freeIndices.empty()) {
                idx = freeIndices.back();
                freeIndices.pop_back();
                slots[idx] = value;
            } else {
                idx = static_cast<uint32_t>(slots.size());
                slots.push_back(value);
            }
            return idx;
        }

        void release(uint32_t idx) {
            freeIndices.push_back(idx);
        }

        T* get(uint32_t idx) {
            if (idx >= slots.size()) return nullptr;
            return &slots[idx];
        }

        const T* get(uint32_t idx) const {
            if (idx >= slots.size()) return nullptr;
            return &slots[idx];
        }

        [[nodiscard]] size_t GetActiveCount() const { return slots.size() - freeIndices.size(); }
        [[nodiscard]] size_t GetCapacity() const    { return slots.size(); }

    private:
        std::vector<T> slots;
        std::vector<uint32_t> freeIndices;
    };

    // ---------------------
    // Dirty Tracker
    // ---------------------
    struct DirtyTracker {
        std::vector<uint32_t> dirtyStatic;
        std::unordered_map<uint32_t, size_t> dirtyStaticIndexHash;

        std::vector<uint32_t> dirtyDynamic;
        std::unordered_map<uint32_t, size_t> dirtyDynamicIndexHash;

        void markDirty(uint32_t idx, EntityMobility mobility) {
            auto mark = [](std::vector<uint32_t>& vec,
                           std::unordered_map<uint32_t, size_t>& mp,
                           uint32_t idx){
                if (!mp.contains(idx)) {
                    mp[idx] = vec.size();
                    vec.push_back(idx);
                }
            };

            switch (mobility) {
                case EntityMobility::Static:     mark(dirtyStatic,     dirtyStaticIndexHash,     idx); break;
                case EntityMobility::Dynamic:    mark(dirtyDynamic,    dirtyDynamicIndexHash,    idx); break;
            }
        }

        void clearFromAll(uint32_t idx) {
            auto swapPop = [](std::vector<uint32_t>& vec,
                              std::unordered_map<uint32_t, size_t>& mp,
                              uint32_t idx){
                auto it = mp.find(idx);
                if (it == mp.end()) return;
                size_t pos = it->second;
                uint32_t last = vec.back();
                vec[pos] = last;
                mp[last] = pos;
                vec.pop_back();
                mp.erase(it);
            };

            swapPop(dirtyStatic,     dirtyStaticIndexHash,     idx);
            swapPop(dirtyDynamic,    dirtyDynamicIndexHash,    idx);
        }

        void clear(EntityMobility mobility) {
            switch (mobility) {
                case EntityMobility::Static:     dirtyStatic.clear();     dirtyStaticIndexHash.clear();     break;
                case EntityMobility::Dynamic:    dirtyDynamic.clear();    dirtyDynamicIndexHash.clear();    break;
            }
        }

        void clearAll() {
            clear(EntityMobility::Static);
            clear(EntityMobility::Dynamic);
        }

        const std::vector<uint32_t>& getDirtyVec(EntityMobility mobility) const {
            switch (mobility) {
                case EntityMobility::Static:     return dirtyStatic;
                case EntityMobility::Dynamic:    return dirtyDynamic;
            }
            return dirtyDynamic;
        }

        std::vector<uint32_t>& getDirtyVec(EntityMobility mobility) {
            switch (mobility) {
                case EntityMobility::Static:     return dirtyStatic;
                case EntityMobility::Dynamic:    return dirtyDynamic;
            }
            return dirtyDynamic;
        }
    };

}
