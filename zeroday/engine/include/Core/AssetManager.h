//
// Created by pointerlost on 8/9/25.
//
#pragma once
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Base.h"
#include "Graphics/OpenGL/Shader.h"

namespace Zeroday {
    struct Texture;

    namespace opengl {
        enum class MaterialTextureType;
        struct Material;
        struct MaterialInstance;
    }

}

namespace Zeroday {

    class AssetManager {
    public:
        AssetManager();
        /*********************************** SHADER MANAGEMENT **************************************/
        Shader* GetShader(const std::string& name);
        [[nodiscard]] bool LoadShader(const std::string &name, const std::string &vertPath, const std::string &fragPath);
        [[nodiscard]] bool CompileComputeShader(const std::string& name, const std::string& filePath);
        [[nodiscard]] bool LoadAllShaders();
        std::string PreprocessShader(const std::string &source, const std::string &basePath);

        /*********************************** MATERIAL MANAGEMENT **************************************/
		[[nodiscard]] bool LoadMaterialsFromFolder(const std::string& filePath);
		[[nodiscard]] Ref<opengl::Material> GetMaterialByName(const std::string& name);
		[[nodiscard]] Ref<opengl::MaterialInstance> CreateMaterialInstance(const std::string& name);
		[[nodiscard]] Ref<opengl::Material> GetDefaultMaterial();

        /********************************* TEXTURE MANAGEMENT *********************************/
        void CreateDefaultTextures();
        Ref<Texture> CreateDefaultTexture(const std::string& debugName,
            unsigned char r, unsigned char g,
            unsigned char b, unsigned char a = 255
        );

        Ref<Texture> GetDefaultTexture(opengl::MaterialTextureType type);
        std::string GetTextureTypeName(opengl::MaterialTextureType type);
        Ref<Texture> GetTextureWithName(const std::string& name);
        Ref<Texture> GetTextureWithPath(const std::string& path);
        Ref<Texture> Load(const std::string& name, const std::string &path);

        uint64_t GetBindlessHandle(Ref<Texture> tex) const;
        void ReleaseAllTextureHandles();

    private:
        std::unordered_map<std::string, Shader> g_Shaders;

        std::unordered_map<std::string, Ref<opengl::Material>> m_Materials;
        std::unordered_map<opengl::MaterialTextureType, Ref<Texture>> m_DefaultTextures;
        std::unordered_set<uint64_t> m_ResidentHandles;
        std::unordered_map<std::string, Ref<Texture>> m_NameMap; // name -> texture
        std::unordered_map<std::string, Ref<Texture>> m_PathMap; // file path -> texture
        std::vector<Ref<Texture>> m_AllTextures;

    };
}
