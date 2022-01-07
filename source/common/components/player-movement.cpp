#include "player-movement.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our
{
    void PlayerMovementComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        horizontalSensitivity = data.value("horizontalSensitivity", horizontalSensitivity);
        forwardVelocity = data.value("forwardVelocity", forwardVelocity);
    }
}