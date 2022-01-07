#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our
{

    class PlayerMovementComponent : public Component
    {
    public:
        float horizontalSensitivity = 3.0f; // How sensitive the keyboard is to A & D
        float forwardVelocity = 3.0f;       // how quick does the player move forward
        float steeringVelocity = 7.0f;       // how quick does the player steer

        // The ID of this component type is "Player Movement"
        static std::string getID() { return "Player Movement"; }

        // Reads sensitivities & speedupFactor from the given json object
        void deserialize(const nlohmann::json &data) override;
    };

}