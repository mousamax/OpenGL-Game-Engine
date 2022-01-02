#pragma once

#include "../ecs/component.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

namespace our {

    // An enum that defines the type of the light (DIRECTIONAL, POINT or SPOT)
    enum class LightType {
        DIRECTIONAL,
        POINT,
        SPOT
    };
    // Since we may want to store colors in bytes instead of floats for efficiency,
    // we are creating our own 32-bit R8G8B8A8 Color data type with the default GLM precision
    typedef glm::vec<4, glm::uint8, glm::defaultp> Color;
    // This component adds lighting ability to the owing entity.
    class LightComponent : public Component {
    public:
        LightType type;         // The current type of light, Possible values are Directional, Point and Spot.
        Color color;            // The light color
        glm::vec2 cone_angles;       //define the angle of a spot light’s cone (Spot light only).

        // The ID of this component type is "Light"
        static std::string getID() { return "Light"; }

        // Reads light properties by the names given in the json object
        void deserialize(const nlohmann::json& data) override;
    };

}