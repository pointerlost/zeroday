#pragma once
#include "glad/glad.h"
#include <string>

namespace Zeroday {
	struct Texture {
		GLuint m_Id = 0;
		uint64_t m_BindlessHandle = 0;
		bool m_Resident = false;
		int width = 0, height = 0;
		std::string m_Name;

		void Create(int w, int h, GLenum internalFormat);
		void UploadData(GLenum format, GLenum type, const void* data);
		void GenerateMipmaps();
		// Generate bindless handle
		bool MakeResident();
		void MakeNonResident();
		[[nodiscard]] uint64_t GetBindlessHandle() const;
		[[nodiscard]] bool IsResident() const { return m_Resident; }

		Texture() = default;
		~Texture() {
			MakeNonResident();
			if (m_Id) glDeleteTextures(1, &m_Id);
		}
	};

}
