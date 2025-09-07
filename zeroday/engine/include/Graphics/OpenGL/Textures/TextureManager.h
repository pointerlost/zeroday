//
// Created by pointerlost on 8/15/25.
//
#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <glad/glad.h>


namespace Graphics {
    enum class MaterialTextureType;
}

namespace Graphics { struct Texture; }

namespace Graphics {

    class TextureManager {
    public:
        TextureManager();
        ~TextureManager();

        [[nodiscard]] std::shared_ptr<Texture> load(const std::string& name, const std::string& path);
        std::shared_ptr<Texture> GetDefaultTexture(MaterialTextureType type);

        std::shared_ptr<Texture> getTextureWithName(const std::string& name);
        std::shared_ptr<Texture> getTextureWithPath(const std::string& path);

        // Bindless helpers (exposed but manager handles residency for you)
        // Returns the bindless handle (0 on failure)
        uint64_t ensureBindlessHandle(std::shared_ptr<Texture> tex);
        uint64_t getBindlessHandle(std::shared_ptr<Texture> tex) const;

        // Release a single texture (makes handle non-resident and deletes GL texture)
        // Must be called on GL context thread
        void releaseTexture(const std::shared_ptr<Texture>& tex);
        // Release all GPU resources
        void releaseAll();

        void loadFromFolder(const std::string& folderName, const std::string& folderPath);

    private:
        std::unordered_map<std::string, std::shared_ptr<Texture>> m_nameMap; // name -> texture (for UI stuff)
        std::unordered_map<std::string, std::shared_ptr<Texture>> m_pathMap; // file path -> texture
        std::vector<std::shared_ptr<Texture>> m_allTextures;

        std::unordered_map<MaterialTextureType, std::shared_ptr<Texture>> m_defaultTextures;

        // track resident handles for safety (registered in ensureBindlessHandle)
        // Use this list to make the handle on the GPU non-resident
        // when the texture is being erased or releaseAll() is called.
        std::unordered_set<uint64_t> residentHandles;

        std::shared_ptr<Texture> CreateDefaultTexture(const std::string& debugName,
                                                      unsigned char r, unsigned char g,
                                                      unsigned char b, unsigned char a = 255);
    };
}