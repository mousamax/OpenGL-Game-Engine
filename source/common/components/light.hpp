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
    // This component adds lighting ability to the owing entity.
    class LightComponent : public Component {
    public:
        LightType type;             // The current type of light, Possible values are Directional, Point and Spot.
        glm::vec3 color;            // The light color
        glm::vec3 direction;        //for directional light
        glm::vec2 cone_angles;// = glm::vec2(0.0f, 0.0f);      //define the angle of a spot light’s cone (Spot light only).
        glm::vec3 attenuation;   
        // The ID of this component type is "Light"
        static std::string getID() { return "Light"; }

        // Reads light properties by the names given in the json object
        void deserialize(const nlohmann::json& data) override;
    };

}
