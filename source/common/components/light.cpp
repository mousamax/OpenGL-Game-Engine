#include "light.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    // Reads Light type & color & cone angles from the given json object
    void LightComponent::deserialize(const nlohmann::json& data) {
        if (!data.is_object()) return;
        std::string lightTypeStr = data.value("lightType", "point");
        if (lightTypeStr == "directional") {
            type = LightType::DIRECTIONAL;
        }
        else if(lightTypeStr == "point"){
            type = LightType::POINT;
        }
        else
        {
            type = LightType::SPOT;
        }
        color = data.value("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        cone_angles = data.value("cone-angels", glm::vec2(1.0f, 1.0f));
    }
}