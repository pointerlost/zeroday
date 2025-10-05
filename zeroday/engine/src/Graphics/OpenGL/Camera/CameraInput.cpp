//
// Created by pointerlost on 10/2/25.
//
#include "Graphics/OpenGL/Camera/CameraInput.h"

#include "Core/EngineState.h"
#include "Core/Services.h"
#include "Editor/EditorState.h"
#include "Input/Input.h"


namespace Zeroday {

    void CameraInput::Update() {
        auto& transform = Services::GetEditorState()->cameraEntity.GetComponent<TransformComponent>().m_Transform;
        auto pos = transform.GetPosition();
        const float cameraSpeed = 50.0f * Services::GetEngineState()->GetDeltaTime();

        if (!Services::GetEngineState()->IsPlaying()) {
            if (Input::g_IsKeyHeld[GLFW_KEY_W]) {
                pos += transform.GetForward() * cameraSpeed;
            }
            if (Input::g_IsKeyHeld[GLFW_KEY_S]) {
                pos -= transform.GetForward() * cameraSpeed;
            }
            if (Input::g_IsKeyHeld[GLFW_KEY_D]) {
                pos += transform.GetRight() * cameraSpeed;
            }
            if (Input::g_IsKeyHeld[GLFW_KEY_A]) {
                pos -= transform.GetRight() * cameraSpeed;
            }
            transform.SetPosition(pos);

            // glm::vec3 front;
            // front.x = cos(glm::radians(Input::yaw) * cos(glm::radians(Input::pitch)));
            // front.y = sin(glm::radians(Input::pitch));
            // front.z = sin(glm::radians(Input::yaw) * cos(glm::radians(Input::pitch)));
            // front = glm::normalize(front);
            //
            // const auto right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
            // const auto up = glm::cross(right, front);
            //
            // auto rotMat = glm::mat4(1.0f);
            // rotMat[0] = glm::vec4(right, 0.0f);
            // rotMat[1] = glm::vec4(up, 0.0f);
            // rotMat[2] = glm::vec4(-front, 0.0f);
            // transform.SetRotation(glm::quat_cast(rotMat));
        }
    }
}
