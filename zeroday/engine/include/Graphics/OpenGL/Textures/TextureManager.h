//
// Created by pointerlost on 8/15/25.
//
#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <glad/glad.h>
#include "core/Base.h"

namespace Zeroday {
    struct Texture;
    namespace opengl {
        enum class MaterialTextureType;
    }
}

namespace Zeroday {

    class TextureManager {
    public:
        TextureManager();
        ~TextureManager();

        [[nodiscard]] Ref<Texture> Load(const std::string& name, const std::string& path);
        Ref<Texture> GetDefaultTexture(opengl::MaterialTextureType type);

        Ref<Texture> GetTextureWithName(const std::string& name);
        Ref<Texture> GetTextureWithPath(const std::string& path);

        // Bindless helpers (exposed but manager handles residency for you)
        // Returns the bindless handle (0 on failure)
        uint64_t EnsureBindlessHandle(Ref<Texture> tex);
        uint64_t GetBindlessHandle(Ref<Texture> tex) const;

        // Release a single texture (makes handle non-resident and deletes GL texture)
        // Must be called on GL context thread
        void ReleaseTexture(const Ref<Texture>& tex);
        // Release all GPU resources
        void ReleaseAll();

        void LoadFromFolder(const std::string& folderName, const std::string& folderPath);

    private:
        std::unordered_map<std::string, Ref<Texture>> m_NameMap; // name -> texture (for UI stuff)
        std::unordered_map<std::string, Ref<Texture>> m_PathMap; // file path -> texture
        std::vector<Ref<Texture>> m_AllTextures;

        std::unordered_map<opengl::MaterialTextureType, Ref<Texture>> m_DefaultTextures;

        // track resident handles for safety (registered in ensureBindlessHandle)
        // Use this list to make the handle on the GPU non-resident
        // when the texture is being erased or releaseAll() is called.
        std::unordered_set<uint64_t> m_ResidentHandles;

        Ref<Texture> CreateDefaultTexture(const std::string& debugName,
                                        unsigned char r, unsigned char g,
                                        unsigned char b, unsigned char a = 255);
    };
}