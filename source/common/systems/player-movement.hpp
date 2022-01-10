#pragma once

#include "../ecs/world.hpp"
#include "../components/player-movement.hpp"
#include "../components/camera.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#include <iostream>

namespace our
{
    class PlayerMovementSystem
    {
        Application* app; // The application in which the state runs

        public:

        void enter(Application* app){
            this->app = app;
        }

        void update(World * world, float deltaTime)
        {
            Entity * playerEntity = nullptr;

            for (auto entity : world->getEntities())
            {
                if (entity->name == "player")
                {
                    playerEntity = entity;
                    break;
                }
            }

            if (!playerEntity)
            {
                std::cout << "Player entity not found" << std::endl;
                return;
            }

            PlayerMovementComponent * controller = nullptr;

            for (auto entity: world->getEntities())
            {
                controller = entity->getComponent<PlayerMovementComponent>();
                if (controller)
                    break;
            }

            if (!controller)
            {
                std::cout << "Player movement component not found" << std::endl;
                return;
            }

            CameraComponent * camera = nullptr;

            for (auto entity: world->getEntities())
            {
                camera = entity->getComponent<CameraComponent>();
                if (camera)
                    break;
            }

            if (!camera)
            {
                std::cout << "Camera component not found" << std::endl;
                return;
            }
            // since we are changing the position  of the player
            // we need to get the translation component of the player
            glm::vec3& position = playerEntity->localTransform.position;

            // get transfomation from camera to player
            glm::mat4 matrix = playerEntity->localTransform.toMat4();

            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
                      right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));
            
            float horizontalSensitivity = controller->horizontalSensitivity;
            float forwardVelocity = controller->forwardVelocity;
            float steeringVelocity = controller->steeringVelocity;
            // We change the camera position based on the keys WASD
            if (our::GameMananger::gameOver)
            {
                forwardVelocity = 0.0f;
                steeringVelocity = 0.0f;
            }
            position += front * (deltaTime * horizontalSensitivity) * forwardVelocity;
            // A & D moves the player left or right 
            if(app->getKeyboard().isPressed(GLFW_KEY_D)) position += right * (deltaTime * steeringVelocity);
            if(app->getKeyboard().isPressed(GLFW_KEY_A)) position -= right * (deltaTime * steeringVelocity);

        }
    };

    
} // namespace name
