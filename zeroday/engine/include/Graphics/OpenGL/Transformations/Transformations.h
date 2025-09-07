#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>


class Transform {
public:
	glm::vec3 position{0.0f};
	glm::vec3 eulerAngles{0.0f};
	glm::vec3 scale{1.0f};

	const glm::mat4& getModelMatrix();

	void addPosition(const glm::vec3& pos);
	void addRotation(const glm::vec3& angle);
	void addScale(const glm::vec3& s);

	void setPosition(const glm::vec3& pos);
	void setRotation(const glm::vec3& angle);
	void setScale(const glm::vec3& s);

	glm::vec3& getPosition() { return position;    };
	glm::vec3& getRotation() { return eulerAngles; };
	glm::vec3& getScale()    { return scale;       };

	[[nodiscard]] bool isDirty() const { return m_modelMatrixDirty; };
	void clearDirty() { m_modelMatrixDirty = false; };

private:
	void markDirty();
	glm::mat4 m_cachedModelMatrix{1.0f};
	bool m_modelMatrixDirty = true;

	void setEulerAngles(const glm::vec3& angle);
};

