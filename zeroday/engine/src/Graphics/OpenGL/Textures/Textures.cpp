//
// Created by pointerlost on 9/28/25.
//
#include "Graphics/OpenGL/Textures/Textures.h"
#include "Core/Assert.h"

namespace Zeroday {

    void Texture::Create(int w, int h, GLenum internalFormat) {
        width = w;
        height = h;
        glCreateTextures(GL_TEXTURE_2D, 1, &m_Id);
        glTextureStorage2D(m_Id, 1, internalFormat, width, height);

        // filtering methods for PBR textures
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // for normal maps and roughness maps
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_R, GL_REPEAT);
    }

    void Texture::UploadData(GLenum format, GLenum type, const void *data) {
        // fills the empty texture with actual image data
        glTextureSubImage2D(m_Id, 0, 0, 0, width, height, format, type, data);

        // generate mipmaps after uploading data
        GenerateMipmaps();
    }

    void Texture::GenerateMipmaps() {
        glGenerateTextureMipmap(m_Id);
        glTextureParameteri(m_Id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(m_Id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    bool Texture::MakeResident() {
        if (m_Resident) return true;

        if (GLAD_GL_ARB_bindless_texture) {
            m_BindlessHandle = glGetTextureHandleARB(m_Id);
            if (m_BindlessHandle) {
                glMakeTextureHandleResidentARB(m_BindlessHandle);
                m_Resident = true;
                return true;
            }
        }
        return false;
    }

    void Texture::MakeNonResident() {
        if (m_Resident && m_BindlessHandle) {
            glMakeTextureHandleNonResidentARB(m_BindlessHandle);
            m_Resident = false;
            m_BindlessHandle = 0;
        }
    }

    uint64_t Texture::GetBindlessHandle() const {
        if (!m_Resident) {
            ZD_WARN(false, "Texture '{}' is not resident", m_Name);
        }
        return m_BindlessHandle;
    }
}
