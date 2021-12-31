#pragma once

#include "../ecs/component.hpp"

namespace our {

    // An enum that defines the type of the light (DIRECTIONAL, POINT or SPOT)
    enum class LightType {
        DIRECTIONAL,
        POINT,
        SPOT
    };
    // This component adds lighting ability to the owing entity.
    class LightComponent : public Component {
    public:
        LightType type;         // The current type of light, Possible values are Directional, Point and Spot.
        Color color;            // The light color
        glm::vec2 cone_angles;       //define the angle of a spot light’s cone (Spot light only).

        // The ID of this component type is "Mesh Renderer"
        static std::string getID() { return "Light"; }

        // Reads light properties by the names given in the json object
        void deserialize(const nlohmann::json& data) override;
    };

}